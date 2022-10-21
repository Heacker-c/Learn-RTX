#include "Metal.h"
#include "Lambertian.h"
#include "Dielectrics.h"
#include "DiffuseLight.h"
#include "ConstantMedium.h"
#include "PDF.h"
#include "Defines.h"
#include <Public/The_Rest_of_Your_Life/GlobalMain.h>

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

auto light_shape = ToPtr(new RectangleXZ(RayVec2(213.0f, 343.0f), RayVec2(227.0f, 332.0f), 554.0f, nullptr));

RayVec3 ray_color(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, CppUtil::Basic::Ptr<Hitable> lights, int depth)
{
	if (depth <= 0)
		return background;

	HitRecord rec;
	if (!world.hit(ray, 0.001f, infinity, rec))
		return background;

	CppUtil::Basic::Ptr<Ray> scattered;
	RayVec3 emitted = rec.material->emitted(ray, rec, rec.uv, rec.p);

	float pdf;
	RayVec3 albedo;

	if (!rec.material->scatter(ray, rec, albedo, scattered, pdf))
		return emitted;

	auto p0 = ToPtr(new HittablePDF(lights, rec.p));
	auto p1 = ToPtr(new CosinePDF(rec.normal));
	MixturePDF mixturepPDF(p0, p1);

	scattered = ToPtr(new Ray(rec.p, mixturepPDF.generate(), ray->time()));
	pdf = mixturepPDF.value(scattered->direction());

	return emitted + albedo * rec.material->scattering_pdf(ray, rec, scattered)
		* ray_color(scattered, world, lights, depth - 1) / pdf;
}

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
{
    return ray_color(ray, world, light_shape, depth);
}

HittableList CornellBox() 
{
	HittableList world;

	auto red = ToPtr(new Lambertian(RayVec3(0.65f, 0.05f, 0.05f)));
	auto white = ToPtr(new Lambertian(RayVec3(0.73f)));
	auto green = ToPtr(new Lambertian(RayVec3(0.12f, 0.45f, 0.15f)));
	auto light = ToPtr(new DiffuseLight(RayVec3(15.0f)));
	
	world.add(ToPtr(new FlipFace(ToPtr(new RectangleXZ(RayVec2(213.0f, 343.0f), RayVec2(227.0f, 332.0f), 554.0f, light)))));
	world.add(ToPtr(new RectangleYZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, green)));
	world.add(ToPtr(new RectangleYZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 0.0f, red)));
	world.add(ToPtr(new RectangleXZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 0.0f, white)));
	world.add(ToPtr(new RectangleXZ(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, white)));
	world.add(ToPtr(new RectangleXY(RayVec2(0.0f, 555.0f), RayVec2(0.0f, 555.0f), 555.0f, white)));

	auto box1 = ToPtr(new Box(RayVec3(0.0f), RayVec3(165.0f, 330.0f, 165.0f), white));
	auto rot1 = ToPtr(new RotateY(box1, 15.0f));
	auto trans1 = ToPtr(new Translate(rot1, RayVec3(265.0f, 0.0f, 295.0f)));
	world.add(trans1);

	auto box2 = ToPtr(new Box(RayVec3(0.0f), RayVec3(165.0f), white));
	auto rot2 = ToPtr(new RotateY(box2, -18.0f));
	auto trans2 = ToPtr(new Translate(rot2, RayVec3(130.0f, 0.5f, 65.0f)));
	world.add(trans2);

	auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.001f));
	world.clear();
	world.add(BVHNode);

	return world;
}

void init(const RayVec2& view)
{
    RayVec3 lookfrom(278.0f, 278.0f, -800.0f);
    RayVec3 lookat(278.0f, 278.0f, 0.0f);
    RayVec3 vup(0.0f, 1.0f, 0.0f);
    RayPrecision aspect_ratio = 1.0f * view.x / view.y;
    auto dist_to_focus = 10.0f;
    auto aperture = 0.1f;

    g_camera = ToPtr(new MoveCamera(lookfrom, lookat, vup, 40.0f,
        aspect_ratio, aperture, dist_to_focus));

    g_world = std::move(CornellBox());
}
