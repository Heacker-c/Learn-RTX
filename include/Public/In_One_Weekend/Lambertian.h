#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Defines.h"
#include "Material.h"

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
		Lambertian(RayPrecision r, RayPrecision g, RayPrecision b) :albedo(r, g, b) {}
		Lambertian(const RayVec3 & specular) :albedo(specular) {}
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			RayVec3 scatter_direction = rec.normal + Define::random_unit_vector();
            // Catch degenerate scatter direction
            if (near_zero(scatter_direction))
                scatter_direction = rec.normal;

			scattered = ToPtr(new Ray(rec.p, scatter_direction));
			attenuation = albedo;

			return true;
		}
	protected:
		RayVec3 albedo;
	};
}

#endif // !_LAMBERTIAN_H_
