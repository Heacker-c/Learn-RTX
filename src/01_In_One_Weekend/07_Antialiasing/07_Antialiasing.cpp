#include "Defines.h"
#include <Public/Ingredients/canvas.h>

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world)
{
	HitRecord rec;
	if (world.hit(ray, 0.001f, infinity, rec))
		return 0.5f * (rec.normal + RayVec3(1.0f));

	RayPrecision t = 0.5f * (normalize(ray->direction()).y + 1.0f);
	RayVec3 white = RayVec3(1.0f, 1.0f, 1.0f);
	RayVec3 blue = RayVec3(0.5f, 0.7f, 1.0f);
	return (1.0f - t)*white + t * blue;
}

HittableList world;
Camera::Ptr camera = nullptr;
void init(int width, int height)
{
    static bool init = true;
    if (init)
    {
        init = false;
        world.add(ToPtr(new Sphere(RayVec3(0.0f, 0.0f, -1.0f), 0.5f)));
        world.add(ToPtr(new Sphere(RayVec3(0.0f, -100.5f, -1.0f), 100.0f)));
        // Camera
        camera = ToPtr(new Camera(RayVec2(width, height)));
    }
}
void updatebuff(Canvas& canvas)
{
    static RayVec2 part = 1.0f / RayVec2(canvas.width(), canvas.height());
    init(canvas.width(), canvas.height());
#pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < canvas.height(); i++)
    {
        for (int j = 0; j < canvas.width(); j++)
        {
            RayPrecision u = (j + random()) * part.x;
            RayPrecision v = (i + random()) * part.y;
            RayVec3 color = rayColor(camera->GenRay(u, v), world);
            auto pixel = (RayVec3*)&canvas.renderBuff[(i * canvas.width() + j) * 4];
            color += *pixel * (RayVec3)canvas.currentFrame();
            *pixel = color * vec3(1.0f / (canvas.currentFrame() + 1.0f));
        }
    }
}