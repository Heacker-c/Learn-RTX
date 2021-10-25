#ifndef _RAYCOLOR_H_
#define _RAYCOLOR_H_

#include "HitableList.h"
#include "Material.h"

namespace RTX
{
    RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
    {
        if (depth <= 0)
            return RayVec3(0.0f);

        HitRecord rec;
        if (world.hit(ray, 0.001f, Define::infinity, rec))
        {
            CppUtil::Basic::Ptr<Ray> scattered;
            RayVec3 attenuation;
            if (rec.material->scatter(ray, rec, attenuation, scattered))
                return attenuation * rayColor(scattered, world, depth - 1);
            return RayVec3(0.0f);
        }

        auto t = 0.5f * (normalize(ray->direction()).y + 1.0f);
        RayVec3 white = RayVec3(1.0f, 1.0f, 1.0f);
        RayVec3 blue = RayVec3(0.5f, 0.7f, 1.0f);
        return (1.0f - t) * white + t * blue;
    }
}
#endif //!_RAY_H_