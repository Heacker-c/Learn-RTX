#ifndef _REFRACTION_H_
#define _REFRACTION_H_

#include "Material.h"
#include "Defines.h"

namespace RTX
{
	using namespace CppUtil::Basic;
	using namespace Define;
	class Dielectric : public Material
	{
	public:
		Dielectric(RayPrecision refract, RayPrecision r = 1.0f, RayPrecision g = 1.0f, RayPrecision b = 1.0f, RayPrecision fuzz = 0.0f)
			:refract(refract), albedo(r,g,b), fuzz(glm::min(fuzz, 1.0f)) {}
		Dielectric(RayPrecision refract, const RayVec3& specular, RayPrecision fuzz = 0.0) :refract(refract), albedo(specular), fuzz(fuzz) {}
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			attenuation = albedo;
			RayPrecision refraction_ratio = rec.front_face ? (1.0f / refract) : refract;

			RayVec3 unit_direction = normalize(ray->direction());
			RayPrecision cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
			RayPrecision sin_theta = sqrt(1.0f - cos_theta * cos_theta);

			bool cannot_refract = refraction_ratio * sin_theta > 1.0f || random() < schlick(cos_theta, refraction_ratio);
            RayVec3 refracted;
            if (cannot_refract)
				refracted = Define::reflect(unit_direction, rec.normal);
            else
				refracted = Define::refract(unit_direction, rec.normal, refraction_ratio);
			if (fuzz > 0.001f)
				refracted += fuzz * random_in_unit_sphere();

			scattered = ToPtr(new Ray(rec.p, refracted));
			return true;
		}

		RayPrecision Fuzz() const { return fuzz; }
	protected:
		RayPrecision fuzz = 0.0;
		RayPrecision refract = 1.0;
		RayVec3 albedo = RayVec3(1.0);
	};
}

#endif // !_REFRACTION_H_
