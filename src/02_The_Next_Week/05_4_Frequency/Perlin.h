#ifndef _PERLIN_H_
#define _PERLIN_H_

#include "Defines.h"

namespace RTX 
{
	class Perlin 
	{
	public:
		Perlin();
		RayPrecision noise(const RayVec3& p) const;
	private:
		static const int MaxCount = 256;
		std::vector<RayPrecision> ranRayPrecision;
		std::vector<int> perm_x;
		std::vector<int> perm_y;
		std::vector<int> perm_z;

		static void perlin_generate_perm(std::vector<int>& perm);
		static void permute(std::vector<int>& perm);
	};
	class NoiseTexture : public Texture 
	{
	public:
		NoiseTexture(RayPrecision sc = 0.35f) : scale(sc) {}
		virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override 
		{
			return RayVec3(1.0f) * noise.noise(scale * p);
		}
	public:
		RayPrecision scale;
		Perlin noise;
	};
}
#endif 
