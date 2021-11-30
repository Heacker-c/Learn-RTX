#include "Perlin.h"
#include <Public/The_Next_Week/RayColor.h>
#include <Public/The_Next_Week/GlobalMain.h>

using namespace RTX;
using namespace Define;

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
{
    return raySkyBox(ray, world, depth);
}

HittableList twoSpheres(RayPrecision t0, RayPrecision t1)
{
	HittableList world;
    auto pertext = ToPtr(new NoiseTexture(4.0f));
	world.add(ToPtr(new Sphere(RayVec3(0.0f, 3.0f, 0.0f), 3.0f, ToPtr(new Lambertian(pertext)))));
	world.add(ToPtr(new Sphere(RayVec3(0.0f, -300.5f, 0.0f), 300.0f, ToPtr(new Lambertian(pertext)))));

	auto BVHNode = ToPtr(new BVH(world, t0, t1));
	world.clear();
	world.add(BVHNode);

	return world;
}

void init(const RayVec2& view)
{
    RayVec3 lookfrom(13.0f, 2.0f, 3.0f);
    RayVec3 lookat(0.0f, 0.0f, 0.0f);
    RayVec3 vup(0.0f, 1.0f, 0.0f);
    RayPrecision aspect_ratio = 1.0f * view.x / view.y;
    auto dist_to_focus = length(lookfrom - lookat);
    auto aperture = 0.1f;
    g_camera = ToPtr(new MoveCamera(lookfrom, lookat,
        vup, 20.0f, aspect_ratio, aperture, dist_to_focus));

    g_world = std::move(twoSpheres(0.0f, 0.1f));
}