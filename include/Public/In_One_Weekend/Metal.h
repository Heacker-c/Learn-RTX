#ifndef _METAL_H_
#define _METAL_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"

namespace RTX 
{
	using namespace CppUtil::Basic;
	using namespace Define;
	class Metal : public Material
	{
	public:
		Metal(RayPrecision r, RayPrecision g, RayPrecision b, RayPrecision fuzz = 0.0f) :albedo(r,g,b), fuzz(std::min(fuzz, 1.0f)) {}
		Metal(const RayVec3 & specular, RayPrecision fuzz = 0.0f) :albedo(specular), fuzz(fuzz) {}
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			RayVec3 reflected = reflect(normalize(ray->direction()), rec.normal);

			if (fuzz > 0.001f)
				reflected += fuzz * random_in_unit_sphere();

			scattered = ToPtr(new Ray(rec.p, reflected));
			attenuation = albedo;
			return (dot(scattered->direction(), rec.normal) > 0);
		}

		RayPrecision Fuzz() const { return fuzz; }
	protected:
		RayPrecision fuzz = 0.0f;
		RayVec3 albedo;
	};
}

#endif // !_METAL_H_
