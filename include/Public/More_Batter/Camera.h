#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <CppUtil/Basic/HeapObj.h>
#include "Defines.h"

namespace RTX
{
    using namespace Define;
    class Camera : public CppUtil::Basic::HeapObj
    {
        HEAP_OBJ_SETUP(Camera)
    public:
        // fov is top to bottom in degrees
        Camera(const RayVec3& lookfrom, const RayVec3& lookat, const RayVec3& vup,
            RayPrecision vfov, RayPrecision aspect_ratio, RayPrecision aperture, RayPrecision focus_dist)
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
        }

        virtual CppUtil::Basic::Ptr<Ray> GenRay(RayPrecision s, RayPrecision t) const
        {
            RayVec3 rd = lens_radius * random_in_unit_disk( );
            RayVec3 offset = u * rd.x + v * rd.y;
            return ToPtr(new Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset));
        }

    private:
        RayVec3 origin;
        RayVec3 lower_left_corner;
        RayVec3 horizontal;
        RayVec3 vertical;
        RayVec3 u, v, w;
        RayPrecision lens_radius;
    };
}

#endif // !_RAY_CAMERA_H_
