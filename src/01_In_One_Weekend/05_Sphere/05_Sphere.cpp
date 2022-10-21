#include "Defines.h"
#include <Public/Ingredients/canvas.h>

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

void updatebuff(Canvas& canvas)
{
    // Camera
    const auto aspect_ratio = 1.0f * canvas.width() / canvas.height();
    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    auto origin = RayVec3(0.0f, 0.0f, 0.0f);
    auto horizontal = RayVec3(viewport_width, 0.0f, 0.0f);
    auto vertical = RayVec3(0, viewport_height, 0.0f);
    auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - RayVec3(0.0f, 0.0f, focal_length);

    static RayVec2 part = 1.0f / RayVec2(canvas.width(), canvas.height());
#pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < canvas.height(); i++)
    {
        for (int j = 0; j < canvas.width(); j++)
        {
            RayPrecision u = (j + random()) * part.x;
            RayPrecision v = (i + random()) * part.y;
            RayVec3 color = rayColor(ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin)));
            auto ptr = &canvas.renderBuff[(i * canvas.width() + j) * 4];
            ptr[0] = color.r;
            ptr[1] = color.g;
            ptr[2] = color.b;
        }
    }
}