#ifndef _DIFFUSELIGHT_H_
#define _DIFFUSELIGHT_H_

#include "Material.h"
#include "Defines.h"

namespace RTX 
{
	using namespace CppUtil::Basic;
	class DiffuseLight : public Material 
	{
	public:
		DiffuseLight(const RayVec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		DiffuseLight(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}
		virtual RayVec3 emitted(CppUtil::Basic::Ptr<Ray> ray, const HitRecord& rec,
			const RayVec2& uv, const RayVec3& p) const override 
		{
			if (rec.front_face)
				return albedo->value(uv, p);
			else
				return RayVec3(0, 0, 0);
		}
	protected:
		CppUtil::Basic::Ptr<Texture> albedo;
	};
}

#endif // !_DIFFUSELIGHT_H_
