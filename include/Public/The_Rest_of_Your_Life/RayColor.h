#ifndef _RAYCOLOR_H_
#define _RAYCOLOR_H_

#include "Defines.h"
#include "HitableList.h"
#include "Material.h"

using namespace RTX;
namespace Define
{
    RayVec3 rayWithoutSkyBox(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
    {
        if (depth <= 0)
            return background;

        HitRecord rec;
        if (!world.hit(ray, 0.001f, infinity, rec))
            return background;

        CppUtil::Basic::Ptr<Ray> scattered;
        RayVec3 attenuation;
        RayVec3 emitted = rec.material->emitted(rec.uv, rec.p);
        RayPrecision pdf = 0.0f;
        if (!rec.material->scatter(ray, rec, attenuation, scattered, pdf))
            return emitted;

        return emitted + attenuation * rec.material->scattering_pdf(ray, rec, scattered)
            * rayWithoutSkyBox(scattered, world, depth - 1) / pdf;
    }

    RayVec3 raySkyBox(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
    {
        if (depth <= 0)
            return background;

        HitRecord rec;
        if (world.hit(ray, 0.001f, infinity, rec))
        {
            CppUtil::Basic::Ptr<Ray> scattered;
            RayVec3 attenuation;
            float pdf = 1.0;
            if (rec.material->scatter(ray, rec, attenuation, scattered, pdf))
                return attenuation * raySkyBox(scattered, world, depth - 1);
            return RayVec3(0.0f);
        }

        auto t = 0.5f * (normalize(ray->direction()).y + 1.0f);
        RayVec3 white = RayVec3(1.0f, 1.0f, 1.0f);
        RayVec3 blue = RayVec3(0.4f, 0.63f, 1.0f);
        return (1.0f - t) * white + t * blue;
    }
}
#endif //!_RAY_H_