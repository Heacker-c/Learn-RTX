#include "Defines.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;

int main(int argc, char** argv)
{
    ImgWindow imgWindow(str_WindowTitle);
    if (!imgWindow.IsValid())
    {
        printf("ERROR: Image Window Create Fail.\n");
        return 1;
    }

    Image& img = imgWindow.GetImg();
    const int val_ImgWidth = img.GetWidth();
    const int val_ImgHeight = img.GetHeight();
    const int val_ImgChannel = img.GetChannel();

    auto imgUpdate = ToPtr(new LambdaOp([&]()
        {
            static int f = 0;
#pragma omp parallel for
            for (int i = 0; i < img.GetWidth(); i++)
            {
                for (int j = 0; j < img.GetHeight(); j++)
                {
                    RayPrecision r = 0.5f * i / (RayPrecision)img.GetWidth();
                    RayPrecision g = 0.5f * j / (RayPrecision)img.GetHeight();
                    RayPrecision b = 0.2f + 0.2 * sinf(0.05f * f);
                    img.SetPixel(img.GetWidth() - 1 - i, j, Image::Pixel<RayPrecision>(r, g, b));
                }
            }
            ++f;
        }, true));

    imgWindow.Run(imgUpdate);
    return 0;
}

