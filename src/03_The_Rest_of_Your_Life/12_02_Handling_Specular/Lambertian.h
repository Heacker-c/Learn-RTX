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
        virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec, scatter_record& srec) const override
        {
            srec.is_specular = false;
            srec.attenuation = albedo->value(rec.uv, rec.p);
            srec.pdf_ptr = ToPtr(new CosinePDF(rec.normal));
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
