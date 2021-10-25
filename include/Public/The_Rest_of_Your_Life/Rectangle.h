#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
#include "Hitable.h"

namespace RTX
{
    class RectangleXY : public Hitable
    {
    public:
        RectangleXY(const RayVec2& x, const RayVec2& y, RayPrecision k, CppUtil::Basic::Ptr<Material> material = nullptr) :x(x), y(y), k(k), material(material) {}
        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
        {
            auto temp = (k - ray->origin().z) / ray->direction().z;
            if (temp > t_max || temp < t_min)
                return false;
            auto xt = ray->origin().x + temp * ray->direction().x;
            auto yt = ray->origin().y + temp * ray->direction().y;
            if (xt < x.x || xt > x.y || yt < y.x || yt > y.y)
                return false;

            rec.uv = RayVec2((xt - x.x) / (x.y - x.x), (yt - y.x) / (y.y - y.x));
            rec.t = temp;
            auto outward_normal = RayVec3(0, 0, 1);
            rec.set_face_normal(ray, outward_normal);
            rec.material = this->material;
            rec.p = ray->at(temp);
            return true;
        }
        virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
        {
            output_box = AABB(RayVec3(x.x, y.x, k - 0.001f), RayVec3(x.y, y.y, k + 0.001f));
            return true;
        }
    protected:
        RayVec2 x, y;
        RayPrecision k;
        CppUtil::Basic::Ptr<Material> material;
    };

    class RectangleYZ : public Hitable
    {
    public:
        RectangleYZ(const RayVec2& y, const RayVec2& z, RayPrecision k, CppUtil::Basic::Ptr<Material> material = nullptr) :y(y), z(z), k(k), material(material) {}
        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
        {
            auto temp = (k - ray->origin().x) / ray->direction().x;
            if (temp > t_max || temp < t_min)
                return false;
            auto yt = ray->origin().y + temp * ray->direction().y;
            auto zt = ray->origin().z + temp * ray->direction().z;
            if (yt < y.x || yt > y.y || zt < z.x || zt > z.y)
                return false;

            rec.uv = RayVec2((yt - y.x) / (y.y - y.x), (zt - z.x) / (z.y - z.x));
            rec.t = temp;
            auto outward_normal = RayVec3(1.0f, 0.0f, 0.0f);
            rec.set_face_normal(ray, outward_normal);
            rec.material = this->material;
            rec.p = ray->at(temp);
            return true;
        }
        virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
        {
            output_box = AABB(RayVec3(k - 0.001f, y.x, z.x), RayVec3(k + 0.001f, y.y, z.y));
            return true;
        }
    protected:
        RayVec2 y, z;
        RayPrecision k;
        CppUtil::Basic::Ptr<Material> material;
    };

    class RectangleXZ : public Hitable
    {
    public:
        RectangleXZ(const RayVec2& x, const RayVec2& z, RayPrecision k, CppUtil::Basic::Ptr<Material> material = nullptr) :x(x), z(z), k(k), material(material) {}
        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
        {
            auto temp = (k - ray->origin().y) / ray->direction().y;
            if (temp > t_max || temp < t_min)
                return false;
            auto xt = ray->origin().x + temp * ray->direction().x;
            auto zt = ray->origin().z + temp * ray->direction().z;
            if (xt < x.x || xt > x.y || zt < z.x || zt > z.y)
                return false;

            rec.uv = RayVec2((xt - x.x) / (x.y - x.x), (zt - z.x) / (z.y - z.x));
            rec.t = temp;
            auto outward_normal = RayVec3(0.0f, 1.0f, 0.0f);
            rec.set_face_normal(ray, outward_normal);
            rec.material = this->material;
            rec.p = ray->at(temp);
            return true;
        }
        virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
        {
            output_box = AABB(RayVec3(x.x, k - 0.001f, z.x), RayVec3(x.y, k + 0.001f, z.y));
            return true;
        }
        RayPrecision pdfValue(const RayVec3& origin, const RayVec3& v) const
        {
            HitRecord rec;
            CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, v));
            if (!this->hit(ray, 0.0001f, infinity, rec))
                return 0;

            auto area = fabs((x.y - x.x) * (z.y - z.x));
            auto distance_squared = rec.t * rec.t * length_2(v);
            auto cosine = fabs(dot(normalize(v), rec.normal) * pi / v.length());

            return distance_squared / (cosine * area);
        }

        RayVec3 RectangleXZ::random(const RayVec3& origin) const 
        {
            auto randomPoint = RayVec3(Define::random(x.x, x.y), k, Define::random(z.x, z.y));
            return randomPoint - origin;
        }
    protected:
        RayVec2 x, z;
        RayPrecision k;
        CppUtil::Basic::Ptr<Material> material;
    };
}
#endif 
