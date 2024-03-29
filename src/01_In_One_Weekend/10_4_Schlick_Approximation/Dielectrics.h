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
			RayPrecision cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
			RayPrecision sin_theta = sqrt(1.0f - cos_theta * cos_theta);

			bool cannot_refract = refraction_ratio * sin_theta > 1.0f || random() < schlick(cos_theta, refraction_ratio);
            RayVec3 direction;
            if (cannot_refract)
				direction = Define::reflect(unit_direction, rec.normal);
            else
				direction = Define::refract(unit_direction, rec.normal, refraction_ratio);

			scattered = ToPtr(new Ray(rec.p, direction));
			return true;
		}
	protected:
		RayPrecision refract;
	};
}

#endif // !_REFRACTION_H_
