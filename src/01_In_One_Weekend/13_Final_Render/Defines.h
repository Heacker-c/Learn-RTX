#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <Public/In_One_Weekend/Ray.h>
#include <Public/In_One_Weekend/Hitable.h>
#include <Public/In_One_Weekend/HitableList.h>
#include <Public/In_One_Weekend/Sphere.h>
#include <Public/In_One_Weekend/Lambertian.h>
#include <Public/In_One_Weekend/Metal.h>
#include <Public/In_One_Weekend/Dielectrics.h>
#include <Public/In_One_Weekend/RayColor.h>
#include <Public/In_One_Weekend/Camera.h>
#include <Public/In_One_Weekend/Scene.h>
#include <Public/In_One_Weekend/GlobalMain.h>

namespace Define 
{
	const std::string str_Chapter = "01_in_a_Weekend";
	const std::string str_Subchapter = "13_Final_Render";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;
}
#endif // !_DEFINES_H_
