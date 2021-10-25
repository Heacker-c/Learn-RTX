#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Ray.h"

namespace RTX 
{
    class Camera : public CppUtil::Basic::HeapObj 
    {
        HEAP_OBJ_SETUP(Camera)
    public:
        Camera()
        {
            auto aspect_ratio = 16.0f / 9.0f;
            auto viewport_height = 2.0f;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0f;

            origin = RayVec3(0.0f);
            horizontal = RayVec3(viewport_width, 0.0f, 0.0f);
            vertical = RayVec3(0.0f, viewport_height, 0.0f);
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - RayVec3(0.0f, 0.0f, focal_length);
        }

        virtual CppUtil::Basic::Ptr<Ray> GenRay(RayPrecision u, RayPrecision v) const
        {
            return ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
        }

    private:
        RayVec3 origin;
        RayVec3 lower_left_corner;
        RayVec3 horizontal;
        RayVec3 vertical;
    };
}

#endif // !_RAY_CAMERA_H_
