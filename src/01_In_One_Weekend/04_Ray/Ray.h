#ifndef _RAY_H_
#define _RAY_H_

#include "Defines.h"

namespace RTX 
{
	class Ray : public CppUtil::Basic::HeapObj {
		HEAP_OBJ_SETUP(Ray)
	public:
		Ray(const RayVec3 & origin = RayVec3(0), const RayVec3 & dir = RayVec3(1)) : orig(origin), dir(dir) {}
		
		inline RayVec3 origin() const { return orig; }
		inline RayVec3 direction() const { return dir; }
		inline RayVec3 at(RayPrecision t) const { return orig + t * dir; }
	private:
		RayVec3 orig;
		RayVec3 dir;
	};
}
#endif //!_RAY_H_