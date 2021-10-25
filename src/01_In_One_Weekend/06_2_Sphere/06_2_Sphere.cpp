#include "Defines.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

RayPrecision hit_sphere(const RayVec3& center, RayPrecision radius, CppUtil::Basic::Ptr<Ray> ray)
{
	RayVec3 oc = ray->origin() - center;
	auto dir = ray->direction();
	auto a = dot(dir, dir);
	auto half_b = dot(oc, dir);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	return discriminant < 0 ? -1.0f : (-half_b - sqrt(discriminant)) / a;
}

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray)
{
	auto t = hit_sphere(RayVec3(0.0f, 0.0f, -1.0f), 0.5f, ray);
	if (t > 0.0f)
		return 0.5f * (ray->at(t) - RayVec3(0.0f, 0.0f, -1.0f) + 1.0f);

	t = 0.5f * (normalize(ray->direction()).y + 1.0f);
	RayVec3 white(1.0f, 1.0f, 1.0f);
	RayVec3 blue(0.5f, 0.7f, 1.0f);
	return (1.0f - t)*white + t * blue;
}

int main(int argc, char ** argv) 
{
	ImgWindow imgWindow(str_WindowTitle);
	if (!imgWindow.IsValid()) {
		printf("ERROR: Image Window Create Fail.\n");
		return 1;
	}

	Image & img = imgWindow.GetImg();
	const int val_ImgWidth = img.GetWidth();
	const int val_ImgHeight = img.GetHeight();
	const int val_ImgChannel = img.GetChannel();
	// Camera
	const auto aspect_ratio = 1.0f * val_ImgWidth / val_ImgHeight;
	auto viewport_height = 2.0f;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0f;

	auto origin = RayVec3(0.0f, 0.0f, 0.0f);
	auto horizontal = RayVec3(viewport_width, 0.0f, 0.0f);
	auto vertical = RayVec3(0, viewport_height, 0.0f);
	auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - RayVec3(0.0f, 0.0f, focal_length);

	RayVec2 part = 1.0f / RayVec2(val_ImgWidth, val_ImgHeight);
    Ptr<Operation> imgUpdate = ToPtr(new LambdaOp([&]( ) 
		{
        int imgSize = val_ImgWidth * val_ImgHeight;
#pragma omp parallel for schedule(dynamic, 8)
        for (int pixelIdx = 0; pixelIdx < imgSize; pixelIdx++)
		{
            const uvec2 pixel(pixelIdx % val_ImgWidth, pixelIdx / val_ImgWidth);
            RayPrecision u = pixel.x * part.x;
            RayPrecision v = pixel.y * part.y;
            CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
            RayVec3 currentColor = rayColor(ray);
            img.SetPixel(pixel.x, pixel.y, currentColor);
        }
    }, false));

	imgWindow.Run(imgUpdate);
	return 0;
}
