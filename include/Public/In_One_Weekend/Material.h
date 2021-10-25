#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <CppUtil/Basic/HeapObj.h>
#include "Ray.h"
#include "Hitable.h"

namespace RTX 
{
	class Material : public CppUtil::Basic::HeapObj
	{
		HEAP_OBJ_SETUP(Material)
	public:
		HitRecord r;
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const = 0;
	};
}

#endif // !_MATERIAL_H_
