#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <CppUtil/Basic/HeapObj.h>
#include <CppUtil/RTX/ImgWindow.h>
#include <CppUtil/Basic/Image.h>
#include <CppUtil/Basic/LambdaOp.h>

#include <glm/glm.hpp>

typedef float RayPrecision;
typedef glm::vec2 RayVec2;
typedef glm::vec3 RayVec3;

namespace Define {
	const std::string str_Chapter = "01_in_a_Weekend";
	const std::string str_Subchapter = "04_Ray";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;
}

#endif // !_DEFINES_H_


