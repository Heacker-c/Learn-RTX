#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Defines.h"

namespace RTX 
{
	class Material : public CppUtil::Basic::HeapObj 
    {
		HEAP_OBJ_SETUP(Material)
	public:
		virtual RayVec3 emitted(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
			const RayVec2& uv, const RayVec3& p) const { return RayVec3(0.0f); }
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
			RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered, RayPrecision& pdf) const { return false; }
        virtual float scattering_pdf(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
            CppUtil::Basic::Ptr<Ray>& scattered) const { return 0; }
	};
}

#endif // !_MATERIAL_H_
