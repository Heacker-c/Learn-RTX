#ifndef AABB_H
#define AABB_H

#include "Ray.h"

namespace RTX 
{
    class AABB
    {
    public:
        AABB() :_min(RayVec3(0.0f)), _max(RayVec3(0.0f)) {}
        AABB(const RayVec3& a, const RayVec3& b) :_min(a), _max(b) {}

        RayVec3 min() const { return _min; }
        RayVec3 max() const { return _max; }

        bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision tmin, RayPrecision tmax) const
        {
            for (int a = 0; a < 3; a++) 
            {
                auto invD = 1.0f / ray->direction()[a];
                auto t0 = (min()[a] - ray->origin()[a]) * invD;
                auto t1 = (max()[a] - ray->origin()[a]) * invD;
                if (invD < 0.0f)
                    std::swap(t0, t1);
                tmin = t0 > tmin ? t0 : tmin;
                tmax = t1 < tmax ? t1 : tmax;
                if (tmax <= tmin)
                    return false;
            }
            return true;
        }
        static AABB surroundingBox(const AABB& box0, const AABB& box1)
        {
            RayVec3 small(fmin(box0.min().x, box1.min().x),
                fmin(box0.min().y, box1.min().y),
                fmin(box0.min().z, box1.min().z));

            RayVec3 big(fmax(box0.max().x, box1.max().x),
                fmax(box0.max().y, box1.max().y),
                fmax(box0.max().z, box1.max().z));

            return AABB(small, big);
        }
    private:
        RayVec3 _min;
        RayVec3 _max;
    };
}
#endif //!_RAY_H_
