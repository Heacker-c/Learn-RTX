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
        RayPrecision pdfValue(const RayVec3& origin, const RayVec3& v) const
        {
            HitRecord rec;
            CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, v));
            if (!this->hit(ray, 0.0001f, infinity, rec))
                return 0;

            auto area = fabs((x.y - x.x) * (y.y - y.x));
            auto distance_squared = rec.t * rec.t * length_2(v);
            auto cosine = fabs(dot(normalize(v), rec.normal) * pi / v.length());

            return distance_squared / (cosine * area);
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
        RayPrecision pdfValue(const RayVec3& origin, const RayVec3& v) const
        {
            HitRecord rec;
            CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, v));
            if (!this->hit(ray, 0.0001f, infinity, rec))
                return 0;

            auto area = fabs((y.y - y.x) * (z.y - z.x));
            auto distance_squared = rec.t * rec.t * length_2(v);
            auto cosine = fabs(dot(normalize(v), rec.normal) * pi / v.length());

            return distance_squared / (cosine * area);
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
    class Triangle : public Hitable
    {
    public:
        Triangle(const RayVec3& p1, const RayVec3& p2, const RayVec3& p3, const RayVec2& uv1, const RayVec2& uv2, const RayVec2& uv3)
        {
            pos[0] = p1;
            pos[1] = p2;
            pos[2] = p3;

            uv[0] = uv1;
            uv[1] = uv2;
            uv[2] = uv3;
        }
        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
        {
            RayVec3 d = ray->direction();                           // 射线方向
            RayVec3 DiffPos[3] = { pos[1] - pos[0], pos[2] - pos[1], pos[0] - pos[2] };
            RayVec3 N = cross(DiffPos[0], DiffPos[2]);              // 法向量
            float s0 = length(N);
            N = normalize(N);
            if (dot(N, d) > 0.0f)
                N = -N;
            // 如果视线和三角形平行
            if (fabs(dot(N, d)) < 0.0001f)
                return false;
            // 距离
            auto t = (dot(N, pos[0]) - dot(ray->origin(), N)) / dot(d, N);
            if (t < t_min || t > t_max)
                return false;

            // 交点计算
            RayVec3 P = ray->origin() + d * t;

            // 判断交点是否在三角形中
            RayVec3 intPos[3] = { P - pos[0], P - pos[1], P - pos[2] };
            float s1 = length(cross(DiffPos[0], intPos[0]));
            float s2 = length(cross(DiffPos[1], intPos[1]));
            float s3 = length(cross(DiffPos[2], intPos[2]));
            if (abs(s0 - s1 - s2 - s3) < 0.0001f)
            {
                rec.t = t;
                rec.set_face_normal(ray, N);
                rec.p = P;
                rec.uv = (s1 * uv[0] + s2 * uv[1] + s3 * uv[2]) / s0;
                return true;
            }
            return false;
        }
        RayPrecision pdfValue(const RayVec3& origin, const RayVec3& v) const
        {
            HitRecord rec;
            CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, v));
            if (!this->hit(ray, 0.0001f, infinity, rec))
                return 0;

            auto area = length(cross(pos[1] - pos[0], pos[0] - pos[2]));
            auto distance_squared = rec.t * rec.t * length_2(v);
            auto cosine = fabs(dot(normalize(v), rec.normal) * pi / v.length());

            return distance_squared / (cosine * area);
        }
        virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
        {
            output_box = AABB(pos[0], pos[1], pos[2]);
            return true;
        }
    protected:
        RayVec3 pos[3];
        RayVec2 uv[3];
    };
}
#endif 
