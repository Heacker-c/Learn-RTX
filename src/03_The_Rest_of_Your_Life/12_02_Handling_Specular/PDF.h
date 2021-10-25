#ifndef _PDF_H_
#define _PDF_H_

#include "Defines.h"

namespace RTX 
{
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

	class MixturePDF : public PDF 
	{
	public:
		MixturePDF(CppUtil::Basic::Ptr<PDF> p0, CppUtil::Basic::Ptr<PDF> p1) 
		{
			p[0] = p0;
			p[1] = p1;
		}
		virtual float value(const RayVec3& direction) const override { return 0.5f * (p[0]->value(direction) + p[1]->value(direction)); }
		virtual RayVec3 generate() const override { return random() < 0.5f ? p[0]->generate() : p[1]->generate(); }
	public:
		CppUtil::Basic::Ptr<PDF> p[2];
	};
}
#endif //!_PDF_H_