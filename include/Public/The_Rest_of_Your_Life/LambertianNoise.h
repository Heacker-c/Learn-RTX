#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"
#include "TextureNoise.h"

namespace RTX 
{
	using namespace CppUtil::Basic;
	class Lambertian : public Material 
	{
	public:
        bool near_zero(const RayVec3& e) const
        {
            return (fabs(e[0]) < FLT_EPSILON) && (fabs(e[1]) < FLT_EPSILON) && (fabs(e[2]) < FLT_EPSILON);
        }
		Lambertian(const RayVec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		Lambertian(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			RayVec3 scatter_direction = rec.normal + Define::random_unit_vector();
            // Catch degenerate scatter direction
            if (near_zero(scatter_direction))
                scatter_direction = rec.normal;
			scattered = ToPtr(new Ray(rec.p, scatter_direction,ray->time()));
			attenuation = albedo->value(rec.uv, rec.p);
			return true;
		}
	protected:
		CppUtil::Basic::Ptr <Texture> albedo;
	};
}

#endif // !_LAMBERTIAN_H_
