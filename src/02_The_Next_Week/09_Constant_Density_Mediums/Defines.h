#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <Public/The_Next_Week/AABB.h>
#include <Public/The_Next_Week/BVHList.h>
#include <Public/The_Next_Week/Camera.h>
#include <Public/The_Next_Week/Dielectrics.h>
#include <Public/The_Next_Week/HitableList.h>
#include <Public/The_Next_Week/Texture.h>
#include <Public/The_Next_Week/Lambertian.h>
#include <Public/The_Next_Week/Metal.h>
#include <Public/The_Next_Week/MoveCamera.h>
#include <Public/The_Next_Week/MoveSphere.h>
#include <Public/The_Next_Week/Sphere.h>
#include <Public/The_Next_Week/Perlin.h>
#include <Public/The_Next_Week/Texture.h>
#include <Public/The_Next_Week/DiffuseLight.h>
#include <Public/The_Next_Week/Rectangle.h>
#include <Public/The_Next_Week/Box.h>
#include <Public/The_Next_Week/Transform.h>
#include <Public/The_Next_Week/Isotropic.h>
#include <Public/The_Next_Week/ConstantMedium.h>

namespace Define 
{
    const std::string str_Chapter = "02_the_Next_Week";
	const std::string str_Subchapter = "09_Constant_Density_Mediums";
    const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;
}

#endif // !_DEFINES_H_
