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

			auto i = static_cast<int>(4 * p.x) & 255;
			auto j = static_cast<int>(4 * p.y) & 255;
			auto k = static_cast<int>(4 * p.z) & 255;

			return ranRayPrecision[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
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

			permute(perm, MaxCount);
		}
		static void permute(std::vector<int>& perm, int n)
		{
			for (int i = n - 1; i > 0; i--)
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
        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override { return RayVec3(1.0f) * noise.noise(p); }
    public:
        Perlin noise;
    };
}
#endif 
