#ifndef TEXTURE_H
#define TEXTURE_H

#include <CppUtil/Basic/HeapObj.h>
#include <CppUtil/Basic/Image.h>

namespace RTX
{
    class Texture : public CppUtil::Basic::HeapObj
    {
        HEAP_OBJ_SETUP(Texture)
    public:
        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const = 0;
    };

    class SolidColor : public Texture
    {
    public:
        SolidColor() {}
        SolidColor(RayVec3 c) : color(c) {}

        SolidColor(RayPrecision red, RayPrecision green, RayPrecision blue)
            : SolidColor(RayVec3(red, green, blue)) {}

        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override
        {
            return color;
        }

    private:
        RayVec3 color;
    };

    class CheckerTexture : public Texture
    {
    public:
        CheckerTexture() {}
        CheckerTexture(CppUtil::Basic::Ptr<Texture> t0, CppUtil::Basic::Ptr<Texture> t1)
            : even(t0), odd(t1) {}
        CheckerTexture(RayVec3& c1, RayVec3& c2) : even(ToPtr(new SolidColor(c1))),
            odd(ToPtr(new SolidColor(c2))) {}

        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override
        {
            auto sines = sin(10.0f * p.x) * sin(10.0f * p.y) * sin(10.0f * p.z);
            if (sines < 0)
                return odd->value(uv, p);
            else
                return even->value(uv, p);
        }

    public:
        CppUtil::Basic::Ptr<Texture> odd;
        CppUtil::Basic::Ptr<Texture> even;
    };
}
#endif