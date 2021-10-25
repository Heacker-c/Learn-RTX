#ifndef _ISOTROPIC_H_
#define _ISOTROPIC_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"
#include "Texture.h"

namespace RTX 
{
	class Isotropic : public Material 
	{
	public:
		Isotropic(const RayVec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		Isotropic(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}

		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered, RayPrecision& pdf) const override
		{
			scattered = ToPtr(new Ray(rec.p, Define::random_in_unit_sphere(), ray->time()));
			attenuation = albedo->value(rec.uv, rec.p);
			return true;
		}
	protected:
		CppUtil::Basic::Ptr<Texture> albedo;
	};
}
#endif // !_ISOTROPIC_H_
