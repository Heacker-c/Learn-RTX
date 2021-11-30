#include "Defines.h"

void init(const RayVec2& view)
{
    RayVec3 lookfrom(13.0f, 2.0f, 3.0f);
    RayVec3 lookat(0.0f, 0.0f, 0.0f);
    RayVec3 vup(0.0f, 1.0f, 0.0f);
    RayPrecision aspect_ratio = 1.0f * view.x / view.y;
    auto dist_to_focus = length(lookfrom - lookat);
    auto aperture = 0.1f;
    g_camera = ToPtr(new Camera(lookfrom, lookat,
        vup, 20.0f, aspect_ratio, aperture, dist_to_focus));

    g_world = std::move(creatWorld());
}