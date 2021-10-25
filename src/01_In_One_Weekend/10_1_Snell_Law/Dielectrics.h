#ifndef _REFRACTION_H_
#define _REFRACTION_H_

#include "Defines.h"

namespace RTX
{
	using namespace CppUtil::Basic;
	using namespace Define;
	class Dielectric : public Material
	{
	public:
		Dielectric(RayPrecision refract) :refract(refract) {}
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			attenuation = RayVec3(1.0f);
			RayPrecision refraction_ratio = rec.front_face ? (1.0f / refract) : refract;

			RayVec3 unit_direction = normalize(ray->direction());
			RayVec3 direction = Define::refract(unit_direction, rec.normal, refraction_ratio);
			scattered = ToPtr(new Ray(rec.p, direction));
			return true;
		}
	protected:
		RayPrecision refract;
	};
}

#endif // !_REFRACTION_H_
