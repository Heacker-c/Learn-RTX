#include "Defines.h"

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

int main(int argc, char ** argv) {
	ImgWindow imgWindow(str_WindowTitle);
	if (!imgWindow.IsValid()) {
		printf("ERROR: Image Window Create Fail.\n");
		return 1;
	}

	Image & img = imgWindow.GetImg();
	const int val_ImgWidth = img.GetWidth();
	const int val_ImgHeight = img.GetHeight();
	const int val_ImgChannel = img.GetChannel();

	//World
	HittableList world;
	world.add(ToPtr(new Sphere(RayVec3(0.0f, 0.0f, -1.0f), 0.5f)));
	world.add(ToPtr(new Sphere(RayVec3(0.0f, -100.5f, -1.0f), 100.0f)));
	// Camera
	Camera::Ptr camera = ToPtr(new Camera());

	RayVec2 part = 1.0f / RayVec2(val_ImgWidth, val_ImgHeight);
	const int samples_per_pixel = 50;
	RayPrecision schedule = 0.0f;
	auto imgUpdate = ToPtr(new LambdaOp([&]()
	{
		Timer timer;
		timer.Start();
#pragma omp parallel for schedule(dynamic, 8)
		for (int i = 0; i < val_ImgWidth; i++)
		{
			for (int j = 0; j < val_ImgHeight; j++)
			{
				RayVec3 pixelColor(0.0f);
				for (int s = 0; s < samples_per_pixel; ++s)
				{
					RayPrecision u = (i + random())* part.x;
					RayPrecision v = (j + random())* part.y;
					auto ray = camera->GenRay(u, v);
					pixelColor += rayColor(ray, world);
				}
				pixelColor /= samples_per_pixel;
				img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<RayPrecision>(pixelColor.r, pixelColor.g, pixelColor.b));
			}
			RayPrecision tmp = (i + 1) * 100.0f / val_ImgWidth;
			if (tmp > schedule)
			{
				schedule = tmp;
				RayPrecision wholeTime = (RayPrecision)timer.GetWholeTime();
				printf("\rINFO: %.2f%%     wholeTime: %.2fs		", tmp, wholeTime);
			}
		}
	}, false));

	imgWindow.Run(imgUpdate);
	return 0;
}
