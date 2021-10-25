#ifndef _FlipFace_H
#define _FlipFace_H

#include "Defines.h"

namespace RTX
{
    class FlipFace : public Hitable
    {
    public:
        FlipFace() {}
        FlipFace(Hitable::CPtr object): objects(object){ }
        virtual bool boundingBox(float t0, float t1, AABB& output_box) const override{ return objects->boundingBox(t0, t1, output_box); }
        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, HitRecord& rec) const override
        {
			if (!objects->hit(ray, t_min, t_max, rec))
				return false;

			rec.front_face = !rec.front_face;
			return true;
        }
    public:
        Hitable::CPtr objects;
    };
}
#endif