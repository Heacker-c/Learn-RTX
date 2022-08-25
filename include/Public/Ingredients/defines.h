#ifndef _BASEDEFINES_H_
#define _BASEDEFINES_H_

#include <CppUtil/Basic/Operation.h>
#include <../Config/ROOT_PATH.h>

#include <random>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>

typedef float RayPrecision;
typedef glm::vec2 RayVec2;
typedef glm::vec3 RayVec3;

namespace Define
{
    using namespace glm;
    using namespace CppUtil;
    using namespace Basic;

	const RayVec3 background(0.005f);
    const RayPrecision infinity = (RayPrecision)std::numeric_limits<double>::infinity();
    const RayPrecision pi = RayPrecision(3.141592653589793);

	extern const std::string str_WindowTitle;

    inline std::string rootPath()
    {
        return ROOT_PATH;
    }

    inline RayPrecision random()
    {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return (RayPrecision)distribution(generator);
    }

    inline RayPrecision random(RayPrecision min, RayPrecision max)
    {
        return min + (max - min) * random();
    }

    inline RayVec3 randomVec3()
    {
        return RayVec3(random(), random(), random());
    }

    inline RayVec3 randomVec3(RayPrecision min, RayPrecision max)
    {
        return RayVec3(random(min, max), random(min, max), random(min, max));
    }

    inline RayPrecision length_2(const RayVec3& pos)
    {
        return pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
    }

    inline RayVec3 random_in_unit_sphere()
    {
        while (true)
        {
            auto p = randomVec3(-1.0f, 1.0f);
            if (length_2(p) > 1.0f)
                continue;
            return p;
        }
    }

    inline RayVec3 random_to_sphere(float radius, float distance_squared)
    {
        auto r1 = random();
        auto r2 = random();
        auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

        auto phi = 2 * pi * r1;
        auto x = cos(phi) * sqrt(1 - z * z);
        auto y = sin(phi) * sqrt(1 - z * z);

        return RayVec3(x, y, z);
    }

    inline RayVec3 random_unit_vector()
    {
        return normalize(random_in_unit_sphere());
    }

    inline RayVec3 random_in_hemisphere(const RayVec3& normal)
    {
        RayVec3 in_unit_sphere = random_in_unit_sphere();
        if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
            return in_unit_sphere;
        else
            return -in_unit_sphere;
    }

    inline RayVec3 reflect(const RayVec3& v, const RayVec3& n)
    {
        return v - 2 * dot(v, n) * n;
    }

    inline RayVec3 refract(const RayVec3& uv, const RayVec3& n, RayPrecision refraction_ratio)
    {
        RayPrecision cos_theta = min(dot(-uv, n), 1.0f);
        RayVec3 r_out_perp = refraction_ratio * (uv + cos_theta * n);
        RayPrecision length_squared = length_2(r_out_perp);
        RayVec3 r_out_parallel = -sqrt(fabs(1.0f - length_squared)) * n;
        return r_out_perp + r_out_parallel;
    }

    inline RayPrecision schlick(RayPrecision cosine, RayPrecision ref_idx)
    {
        RayPrecision r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * (RayPrecision)pow((1.0f - cosine), 5);
    }

    inline RayPrecision degrees_to_radians(RayPrecision vfov)
    {
        return vfov * pi / (RayPrecision)180.0f;
    }

    inline RayVec3 random_in_unit_disk()
    {
        while (true)
        {
            RayVec3 p = RayVec3(random(-1.0f, 1.0f), random(-1.0f, 1.0f), 0.0f);
            if (length_2(p) > 1.0f) continue;
            return p;
        }
    }

    inline int random_int(int min, int max)
    {
        return static_cast<int>(random(RayPrecision(min), RayPrecision(max + 1)));
    }

    inline void get_sphere_uv(const RayVec3& p, RayVec2& uv)
    {
        RayPrecision phi = atan2(p.z, p.x);
        RayPrecision theta = asin(p.y);
        uv.x = 1.0f - (phi + pi) / (2.0f * pi);
        uv.y = (theta + pi / 2.0f) / pi;
    }

    inline RayPrecision trilinear_interp(RayPrecision c[2][2][2], RayPrecision u, RayPrecision v, RayPrecision w)
    {
        RayPrecision accum = 0.0f;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                    accum += (i * u + (1.0f - i) * (1.0f - u)) *
                    (j * v + (1.0f - j) * (1.0f - v)) *
                    (k * w + (1.0f - k) * (1.0f - w)) * c[i][j][k];

        return accum;
    }

    inline RayPrecision pdf(RayPrecision x)
    {
        return RayPrecision(3.0) * x * x / RayPrecision(8.0);
    }

    inline RayPrecision pdf(const RayVec3& p)
    {
        return RayPrecision(1.0) / (RayPrecision(4.0) * pi);
    }

    inline RayVec3 random_cosine_direction()
    {
        RayPrecision r1 = random();
        RayPrecision r2 = random();
        RayPrecision z = sqrt(RayPrecision(1.0) - r2);

        RayPrecision phi = RayPrecision(2.0) * pi * r1;
        RayPrecision x = cos(phi) * sqrt(r2);
        RayPrecision y = sin(phi) * sqrt(r2);

        return RayVec3(x, y, z);
    }
}

#endif // !_DEFINES_H_