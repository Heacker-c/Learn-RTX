#ifndef _GLOBALMAIN_H_
#define _GLOBALMAIN_H_

#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectrics.h"
#include "Camera.h"
#include "MoveCamera.h"

using namespace RTX;
#define useGamma 0

std::string g_WindowTitle;
glm::ivec2 g_imgSize;
HittableList g_world;
MoveCamera::Ptr g_camera = nullptr;
bool g_withSkyBox = true;
void init();
RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth);

int main(int argc, char** argv)
{
    ImgWindow imgWindow(g_WindowTitle);
    if (!imgWindow.IsValid())
    {
        printf("ERROR: Image Window Create Fail.\n");
        return 1;
    }

    Image& img = imgWindow.GetImg();
    g_imgSize = glm::ivec2(img.GetWidth(), img.GetHeight());

    init();

    RayVec2 part = 1.0f / RayVec2(g_imgSize);
    size_t samples_per_pixel = 50;
    const int maxDepth = 7;
#if 0
    size_t curSumLoop = 0;
    Timer timer;
    timer.Start();
    Ptr<Operation> imgUpdate = ToPtr(new LambdaOp([&]()
        {
#pragma omp parallel for schedule(dynamic, 8)
            for (int i = 0; i < g_imgSize.x; i++)
            {
                for (int j = 0; j < g_imgSize.y; j++)
                {
                    RayPrecision u = (i + random()) * part.x;
                    RayPrecision v = (j + random()) * part.y;
                    auto ray = g_camera->GenRay(u, v);
                    RayVec3 currentColor = rayColor(ray, g_world, maxDepth);
                    auto imgColor = img.GetPixel_F(g_imgSize.x - 1 - i, j);
                    RayVec3 color(imgColor.r, imgColor.g, imgColor.b);
#if useGamma
                    RayVec3 newColor = (color * color * (RayPrecision)curSumLoop + currentColor) / ((RayPrecision)curSumLoop + 1.0f);
                    img.SetPixel(g_imgSize.x - 1 - i, j, Image::Pixel<RayPrecision>(sqrt(newColor.r), sqrt(newColor.g), sqrt(newColor.b)));
#else
                    RayVec3 newColor = (color * (RayPrecision)curSumLoop + currentColor) / ((RayPrecision)curSumLoop + 1.0f);
                    img.SetPixel(g_imgSize.x - 1 - i, j, Image::Pixel<RayPrecision>(newColor.r, newColor.g, newColor.b));
#endif
                }
            }
            ++curSumLoop;
            double curStep = curSumLoop / (double)samples_per_pixel * 100;
            double wholeTime = timer.GetWholeTime();
            double speed = curSumLoop / wholeTime;
            double needTime = (samples_per_pixel - curSumLoop) / speed;
            double sumTime = wholeTime + needTime;
            printf("\rINFO: %.2f%%, %.2f loop / s, use %.2f s, need %.2f s, sum %.2f s     ",
                curStep, speed, wholeTime, needTime, sumTime);
            if (curSumLoop == samples_per_pixel)
                imgUpdate->SetIsHold(false);
        }));
#else
    float schedule = 0.0f;
    auto imgUpdate = ToPtr(new LambdaOp([&]()
        {
            Timer timer;
            timer.Start();

#pragma omp parallel for schedule(dynamic, 8)
            for (int i = 0; i < g_imgSize.x; i++)
            {
                for (int j = 0; j < g_imgSize.y; j++)
                {
                    RayVec3 pixelColor(0.0f);
                    for (int s = 0; s < samples_per_pixel; ++s)
                    {
                        float u = (i + random()) * part.x;
                        float v = (j + random()) * part.y;
                        auto ray = g_camera->GenRay(u, v);
                        pixelColor += rayColor(ray, g_world, maxDepth);
                    }
                    pixelColor /= samples_per_pixel;
#if useGamma
                    pixelColor = sqrt(pixelColor * pixelColor);
#endif
                    img.SetPixel(g_imgSize.x - 1 - i, j, Image::Pixel<RayPrecision>(pixelColor.r, pixelColor.g, pixelColor.b));
                }
                float curStep = (i + 1) * 100.0f / g_imgSize.x;
                if (curStep > schedule)
                {
                    schedule = curStep;
                    double wholeTime = (double)timer.GetWholeTime();
                    double speed = wholeTime / curStep;
                    double needTime = (100.0f - curStep) * speed;
                    double sumTime = wholeTime + needTime;
                    printf("\rINFO: %.2f%%, %.2f per_pixel / s, use %.2f s, need %.2f s, sum %.2f s     ",
                        curStep, speed, wholeTime, needTime, sumTime);
                }
            }
        }, false));
#endif

    imgWindow.Run(imgUpdate);
    return 0;
}
#endif
