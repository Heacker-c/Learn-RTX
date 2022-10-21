#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <Public/The_Rest_of_Your_Life/Hitable.h>
#include <Public/The_Rest_of_Your_Life/ONB.h>

namespace RTX 
{
    class Sphere : public Hitable
    {
    public:
        Sphere(const RayVec3& center = RayVec3(0.0f), RayPrecision radius = 1.0f, CppUtil::Basic::Ptr<Material> material = nullptr) 
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
            Define::get_sphere_uv((rec.p - center) / radius, rec.uv);
            rec.material = material;
            return true;
        }

        virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
        {
            output_box = AABB(center - RayVec3(radius), center + RayVec3(radius));
            return true;
        }

        RayPrecision pdfValue(const RayVec3& o, const RayVec3& v) const override
        {
            HitRecord rec;
            if (!this->hit(ToPtr(new Ray(o, v)), 0.001f, Define::infinity, rec))
                return 0.0f;

            auto cos_theta_max = sqrt(1.0f - radius * radius / Define::length_2(center - o));
            auto solid_angle = 2.0f * Define::pi * (1.0f - cos_theta_max);

            return  1.0f / solid_angle;
        }

        RayVec3 random(const RayVec3& o) const override
        {
            RayVec3 direction = center - o;
            auto distance_squared = Define::length_2(direction);
            ONB uvw;
            uvw.build_from_w(direction);
            
            return uvw.local(Define::random_to_sphere(radius, distance_squared));
        }

        RayVec3& Center() { return center; }
        const RayVec3& Center() const { return center; }
        RayPrecision Radius() { return radius; }
    protected:
        RayVec3 center;
        RayPrecision radius;
        CppUtil::Basic::Ptr<Material> material;
    };
}
#endif 
