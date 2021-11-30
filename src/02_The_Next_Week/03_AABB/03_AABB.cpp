#include "Metal.h"
#include "Lambertian.h"
#include "Dielectrics.h"
#include "Sphere.h"
#include "Defines.h"

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
{
    return raySkyBox(ray, world, depth);
}

HittableList creatWorld(RayPrecision t0, RayPrecision t1)
{
    const int maxCount = 11;
    HittableList world;
    for (int a = -maxCount; a < maxCount; a++) {
        for (int b = -maxCount; b < maxCount; b++) {
            auto choose_mat = random( );
            RayVec3 center(a + 0.9f * random( ), 0.2f, b + 0.9f * random( ));

            if ((center - RayVec3(4.0f, 0.2f, 0.0f)).length( ) > 0.9f)
            {
                if (choose_mat < 0.55f) {
                    // diffuse
                    auto diffuse = ToPtr(new Lambertian(randomVec3(0.0f, 1.0f)));
                    auto center2 = center + RayVec3(0, random(0.0f, 0.5f), 0.0f);
                    world.add(ToPtr(new MoveSphere(center, center2, 0.0f, 1.0f, 0.2f, diffuse)));
                }
                else if (choose_mat < 0.9f) {
                    // metal
                    auto metal = ToPtr(new Metal((RayVec3(1.0f) + random_in_unit_sphere( )) * 0.5f, random( ) * 0.5f));
                    world.add(ToPtr(new Sphere(center, 0.2f, metal)));
                }
                else {
                    // glass
                    auto  glass = ToPtr(new Dielectric(1.5f + random(-1.0f, 1.0f), (RayVec3(1.0f) + random_in_unit_sphere( )) * 0.5f));
                    world.add(ToPtr(new Sphere(center, 0.2f, glass)));
                }
            }
        }
    }

    world.add(ToPtr(new Sphere(RayVec3(6.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Metal(0.7f, 0.6f, 0.5f)))));
    world.add(ToPtr(new Sphere(RayVec3(2.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Dielectric(1.5f, 0.98f, 0.98f, 0.98f)))));
    world.add(ToPtr(new Sphere(RayVec3(2.0f, 1.0f, 0.0f), -0.96f, ToPtr(new Dielectric(1.5f, 0.98f, 0.98f, 0.98f, 0.02f)))));
    world.add(ToPtr(new Sphere(RayVec3(-2.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Lambertian(0.4f, 0.2f, 0.1f)))));
    world.add(ToPtr(new Sphere(RayVec3(-6.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Dielectric(2.5f, 0.0f, 0.98f, 0.98f)))));
    world.add(ToPtr(new Sphere(RayVec3(0.0f, -300.5f, 0.0f), 300.0f, ToPtr(new Lambertian(0.5f, 0.5f, 0.5f)))));

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

    g_world = std::move(creatWorld(0.0f, 0.1f));
}
