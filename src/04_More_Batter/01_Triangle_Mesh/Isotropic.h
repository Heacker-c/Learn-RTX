#ifndef _ISOTROPIC_H_
#define _ISOTROPIC_H_

#include "Material.h"
#include "Defines.h"

namespace RTX 
{
	using namespace CppUtil::Basic;
	class Isotropic : public Material 
	{
	public:
		Isotropic(const RayVec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		Isotropic(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}

		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, scatter_record& srec) const override
		{
			srec.is_specular = false;
			srec.specular_ray = ToPtr(new Ray(rec.p, Define::random_in_unit_sphere(), ray->time()));
			srec.attenuation = albedo->value(rec.uv, rec.p);
			return true;
		}
	protected:
		CppUtil::Basic::Ptr<Texture> albedo;
	};
}
#endif // !_ISOTROPIC_H_
