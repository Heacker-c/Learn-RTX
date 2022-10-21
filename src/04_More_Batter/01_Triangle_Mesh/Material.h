#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Defines.h"
#include "PDF.h"

namespace RTX 
{
    struct scatter_record 
	{
        CppUtil::Basic::Ptr<Ray> specular_ray;
        bool is_specular = false;
        RayVec3 attenuation = RayVec3(0.0f);
        CppUtil::Basic::Ptr<PDF> pdf_ptr;
    };
	class Material : public CppUtil::Basic::HeapObj 
    {
		HEAP_OBJ_SETUP(Material)
	public:
		virtual RayVec3 emitted(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
			const RayVec2& uv, const RayVec3& p) const { return RayVec3(0.0f); }
        virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, scatter_record& srec) const { return false; }
        virtual float scattering_pdf(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
            CppUtil::Basic::Ptr<Ray>& scattered) const { return 0.0f; }
	};
}

#endif // !_MATERIAL_H_
