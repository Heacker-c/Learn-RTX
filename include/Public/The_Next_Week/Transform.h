#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_ 

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include "Defines.h"
#include "Ray.h"
#include "Hitable.h"
#include "AABB.h"

namespace RTX
{
	class Translate : public Hitable
	{
	public:
		Translate(Hitable::CPtr object, const RayVec3& displacement)
			: object(object), offset(displacement) {}
		virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
		{

			if (!object->boundingBox(t0, t1, output_box))
				return false;

			output_box = AABB( output_box.min() + offset, output_box.max() + offset);
			return true;
		}

		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
		{
			CppUtil::Basic::Ptr<Ray> newRay = ToPtr(new Ray(ray->origin() - offset, ray->direction(), ray->time()));
			if (!object->hit(newRay, t_min, t_max, rec))
				return false;

			rec.p += offset;
			rec.set_face_normal(newRay, rec.normal);

			return true;
		}
	public:
		Hitable::CPtr object;
		RayVec3 offset;
	};

	class RotateY : public Hitable
	{
	public:
		RotateY(Hitable::CPtr object, RayPrecision angle) :object(object)
		{
			auto radians = Define::degrees_to_radians(angle);
			sin_theta = sin(radians);
			cos_theta = cos(radians);
			hasbox = object->boundingBox(0, 1, bbox);

			RayVec3 min(Define::infinity);
			RayVec3 max(-Define::infinity);

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					for (int k = 0; k < 2; k++) {
						auto x = i * bbox.max().x + (1 - i) * bbox.min().x;
						auto y = j * bbox.max().y + (1 - j) * bbox.min().y;
						auto z = k * bbox.max().z + (1 - k) * bbox.min().z;

						auto newx = cos_theta * x + sin_theta * z;
						auto newz = -sin_theta * x + cos_theta * z;

						RayVec3 tester(newx, y, newz);

						for (int c = 0; c < 3; ++c) {
							min[c] = fmin(min[c], tester[c]);
							max[c] = fmax(max[c], tester[c]);
						}
					}
				}
			}
			bbox = AABB(min, max);
		}

		virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
		{
			output_box = bbox;
			return hasbox;
		}

		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
		{
			auto origin = ray->origin();
			auto direction = ray->direction();

			origin[0] = cos_theta * ray->origin()[0] - sin_theta * ray->origin()[2];
			origin[2] = sin_theta * ray->origin()[0] + cos_theta * ray->origin()[2];

			direction[0] = cos_theta * ray->direction()[0] - sin_theta * ray->direction()[2];
			direction[2] = sin_theta * ray->direction()[0] + cos_theta * ray->direction()[2];

			CppUtil::Basic::Ptr<Ray> newRay = ToPtr(new Ray(origin, direction, ray->time()));

			if (!object->hit(newRay, t_min, t_max, rec))
				return false;

			auto p = rec.p;
			auto normal = rec.normal;

			p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
			p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

			normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
			normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

			rec.p = p;
			rec.set_face_normal(newRay, normal);

			return true;
		}
	public:
		Hitable::CPtr object;
		RayPrecision sin_theta;
		RayPrecision cos_theta;
		bool hasbox;
		AABB bbox;
	};
}
#endif