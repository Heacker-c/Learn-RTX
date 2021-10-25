#ifndef _PERLIN_H_
#define _PERLIN_H_

#include "Defines.h"

namespace RTX {
	using namespace glm;
	class Perlin {
	public:
		Perlin();
		RayPrecision noise(const RayVec3& p) const;
		RayPrecision turb(const RayVec3& p, int depth = 7) const;
	private:
		static const int MaxCount = 256;
		std::vector<RayVec3> ranvec;
		std::vector<int> perm_x;
		std::vector<int> perm_y;
		std::vector<int> perm_z;

		static void perlin_generate_perm(std::vector<int>& perm);
		static void permute(std::vector<int>& perm);
		static RayPrecision perlin_interp(RayVec3 c[2][2][2], RayPrecision u, RayPrecision v, RayPrecision w);
	};
	class NoiseTexture : public Texture {
	public:
		NoiseTexture(RayPrecision sc = 0.35f) : scale(sc) {}

		virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override {
			return RayVec3(1.0f) * noise.turb(scale * p);
		}

	public:
		RayPrecision scale;
		Perlin noise;
	};
}
#endif 
