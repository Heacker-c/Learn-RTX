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
    auto metal = ToPtr(new Metal(RayVec3(0.8, 0.8, 0.9), 1.0f));
    world.add(ToPtr(new Sphere(RayVec3(0.0f, 150.0f, 145.0f), 50.0f, metal)));

    auto boundary = ToPtr(new Sphere(RayVec3(360.0f, 150.0f, 145.0f), 70.0f, glass));
    world.add(boundary);
    world.add(ToPtr(new ConstantMedium(boundary, 0.2, RayVec3(0.2, 0.4, 0.9))));
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

void init()
{
    g_WindowTitle = str_WindowTitle;
    g_withSkyBox = false;

    RayVec3 lookfrom(478, 278, -600);
    RayVec3 lookat(278.0f, 278.0f, 0.0f);
    RayVec3 vup(0.0f, 1.0f, 0.0f);
    RayPrecision aspect_ratio = 1.0f * g_imgSize.x / g_imgSize.y;
    RayPrecision dist_to_focus = 10.0f;
    RayPrecision aperture = 0.1f;

    g_camera = ToPtr(new MoveCamera(lookfrom, lookat, vup, 40.0f,
        aspect_ratio, aperture, dist_to_focus));

    g_world = std::move(FinalScene());
}
