#ifndef _HITABLE_H_
#define _HITABLE_H_ 

#include "Ray.h"

namespace RTX 
{
	class Material;
	struct HitRecord 
	{
        RayVec3 p;
        RayVec3 normal;
		RayPrecision t;
		bool front_face;
		CppUtil::Basic::Ptr<Material> material;

		inline void set_face_normal(CppUtil::Basic::Ptr<Ray> ray, const RayVec3& outward_normal) 
		{
			front_face = dot(ray->direction(), outward_normal) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
	};
	class Hitable : public CppUtil::Basic::HeapObj 
	{
		HEAP_OBJ_SETUP(Hitable)
	public:
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const = 0;
	};
}
#endif