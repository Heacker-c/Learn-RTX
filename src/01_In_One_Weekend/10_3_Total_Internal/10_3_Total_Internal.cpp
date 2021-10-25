#include "Dielectrics.h"
#include "Defines.h"

using namespace RTX;

void init()
{
    g_WindowTitle = str_WindowTitle;

    g_camera = ToPtr(new Camera());

    g_world.add(ToPtr(new Sphere(RayVec3(-1.1f, 0.0f, -1.0f), 0.5f, ToPtr(new Metal(0.98f, 0.839f, 0.35f, 0.05f)))));
    g_world.add(ToPtr(new Sphere(RayVec3(0.0f, 0.0f, -1.0f), 0.5f, ToPtr(new Dielectric(1.5f)))));
    g_world.add(ToPtr(new Sphere(RayVec3(1.1f, 0.0f, -1.0f), 0.5f, ToPtr(new Lambertian(0.1f, 0.2f, 0.5f)))));
    g_world.add(ToPtr(new Sphere(RayVec3(0.0f, -100.5f, -1.0f), 100.0f, ToPtr(new Lambertian(0.8f, 0.8f, 0.0f)))));
}
