#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"
#include "Defines.h"

namespace RTX 
{
	using namespace CppUtil::Basic;
	class Lambertian : public Material 
	{
	public:
		Lambertian(const RayVec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		Lambertian(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}

		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
			RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered, RayPrecision& pdf) const override 
		{

			ONB uvw;
			uvw.build_from_w(rec.normal);
			auto direction = uvw.local(random_cosine_direction());
			scattered = ToPtr(new Ray(rec.p, normalize(direction), ray->time()));
			attenuation = albedo->value(rec.uv, rec.p);
			pdf = dot(uvw.w(), scattered->direction()) / pi;
			return true;
		}

		virtual float scattering_pdf(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
			CppUtil::Basic::Ptr<Ray>& scattered) const {
			auto cosine = dot(rec.normal, normalize(scattered->direction()));
			return cosine < 0 ? 0 : cosine / pi;
		}
	protected:
		CppUtil::Basic::Ptr <Texture> albedo;
	};
}

#endif // !_LAMBERTIAN_H_
