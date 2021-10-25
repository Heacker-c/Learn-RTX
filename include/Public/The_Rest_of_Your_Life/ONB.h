#ifndef _ONB_H
#define _ONB_H

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>

namespace RTX
{
	class ONB
	{
	public:
		ONB() {}
		inline RayVec3 operator[](int i) const { return axis[i]; }
		RayVec3 u() const { return axis[0]; }
		RayVec3 v() const { return axis[1]; }
		RayVec3 w() const { return axis[2]; }
		RayVec3 local(float a, float b, float c) const{ return a * u() + b * v() + c * w(); }
		RayVec3 local(const RayVec3& a) const { return a.x * u() + a.y * v() + a.z * w(); }
		void build_from_w(const RayVec3& n)
		{
			axis[2] = normalize(n);
			RayVec3 a = (fabs(w().x) > 0.9f) ? RayVec3(0.0f, 1.0f, 0.0f) : RayVec3(1.0f, 0.0f, 0.0f);
			axis[1] = normalize(cross(w(), a));
			axis[0] = cross(w(), v());
		}
	public:
		RayVec3 axis[3];
	};
}
#endif //!_ONB_H