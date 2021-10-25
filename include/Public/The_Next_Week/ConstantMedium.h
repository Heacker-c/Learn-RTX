#ifndef _CONSTANTMEDIUM_H
#define _CONSTANTMEDIUM_H

#include "Hitable.h"
#include "Material.h"
#include "Texture.h"
#include "Isotropic.h"
#include "Ray.h"
#include "Defines.h"
#include <vector>

namespace RTX 
{
	using namespace Define;
	class ConstantMedium : public Hitable 
	{
	public:
		ConstantMedium(Hitable::CPtr object, RayPrecision d, CppUtil::Basic::Ptr<Texture> albedo)
			: boundary(object), neg_inv_density(-1.0f / d),phase_function(ToPtr(new Isotropic(albedo))) {}
		ConstantMedium(Hitable::CPtr object, RayPrecision d, const RayVec3& color)
			: boundary(object), neg_inv_density(-1.0f / d), phase_function(ToPtr(new Isotropic(color))) {}

		virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override{
			return boundary->boundingBox(t0, t1, output_box);
		}

		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override{
			HitRecord rec1, rec2;

			if (!boundary->hit(ray, -infinity, infinity, rec1))
				return false;

			if (!boundary->hit(ray, rec1.t + 0.0001f, infinity, rec2))
				return false;

			if (rec1.t < t_min) rec1.t = t_min;
			if (rec2.t > t_max) rec2.t = t_max;

			if (rec1.t >= rec2.t) return false;

			if (rec1.t < 0) rec1.t = 0;

			const auto ray_length = ray->direction().length();
			const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
			const auto hit_distance = neg_inv_density * log(random());

			if (hit_distance > distance_inside_boundary)
				return false;

			rec.t = rec1.t + hit_distance / ray_length;
			rec.p = ray->at(rec.t);

			rec.normal = RayVec3(1.0f, 0.0f, 0.0f);
			rec.front_face = true;
			rec.material = phase_function;

			return true;
		}
	public:

		Hitable::CPtr boundary;
		CppUtil::Basic::Ptr<Material> phase_function;
		RayPrecision neg_inv_density;
	};
}
#endif