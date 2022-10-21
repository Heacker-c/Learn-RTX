#ifndef _BOX_H_
#define _BOX_H_

#include "Rectangle.h"
#include "HitableList.h"

namespace RTX 
{
	class Box : public Hitable
	{
	public:
		Box(const RayVec3& p0 = RayVec3(0.0f), const RayVec3& p1 = RayVec3(0.0f),CppUtil::Basic::Ptr<Material> material = nullptr)
        {
            box_min = p0;
            box_max = p1;

            sides.add(ToPtr(new RectangleXY(RayVec2(p0.x, p1.x), RayVec2(p0.y, p1.y), p1.z, material)));
            sides.add(ToPtr(new RectangleXY(RayVec2(p0.x, p1.x), RayVec2(p0.y, p1.y), p0.z, material)));

            sides.add(ToPtr(new RectangleXZ(RayVec2(p0.x, p1.x), RayVec2(p0.z, p1.z), p1.y, material)));
            sides.add(ToPtr(new RectangleXZ(RayVec2(p0.x, p1.x), RayVec2(p0.z, p1.z), p0.y, material)));

            sides.add(ToPtr(new RectangleYZ(RayVec2(p0.y, p1.y), RayVec2(p0.z, p1.z), p1.x, material)));
            sides.add(ToPtr(new RectangleYZ(RayVec2(p0.y, p1.y), RayVec2(p0.z, p1.z), p0.x, material)));
        }
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
        {
            return sides.hit(ray, t_min, t_max, rec);
        }
        virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
        {
            output_box = AABB(box_min, box_max);
            return true;
        }
	protected:
		RayVec3 box_min;
		RayVec3 box_max;
		HittableList sides;
	};
}
#endif 
