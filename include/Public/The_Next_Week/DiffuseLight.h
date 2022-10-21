#ifndef _DIFFUSELIGHT_H_
#define _DIFFUSELIGHT_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"
#include "Texture.h"

namespace RTX
{
    class DiffuseLight : public Material
    {
    public:
        DiffuseLight(const RayVec3& a) : albedo(ToPtr(new SolidColor(a))) {}
        DiffuseLight(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}
        virtual RayVec3 emitted(const RayVec2& uv, const RayVec3& p) const override { return albedo->value(uv, p); }
    protected:
        CppUtil::Basic::Ptr<Texture> albedo;
    };
}

#endif // !_DIFFUSELIGHT_H_
