#ifndef _CONSTANTMEDIUM_H
#define _CONSTANTMEDIUM_H

#include "Material.h"
#include "Isotropic.h"
#include "Defines.h"

namespace RTX 
{
	class ConstantMedium : public Hitable 
	{
	public:
		ConstantMedium(Hitable::CPtr object, float d, CppUtil::Basic::Ptr<Texture> albedo)
			: boundary(object), neg_inv_density(-1.0f / d),phase_function(ToPtr(new Isotropic(albedo))) {}
		ConstantMedium(Hitable::CPtr object, float d, const RayVec3& color)
			: boundary(object), neg_inv_density(-1.0f / d), phase_function(ToPtr(new Isotropic(color))) {}

		virtual bool boundingBox(float t0, float t1, AABB& output_box) const override
		{
			return boundary->boundingBox(t0, t1, output_box);
		}

		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, HitRecord& rec) const override
		{
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
			RayPrecision hit_distance = neg_inv_density * log(Define::random());

			if (hit_distance > distance_inside_boundary)
				return false;

			rec.t = rec1.t + hit_distance / ray_length;
			rec.p = ray->at(rec.t);
			rec.normal = RayVec3(1.0f, 0.0f, 0.0f);	// arbitrary
			rec.front_face = true;					// also arbitrary
			rec.material = phase_function;

			return true;
		}
	public:

		Hitable::CPtr boundary;
		CppUtil::Basic::Ptr<Material> phase_function;
		float neg_inv_density;
	};
}
#endif