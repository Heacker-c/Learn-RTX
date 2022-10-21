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
			ranRayPrecision.resize(MaxCount);
			for (int i = 0; i < MaxCount; ++i)
				ranRayPrecision[i] = Define::random();
			perlin_generate_perm(perm_x);
			perlin_generate_perm(perm_y);
			perlin_generate_perm(perm_z);
		}
		RayPrecision noise(const RayVec3& p) const
		{
			auto u = p.x - floor(p.x);
			auto v = p.y - floor(p.y);
			auto w = p.z - floor(p.z);

			u = u * u * (3.0f - 2.0f * u);
			v = v * v * (3.0f - 2.0f * v);
			w = w * w * (3.0f - 2.0f * w);

			int i = (int)floor(p.x);
			int j = (int)floor(p.y);
			int k = (int)floor(p.z);
			RayPrecision c[2][2][2];

			for (int di = 0; di < 2; di++)
				for (int dj = 0; dj < 2; dj++)
					for (int dk = 0; dk < 2; dk++)
						c[di][dj][dk] = ranRayPrecision[
							perm_x[(i + di) & 255] ^
								perm_y[(j + dj) & 255] ^
								perm_z[(k + dk) & 255]
						];

			return Define::trilinear_interp(c, u, v, w);
		}
	private:
		static const int MaxCount = 256;
		std::vector<RayPrecision> ranRayPrecision;
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
	};
    class NoiseTexture : public Texture 
	{
    public:
        NoiseTexture() {}

        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override 
		{
            return RayVec3(1.0f) * noise.noise(p);
        }

    public:
        Perlin noise;
    };
}
#endif 
