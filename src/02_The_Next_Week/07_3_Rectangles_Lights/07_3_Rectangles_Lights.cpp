#include "Defines.h"
#include <Public/The_Next_Week/RayColor.h>
#include <Public/The_Next_Week/GlobalMain.h>

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
{
    return rayWithoutSkyBox(ray, world, depth);
}

HittableList earth() 
{
	HittableList world;
	auto image = ToPtr(new ImageTexture(rootPath() + "/data/textures/earth.jpg"));
	world.add(ToPtr(new Sphere(RayVec3(0.0f, 1.0f, 0.0f), 1.5f, ToPtr(new Lambertian(image)))));
	auto pertext = ToPtr(new NoiseTexture(4.0f));
	world.add(ToPtr(new Sphere(RayVec3(0.0f, -300.5f, 0.0f), 300.0f, ToPtr(new Lambertian(pertext)))));

    auto light = ToPtr(new DiffuseLight(RayVec3(4.0f)));
	world.add(ToPtr(new RectangleXY(RayVec2(-1.5f, 1.5f), RayVec2(-1.5f, 1.5f), 2.5f, light)));

	world.add(ToPtr(new Sphere(RayVec3(0.0f, 4.5f, 0.0f), 1.25f, light)));

	auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.01f));
	world.clear();
	world.add(BVHNode);

	return world;
}

void init()
{
    g_WindowTitle = str_WindowTitle;
    g_withSkyBox = false;
    RayVec3 lookfrom(15.0f, 2.0f, 0.0f);
    RayVec3 lookat(0.0f, 1.0f, 0.0f);
    RayVec3 vup(0.0f, 1.0f, 0.0f);
    RayPrecision aspect_ratio = 1.0f * g_imgSize.x / g_imgSize.y;
    auto dist_to_focus = 25.0f;
    auto aperture = 0.1f;
    g_camera = ToPtr(new MoveCamera(lookfrom, lookat,
        vup, 20.0f, aspect_ratio, aperture, 10.0));

    g_world = std::move(earth());
}
