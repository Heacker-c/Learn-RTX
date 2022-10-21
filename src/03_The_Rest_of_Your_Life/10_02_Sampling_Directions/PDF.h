#ifndef _PDF_H_
#define _PDF_H_

#include "Defines.h"

namespace RTX 
{
	using namespace glm;
	using namespace Define;
	class PDF : public CppUtil::Basic::HeapObj  
	{
		HEAP_OBJ_SETUP(PDF)
	public:
		virtual float value(const RayVec3& direction) const = 0;
		virtual RayVec3 generate() const = 0;
	};

	class CosinePDF : public PDF 
	{
	public:
		CosinePDF(const RayVec3& w) { uvw.build_from_w(w); }
		virtual float value(const RayVec3& direction) const override 
		{
			auto cosine = dot(normalize(direction), uvw.w());
			return (cosine <= 0) ? 0 : cosine / pi;
		}
		virtual RayVec3 generate() const override { return uvw.local(random_cosine_direction()); }
	public:
		ONB uvw;
	};

	class HittablePDF : public PDF 
	{
	public:
		HittablePDF(CppUtil::Basic::Ptr<Hitable> p, const RayVec3& origin) : ptr(p), origin(origin) {}
		virtual float value(const RayVec3& direction) const override { return ptr->pdfValue(origin, direction); }
		virtual RayVec3 generate() const override { return ptr->random(origin); }
	public:
		RayVec3 origin;
		CppUtil::Basic::Ptr<Hitable> ptr;
	};
}
#endif //!_PDF_H_