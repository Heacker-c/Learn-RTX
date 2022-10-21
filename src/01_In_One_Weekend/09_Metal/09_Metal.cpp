#include "Defines.h"
#include <Public/Ingredients/canvas.h>

using namespace RTX;

void init(const RayVec2& view)
{
    g_camera = ToPtr(new Camera(view));

    g_world.add(ToPtr(new Sphere(RayVec3(-1.1f, 0.0f, -1.0f), 0.5f, ToPtr(new Metal(0.98f, 0.839f, 0.35f, 0.05f)))));
    g_world.add(ToPtr(new Sphere(RayVec3(0.0f, 0.0f, -1.0f), 0.5f, ToPtr(new Lambertian(0.7f, 0.3f, 0.3f)))));
    g_world.add(ToPtr(new Sphere(RayVec3(1.1f, 0.0f, -1.0f), 0.5f, ToPtr(new Metal(0.8f, 0.8f, 0.8f, 0.35f)))));
    g_world.add(ToPtr(new Sphere(RayVec3(0.0f, -100.5f, -1.0f), 100.0f, ToPtr(new Lambertian(0.0f, 0.8f, 0.8f)))));
}
