#include "Defines.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectrics.h"
#include "RayColor.h"

std::string g_WindowTitle;
ivec2 g_imgSize;
HittableList g_world;
Camera::Ptr g_camera;
void init();

int main(int argc, char** argv)
{
    ImgWindow imgWindow(g_WindowTitle);
    if (!imgWindow.IsValid())
    {
        printf("ERROR: Image Window Create Fail.\n");
        return 1;
    }

    Image& img = imgWindow.GetImg();
    g_imgSize = ivec2(img.GetWidth(),img.GetHeight());

    init();

    RayVec2 part = 1.0f / RayVec2(g_imgSize);
    size_t maxSumLoop = 500;
    size_t curSumLoop = 0;
    const int maxDepth = 15;
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
                    auto currentColor = rayColor(ray, g_world, maxDepth);

                    auto imgColor = img.GetPixel_F(g_imgSize.x - 1 - i, j);
                    RayVec3 color(imgColor.r, imgColor.g, imgColor.b);
                    RayVec3 newColor = (color * (RayPrecision)curSumLoop + currentColor) / ((RayPrecision)curSumLoop + 1.0f);
                    img.SetPixel(g_imgSize.x - 1 - i, j, Image::Pixel<RayPrecision>(newColor.r, newColor.g, newColor.b));
                }
            }
            ++curSumLoop;
            double curStep = curSumLoop / (double)maxSumLoop * 100;
            double wholeTime = timer.GetWholeTime();
            double speed = curSumLoop / wholeTime;
            double needTime = (maxSumLoop - curSumLoop) / speed;
            double sumTime = wholeTime + needTime;
            printf("\rINFO: %.2f%%, %.2f loop / s, use %.2f s, need %.2f s, sum %.2f s     ",
                curStep, speed, wholeTime, needTime, sumTime);
            if (curSumLoop == maxSumLoop)
                imgUpdate->SetIsHold(false);
        }));
    imgWindow.Run(imgUpdate);
    return 0;
}