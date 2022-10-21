#ifndef _GLOBALMAIN_H_
#define _GLOBALMAIN_H_

#include "HitableList.h"
#include "Camera.h"
#include "RayColor.h"
#include <Public/Ingredients/canvas.h>

HittableList g_world;
Camera::Ptr g_camera;

void init(const RayVec2& view);

bool near_zero(const vec3& e)
{
	return (e[0] < 0.00001f) && (e[1] < 0.00001f) && (e[2] < 0.00001f);
}

void updatebuff(Canvas& canvas)
{
    static RayVec2 part = 1.0f / RayVec2(canvas.width(), canvas.height());
    static bool needInit = true;
    if (needInit)
    {
        auto vv = RayVec2(canvas.width(), canvas.height());
        init(RayVec2(canvas.width(), canvas.height()));
        needInit = false;
    }
#pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < canvas.height(); i++)
    {
        for (int j = 0; j < canvas.width(); j++)
        {
            RayPrecision u = (j + random()) * part.x;
            RayPrecision v = (i + random()) * part.y;
            RayVec3 color = rayColor(g_camera->GenRay(u, v), g_world, canvas.maxDepth());
            if (!near_zero(color))
            {
                auto pixel = (RayVec3*)&canvas.renderBuff[(i * canvas.width() + j) * 4];
                color += *pixel * (RayVec3)canvas.currentFrame();
                *pixel = color * RayVec3(1.0f / (canvas.currentFrame() + 1.0f));
            }
        }
    }
}
#endif