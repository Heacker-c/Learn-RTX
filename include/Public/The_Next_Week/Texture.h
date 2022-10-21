#ifndef TEXTURE_H
#define TEXTURE_H

#include <CppUtil/Basic/HeapObj.h>
#include <CppUtil/Basic/Image.h>
#include "Perlin.h"

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
        SolidColor(RayPrecision red, RayPrecision green, RayPrecision blue) : SolidColor(RayVec3(red, green, blue)) {}
        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override { return color; }
    private:
        RayVec3 color;
    };

    class CheckerTexture : public Texture
    {
    public:
        CheckerTexture() {}
        CheckerTexture(CppUtil::Basic::Ptr<Texture> t0, CppUtil::Basic::Ptr<Texture> t1) : even(t0), odd(t1) {}
        CheckerTexture(RayVec3& c1, RayVec3& c2) : even(ToPtr(new SolidColor(c1))), odd(ToPtr(new SolidColor(c2))) {}

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

    class NoiseTexture : public Texture 
    {
    public:
        NoiseTexture(RayPrecision sc = 0.35f) : scale(sc) {}

        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override 
        {
            return RayVec3(1.0f) * 0.5f * (1.0f + sin(scale * p.z + 10.0f * noise.turb(p)));
        }
    public:
        RayPrecision scale;
        Perlin noise;
    };

    class ImageTexture : public Texture 
    {
    public:
        const static int bytes_per_pixel = 3;
        ImageTexture(const std::string& filename) 
        {
            img.Load(filename.c_str());
        }
        virtual RayVec3 value(const RayVec2& uv, const RayVec3& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (!img.IsValid())
                return RayVec3(0.0f, 1.0f, 1.0f);
            // Clamp input texture coordinates to [0,1] x [1,0]
            auto u = glm::clamp(uv.x, 0.0f, 1.0f);
            auto v = 1.0f - glm::clamp(uv.y, 0.0f, 1.0f);  // Flip V to image coordinates
            auto i = static_cast<int>(u * img.GetWidth());
            auto j = static_cast<int>(v * img.GetHeight());
            // Clamp integer mapping, since actual coordinates should be less than 1.0f
            if (i >= img.GetWidth())  i = img.GetWidth() - 1;
            if (j >= img.GetHeight()) j = img.GetHeight() - 1;
            const auto scale = 1.0f / 255.0f;
            //auto pixel = img.GetPixel_UB(); data + j * img.GetChannel() + i * bytes_per_pixel;
            auto pixel = img.GetPixel_UB(i, j);
            return RayVec3(scale * pixel[0], scale * pixel[1], scale * pixel[2]);
        }
    private:
        CppUtil::Basic::Image img;
    };
}
#endif