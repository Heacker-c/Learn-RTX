#include "Defines.h"
#include "Ray.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray)
{
	RayPrecision t = 0.5f * (normalize(ray->direction()).y + 1.0f);
	RayVec3 white = RayVec3(1.0f, 1.0f, 1.0f);
	RayVec3 blue = RayVec3(0.5f, 0.7f, 1.0f);
	return (1.0f - t) * white + t * blue;
}

int main(int argc, char ** argv)
{
	ImgWindow imgWindow(str_WindowTitle);
	if (!imgWindow.IsValid())
	{
		printf("ERROR: Image Window Create Fail.\n");
		return 1;
	}
	Image & img = imgWindow.GetImg();
	const size_t val_ImgWidth = img.GetWidth();
	const size_t val_ImgHeight = img.GetHeight();
	const size_t val_ImgChannel = img.GetChannel();
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
	auto imgUpdate = ToPtr(new LambdaOp([&]()
	{
		for (size_t i = 0; i < val_ImgWidth; i++) 
		{
			for (size_t j = 0; j < val_ImgHeight; j++)
			{
				RayPrecision u = i * part.x;
				RayPrecision v = j * part.y;
				CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
				RayVec3 BGColor = rayColor(ray);
				img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<RayPrecision>(BGColor.r, BGColor.g, BGColor.b));
			}
		}
	}, false));

	imgWindow.Run(imgUpdate);
	return 0;
}
