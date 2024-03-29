#ifndef _PERLIN_H_
#define _PERLIN_H_

#include "Defines.h"

namespace RTX 
{
    class Perlin 
    {
    public:
        Perlin()
        {
            ranvec.resize(MaxCount);
            for (int i = 0; i < MaxCount; ++i)
                ranvec[i] = Define::randomVec3(-1.0f, 1.0f);
            perlin_generate_perm(perm_x);
            perlin_generate_perm(perm_y);
            perlin_generate_perm(perm_z);
        }
        RayPrecision noise(const RayVec3& p) const
        {
            auto u = p.x - floor(p.x);
            auto v = p.y - floor(p.y);
            auto w = p.z - floor(p.z);
            int i = (int)floor(p.x);
            int j = (int)floor(p.y);
            int k = (int)floor(p.z);
            RayVec3 c[2][2][2];

            for (int di = 0; di < 2; di++)
            {
                for (int dj = 0; dj < 2; dj++)
                {
                    for (int dk = 0; dk < 2; dk++)
                    {
                        c[di][dj][dk] = ranvec[
                            perm_x[(i + di) & 255] ^
                                perm_y[(j + dj) & 255] ^
                                perm_z[(k + dk) & 255]
                        ];
                    }
                }
            }
            return perlin_interp(c, u, v, w);
        }
        RayPrecision turb(const RayVec3& p, int depth = 7) const
        {
            auto accum = 0.0f;
            auto temp_p = p;
            auto weight = 1.0f;

            for (int i = 0; i < depth; i++)
            {
                accum += weight * noise(temp_p);
                weight *= 0.5f;
                temp_p *= 2.0f;
            }
            return fabs(accum);
        }
    private:
        static const int MaxCount = 256;
        std::vector<RayVec3> ranvec;
        std::vector<int> perm_x;
        std::vector<int> perm_y;
        std::vector<int> perm_z;

        static void perlin_generate_perm(std::vector<int>& perm)
        {
            perm.resize(MaxCount);
            for (int i = 0; i < Perlin::MaxCount; i++)
                perm[i] = i;

            permute(perm);
        }

        static void permute(std::vector<int>& perm)
        {
            for (int i = perm.size() - 1; i > 0; --i)
            {
                int target = Define::random_int(0, i);
                int tmp = perm[i];
                perm[i] = perm[target];
                perm[target] = tmp;
            }
        }
        static RayPrecision perlin_interp(RayVec3 c[2][2][2], RayPrecision u, RayPrecision v, RayPrecision w)
        {
            auto uu = u * u * (3.0f - 2.0f * u);
            auto vv = v * v * (3.0f - 2.0f * v);
            auto ww = w * w * (3.0f - 2.0f * w);
            auto accum = 0.0f;

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        RayVec3 weight_v(u - i, v - j, w - k);
                        accum += (i * uu + (1.0f - i) * (1.0f - uu))
                            * (j * vv + (1.0f - j) * (1.0f - vv))
                            * (k * ww + (1.0f - k) * (1.0f - ww))
                            * dot(c[i][j][k], weight_v);
                    }
                }
            }
            return accum;
        }
    };
}
#endif 
