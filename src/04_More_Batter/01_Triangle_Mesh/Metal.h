#ifndef _METAL_H_
#define _METAL_H_

#include "Material.h"
#include "Defines.h"

namespace RTX 
{
	using namespace CppUtil::Basic;
	using namespace Define;
	class Metal : public Material
	{
	public:
		Metal(float r, float g, float b, float fuzz = 0.0f) :albedo(r,g,b), fuzz(std::min(fuzz, 1.0f)) {}
		Metal(const RayVec3 & specular, float fuzz = 0.0f) :albedo(specular), fuzz(fuzz) {}
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, scatter_record& srec) const override
		{
			RayVec3 reflected = reflect(normalize(ray->direction()), rec.normal);
            if (fuzz > 0.001f)
				reflected += fuzz * random_in_unit_sphere();
			srec.specular_ray = ToPtr(new Ray(rec.p, reflected));
			srec.attenuation = albedo;
            srec.is_specular = true;
            srec.pdf_ptr = 0;
            return true;
		}
		float Fuzz() const { return fuzz; }
	protected:
		float fuzz;
		RayVec3 albedo;
	};
}

#endif // !_METAL_H_
