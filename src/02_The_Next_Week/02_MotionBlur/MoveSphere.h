#ifndef _MOVESPHERE_H_
#define _MOVESPHERE_H_

#include "Hitable.h"

namespace RTX 
{
	class MoveSphere : public Hitable
	{
    public:
		MoveSphere(const RayVec3& center0 = RayVec3(0.0f), const RayVec3& center1 = RayVec3(0.0f), RayPrecision t0 = 0.0f,
            RayPrecision t1 = 0.0f, RayPrecision radius = 1.0f, CppUtil::Basic::Ptr<Material> material = nullptr) 
            :center0(center0), center1(center1), time0(t0), time1(t1), radius(radius), material(material) {}
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
		{
            RayVec3 oc = ray->origin() - center(ray->time());
            auto dir = ray->direction();
            auto a = dot(dir, dir);
            auto half_b = dot(oc, dir);
            auto c = dot(oc, oc) - radius * radius;
            auto discriminant = half_b * half_b - a * c;

            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root) {
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            rec.t = root;
            rec.p = ray->at(rec.t);
            rec.set_face_normal(ray, (rec.p - center(ray->time())) / radius);
            rec.material = material;
            return true;
		}
        RayVec3 MoveSphere::center(RayPrecision time) const
        {
            return center0 + (center1 - center0) * ((time - time0) / (time1 - time0));
        }
		RayPrecision Radius() { return radius; }
	protected:
		RayVec3 center0, center1;
		RayPrecision radius, time0, time1;
		CppUtil::Basic::Ptr<Material> material;
	};
}
#endif 
