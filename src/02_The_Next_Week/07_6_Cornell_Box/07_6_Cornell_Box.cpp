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

HittableList CornellBox()
{
    HittableList world;

    auto red = ToPtr(new Lambertian(RayVec3(0.65f, 0.05f, 0.05f)));
	auto white = ToPtr(new Lambertian(RayVec3(0.73f)));
	auto green = ToPtr(new Lambertian(RayVec3(0.12f, 0.45f, 0.15f)));
	auto light = ToPtr(new DiffuseLight(RayVec3(15.0f, 15.0f, 15.0f)));
	
    world.add(ToPtr(new RectangleXZ(RayVec2(213.0f, 343.0f), RayVec2(227.0f, 332.0f), 554.0f, light)));
    
    world.add(ToPtr(new RectangleYZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, green)));
	world.add(ToPtr(new RectangleYZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 0.0f, red)));
	world.add(ToPtr(new RectangleXZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 0.0f, white)));
	world.add(ToPtr(new RectangleXZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, white)));
	world.add(ToPtr(new RectangleXY(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, white)));

	auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.01f));
	world.clear();
	world.add(BVHNode);

	return world;
}

void init(const RayVec2& view)
{
    RayVec3 lookfrom(278.0f, 278.0f, -800.0f);
    RayVec3 lookat(278.0f, 278.0f, 0.0f);
    RayVec3 vup(0.0f, 1.0f, 0.0f);
    RayPrecision aspect_ratio = 1.0f * view.x / view.y;
    auto dist_to_focus = 25.0f;
    auto aperture = 0.1f;

    g_camera = ToPtr(new MoveCamera(lookfrom, lookat, vup, 40.0f,
        aspect_ratio, aperture, dist_to_focus));

    g_world = std::move(CornellBox());
}
