#include "Defines.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

RayVec3 rayColor(CppUtil::Basic::Ptr<Ray> ray, HittableList& world, int depth)
{
    if (depth <= 0)
        return RayVec3(0.005f);
    HitRecord rec;
    if (world.hit(ray, 0.001f, infinity, rec))
    {
        RayVec3 target = rec.normal + random_unit_vector();
        return 0.5f * rayColor(ToPtr(new Ray(rec.p, target)), world, depth - 1);
    }

    auto t = 0.5f * (normalize(ray->direction()).y + 1.0f);
    RayVec3 white = RayVec3(1.0f, 1.0f, 1.0f);
    RayVec3 blue = RayVec3(0.5f, 0.7f, 1.0f);
    return (1.0f - t) * white + t * blue;
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

	//World
	HittableList world;
	world.add(ToPtr(new Sphere(RayVec3(0.0f, 0.0f, -1.0f), 0.5f)));
	world.add(ToPtr(new Sphere(RayVec3(0.0f, -100.5f, -1.0f), 100.0f)));
	// Camera
	Camera::Ptr camera = ToPtr(new Camera());

    RayVec2 part = 1.0f / RayVec2(val_ImgWidth, val_ImgHeight);
    size_t maxSumLoop = 50;
    size_t curSumLoop = 0;
    const int maxDepth = 7;
    Timer timer;
    timer.Start( );
    Ptr<Operation> imgUpdate = ToPtr(new LambdaOp([&]( )
    {
#pragma omp parallel for schedule(dynamic, 8)
        for (int i = 0; i < val_ImgWidth; i++)
        {
            for (int j = 0; j < val_ImgHeight; j++)
            {
                RayPrecision u = (i + random( ))* part.x;
                RayPrecision v = (j + random( ))* part.y;
                auto ray = camera->GenRay(u, v);
                auto currentColor = rayColor(ray, world, maxDepth);

                auto imgColor = img.GetPixel_F(val_ImgWidth - 1 - i, j);
                RayVec3 color(imgColor.r, imgColor.g, imgColor.b);
                RayVec3 newColor = (color * (RayPrecision)curSumLoop + currentColor) / ((RayPrecision)curSumLoop + 1.0f);
                img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<RayPrecision>(newColor.r, newColor.g, newColor.b));
            }
        }
        ++curSumLoop;
        double curStep = curSumLoop / (double)maxSumLoop * 100;
        double wholeTime = timer.GetWholeTime( );
        double speed = curSumLoop / wholeTime;
        double needTime = (maxSumLoop - curSumLoop) / speed;
        double sumTime = wholeTime + needTime;
        printf("\rINFO: %.2f%%, %.2f loop / s, use %.2f s, need %.2f s, sum %.2f s     ",
            curStep, speed, wholeTime, needTime, sumTime);
        if (curSumLoop == maxSumLoop)
            imgUpdate->SetIsHold(false);
    }));
	imgWindow.Run(imgUpdate);
	return 0;
}
