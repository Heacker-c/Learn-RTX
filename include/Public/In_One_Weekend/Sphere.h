#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Hitable.h"

namespace RTX 
{
    class Sphere : public Hitable
    {
    public:
        Sphere(const RayVec3& center, RayPrecision radius, CppUtil::Basic::Ptr<Material> material) 
            : center(center), radius(radius), material(material) {}
        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
        {
            RayVec3 oc = ray->origin() - center;
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
            auto outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(ray, outward_normal);
            rec.material = material;

            return true;
        }

        RayVec3& Center() { return center; }
        const RayVec3& Center() const { return center; }
        RayPrecision Radius() { return radius; }
    protected:
        RayVec3 center = RayVec3(0.0f);
        RayPrecision radius = 1.0f;
        CppUtil::Basic::Ptr<Material> material = nullptr;
    };
}
#endif 
