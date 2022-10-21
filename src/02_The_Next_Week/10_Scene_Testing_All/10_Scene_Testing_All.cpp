#include "Defines.h"
#include <Public/The_Next_Week/GlobalMain.h>

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
{
    return rayWithoutSkyBox(ray, world, depth);
}

HittableList FinalScene()
{
    HittableList world;

    HittableList box1;
    auto ground = ToPtr(new Lambertian(RayVec3(0.48, 0.83, 0.53)));
    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++)
    {
        for (int j = 0; j < boxes_per_side; j++)
        {
            auto w = 100.0f;
            auto x0 = -1000.0f + i * w;
            auto z0 = -1000.0f + j * w;
            auto y0 = 0.0f;
            auto x1 = x0 + w;
            auto y1 = random(0.0f, 101.0f);
            auto z1 = z0 + w;
            box1.add(ToPtr(new Box(RayVec3(x0, y0, z0), RayVec3(x1, y1, z1), ground)));
        }
    }
    world.add(ToPtr(new BVH(box1, 0.0f, 0.01f)));

    auto difflight = ToPtr(new DiffuseLight(RayVec3(7.0f)));
    world.add(ToPtr(new RectangleXZ(RayVec2(123.0f, 423.0f), RayVec2(147.0f, 412.0f), 554.0f, difflight)));

    auto center1 = RayVec3(400.0f, 400.0f, 200.0f);
    auto center2 = center1 + RayVec3(30.0f, 0.0f, 0.0f);
    auto moving_sphere_material = ToPtr(new Lambertian(RayVec3(0.7, 0.3, 0.1)));
    world.add(ToPtr(new MoveSphere(center1, center2, 0.0f, 1.0f, 50.0f, moving_sphere_material)));

    auto glass = ToPtr(new Dielectric(1.5f, RayVec3(0.95f)));
    world.add(ToPtr(new Sphere(RayVec3(260.0f, 150.0f, 45.0f), 50.0f, glass)));
    auto metal = ToPtr(new Metal(RayVec3(0.8f, 0.8f, 0.9f), 1.0f));
    world.add(ToPtr(new Sphere(RayVec3(0.0f, 150.0f, 145.0f), 50.0f, metal)));

    auto boundary = ToPtr(new Sphere(RayVec3(360.0f, 150.0f, 145.0f), 70.0f, glass));
    world.add(boundary);
    world.add(ToPtr(new ConstantMedium(boundary, 0.2f, RayVec3(0.2f, 0.4f, 0.9f))));
    //auto dust = ToPtr(new Sphere(RayVec3(0.0f), 5000.0f, glass));
    //world.add(ToPtr(new ConstantMedium(dust, 0.0001, RayVec3(0.95))));

    auto image = ToPtr(new ImageTexture(rootPath() + "/data/textures/earth.jpg"));
    world.add(ToPtr(new Sphere(RayVec3(400.0f, 200.0f, 400.0f), 100.0f, ToPtr(new Lambertian(image)))));

    auto pertext = ToPtr(new NoiseTexture(0.4f));
    world.add(ToPtr(new Sphere(RayVec3(220.0f, 280.0f, 300.0f), 80.0f, ToPtr(new Lambertian(pertext)))));

    HittableList box2;
    auto white = ToPtr(new Lambertian(RayVec3(0.73f)));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
        box2.add(ToPtr(new Sphere(Define::randomVec3(400.0f, 550.0f), 10.0f, white)));
    auto bvh = ToPtr(new BVH(box2, 0.0f, 0.01f));
    auto root = ToPtr(new RotateY(bvh, 15.0f));
    auto trans = ToPtr(new Translate(root, RayVec3(-450.0f, 0.0f, -100.0f)));
    world.add(trans);

    return world;
}

HittableList CornellBox()
{
	HittableList world;

	auto red = ToPtr(new Lambertian(RayVec3(0.65f, 0.05f, 0.05f)));
	auto white = ToPtr(new Lambertian(RayVec3(0.73f)));
	auto green = ToPtr(new Lambertian(RayVec3(0.12f, 0.45f, 0.15f)));
	auto light = ToPtr(new DiffuseLight(RayVec3(15.0f)));
	auto metal = ToPtr(new Metal(RayVec3(0.98f)));

	//world.add(ToPtr(new FlipFace(ToPtr(new RectangleXZ(RayVec2(213.0f, 343.0f), RayVec2(227.0f, 332.0f), 554.0f, light)))));
	world.add(ToPtr(new RectangleXZ(RayVec2(213.0f, 343.0f), RayVec2(227.0f, 332.0f), 554.0f, light)));

	world.add(ToPtr(new RectangleYZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, green)));
	world.add(ToPtr(new RectangleYZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 0.0f, red)));
	world.add(ToPtr(new RectangleXZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 0.0f, white)));
	world.add(ToPtr(new RectangleXZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, white)));
	world.add(ToPtr(new RectangleXY(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, white)));

	auto box1 = ToPtr(new Box(RayVec3(0.0f), RayVec3(165.0f, 330.0f, 165.0f), metal));
	auto rot1 = ToPtr(new RotateY(box1, 25.0f));
	auto trans1 = ToPtr(new Translate(rot1, RayVec3(265.0f, 0.0f, 295.0f)));
	world.add(trans1);

	auto box2 = ToPtr(new Box(RayVec3(0.0f), RayVec3(165.0f), white));
	auto rot2 = ToPtr(new RotateY(box2, -18.0f));
	auto trans2 = ToPtr(new Translate(rot2, RayVec3(130.0f, 0.5f, 65.0f)));
	world.add(trans2);

	auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.001f));
	world.clear();
	world.add(BVHNode);

	return world;
}

void init(const RayVec2& view)
{
    RayVec3 lookfrom(478, 278, -600);
    RayVec3 lookat(278.0f, 278.0f, 0.0f);
    RayVec3 vup(0.0f, 1.0f, 0.0f);
    RayPrecision aspect_ratio = 1.0f * view.x / view.y;
    RayPrecision dist_to_focus = 10.0f;
    RayPrecision aperture = 0.1f;

    g_camera = ToPtr(new MoveCamera(lookfrom, lookat, vup, 40.0f,
        aspect_ratio, aperture, dist_to_focus));

    g_world = std::move(FinalScene());
}
