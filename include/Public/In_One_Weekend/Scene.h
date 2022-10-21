#ifndef _SCENE_H_
#define _SCENE_H_

#include "Defines.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectrics.h"
#include "Camera.h"
#include "RayColor.h"

namespace RTX
{
    HittableList creatWorld()
    {
        HittableList world;
        for (int a = -11; a < 11; a++)
        {
            for (int b = -11; b < 11; b++)
            {
                auto choose_mat = random();
                RayVec3 center(a + 0.9f * random(), 0.2f, b + 0.9f * random());

                if ((center - RayVec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
                {
                    if (choose_mat < 0.55f) {
                        // diffuse
                        auto diffuse = ToPtr(new Lambertian(randomVec3(0.0f, 1.0f)));
                        world.add(ToPtr(new Sphere(center, 0.2f, diffuse)));
                    }
                    else if (choose_mat < 0.9f) {
                        // metal
                        auto metal = ToPtr(new Metal((RayVec3(1.0f) + random_in_unit_sphere()) * 0.5f, random() * 0.5f));
                        world.add(ToPtr(new Sphere(center, 0.2f, metal)));
                    }
                    else {
                        // glass
                        auto  glass = ToPtr(new Dielectric(1.5f + random(-1.0f, 1.0f), (RayVec3(1.0f) + random_in_unit_sphere()) * 0.5f));
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

        return world;
    }
}
#endif //!_RAY_H_