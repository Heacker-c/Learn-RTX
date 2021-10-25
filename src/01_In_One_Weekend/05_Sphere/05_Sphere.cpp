#include "Defines.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

bool hit_sphere(const RayVec3& center, RayPrecision radius, CppUtil::Basic::Ptr<Ray> ray)
{
    RayVec3 oc = ray->origin() - center;
    auto dir = ray->direction();
    auto a = dot(dir, dir);
    auto b = 2.0f * dot(oc, dir);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4.0f * a * c;
    return (discriminant > 0);
}

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray)
{
    if (hit_sphere(RayVec3(0.0f, 0.0f, -1.0f), 0.5f, ray))
        return RayVec3(1.0f, 0.0f, 0.0f);

    RayPrecision t = 0.5f * (normalize(ray->direction()).y + 1.0f);
    RayVec3 white(1.0f, 1.0f, 1.0f);
    RayVec3 blue(0.5f, 0.7f, 1.0f);
    return (1.0f - t) * white + t * blue;
}

int main(int argc, char** argv) {
    ImgWindow imgWindow(str_WindowTitle);
    if (!imgWindow.IsValid()) {
        printf("ERROR: Image Window Create Fail.\n");
        return 1;
    }

    Image& img = imgWindow.GetImg();
    const int val_ImgWidth = img.GetWidth();
    const int val_ImgHeight = img.GetHeight();
    const int val_ImgChannel = img.GetChannel();
    // Camera
    const RayPrecision aspect_ratio = 1.0f * val_ImgWidth / val_ImgHeight;
    RayPrecision viewport_height = 2.0f;
    RayPrecision viewport_width = aspect_ratio * viewport_height;
    RayPrecision focal_length = 1.0f;

    auto origin = RayVec3(0.0f, 0.0f, 0.0f);
    auto horizontal = RayVec3(viewport_width, 0.0f, 0.0f);
    auto vertical = RayVec3(0, viewport_height, 0.0f);
    auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - RayVec3(0.0f, 0.0f, focal_length);

    RayVec2 part = 1.0f / RayVec2(val_ImgWidth, val_ImgHeight);
    auto imgUpdate = ToPtr(new LambdaOp([&]()
        {
#pragma omp parallel for schedule(dynamic, 8)
            for (int i = 0; i < val_ImgWidth; i++)
            {
                for (int j = 0; j < val_ImgHeight; j++)
                {
                    RayPrecision u = i * part.x;
                    RayPrecision v = j * part.y;
                    CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
                    RayVec3 BGColor = rayColor(ray);
                    img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<RayPrecision>(BGColor.r, BGColor.g, BGColor.b));
                }
            }
        }, false));

    imgWindow.Run(imgUpdate);
    return 0;
}
