#ifndef _MOVECAMERA_H_
#define _MOVECAMERA_H_

#include "Ray.h"

namespace RTX 
{
    using namespace Define;
    class MoveCamera : public CppUtil::Basic::HeapObj 
    {
        HEAP_OBJ_SETUP(MoveCamera)
    public:
        // fov is top to bottom in degrees
        MoveCamera(RayVec3 lookfrom, RayVec3 lookat, RayVec3 vup, RayPrecision vfov, RayPrecision aspect_ratio,
            RayPrecision aperture, RayPrecision focus_dist, RayPrecision t0 = 0.0f, RayPrecision t1 = 0.0f)
        {
            RayPrecision theta = degrees_to_radians(vfov);
            RayPrecision h = tan(theta / 2.0f);
            RayPrecision viewport_height = 2.0f * h;
            RayPrecision viewport_width = aspect_ratio * viewport_height;

            w = normalize(lookfrom - lookat);
            u = normalize(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - focus_dist * w;

            lens_radius = aperture / 2.0f;
            time0 = t0;
            time1 = t1;
        }
        virtual CppUtil::Basic::Ptr<Ray> GenRay(RayPrecision s, RayPrecision t) const
        {
            RayVec3 rd = lens_radius * random_in_unit_disk();
            RayVec3 offset = u * rd.x + v * rd.y;
            return ToPtr(new Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random(time0, time1)));
        }
    private:
        RayVec3 origin;
        RayVec3 lower_left_corner;
        RayVec3 horizontal;
        RayVec3 vertical;
        RayVec3 u, v, w;
        RayPrecision lens_radius;
        RayPrecision time0, time1;
    };
}

#endif // !_MOVECAMERA_H_
