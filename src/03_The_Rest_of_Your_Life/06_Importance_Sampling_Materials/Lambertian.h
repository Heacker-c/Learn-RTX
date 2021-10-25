#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"
#include "Defines.h"

namespace RTX
{
    class Lambertian : public Material
    {
    public:
        bool near_zero(const RayVec3& e) const 
		{
            return (fabs(e[0]) < FLT_EPSILON) && (fabs(e[1]) < FLT_EPSILON) && (fabs(e[2]) < FLT_EPSILON);
        }
        Lambertian(const RayVec3& a) : albedo(ToPtr(new SolidColor(a))) {}
        Lambertian(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}
        virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, RayVec3& attenuation, CppUtil::Basic::Ptr<Ray>& scattered, RayPrecision& pdf) const override
        {
#if 0
            RayVec3 direction = rec.normal + Define::random_unit_vector();
            scattered = ToPtr(new Ray(rec.p, normalize(direction), ray->time()));
            attenuation = albedo->value(rec.uv, rec.p);
            pdf = dot(rec.normal, scattered->direction()) / pi;
#else
            RayVec3 direction = random_in_hemisphere(rec.normal);
            scattered = ToPtr(new Ray(rec.p, normalize(direction), ray->time()));
            attenuation = albedo->value(rec.uv, rec.p);
            pdf = 0.5f / pi;
#endif
            return true;
        }
        virtual float scattering_pdf(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, CppUtil::Basic::Ptr<Ray>& scattered) const 
        {
            auto cosine = dot(rec.normal, normalize(scattered->direction()));
            return cosine < 0 ? 0 : cosine / pi;
        }
    protected:
        CppUtil::Basic::Ptr <Texture> albedo;
    };
}

#endif // !_LAMBERTIAN_H_
