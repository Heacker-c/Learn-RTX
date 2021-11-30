#include "Defines.h"

void updatebuff(Canvas& canvas)
{
    canvas.maxFrame(99999);
    static int f = 0;
#pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < canvas.height(); i++)
    {
        for (int j = 0; j < canvas.width(); j++)
        {
            auto ptr = &canvas.renderBuff[(i * canvas.width() + j) * 4];
            ptr[0] = 0.5f * i / (RayPrecision)canvas.width();
            ptr[1] = 0.5f * j / (RayPrecision)canvas.height();
            ptr[2] = 0.5f + 0.4 * sinf(0.05f * f);
        }
    }
    ++f;
}