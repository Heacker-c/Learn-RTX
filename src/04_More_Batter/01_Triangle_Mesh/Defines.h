#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <Public/Ingredients/defines.h>
#include <Public/More_Batter/AABB.h>
#include <Public/More_Batter/BVHList.h>
#include <Public/More_Batter/Camera.h>
#include <Public/More_Batter/HitableList.h>
#include <Public/More_Batter/Texture.h>
#include <Public/More_Batter/MoveCamera.h>
#include <Public/More_Batter/MoveSphere.h>
#include <Public/More_Batter/Sphere.h>
#include <Public/More_Batter/Perlin.h>
#include <Public/More_Batter/Texture.h>
#include <Public/More_Batter/Rectangle.h>
#include <Public/More_Batter/Box.h>
#include <Public/More_Batter/Transform.h>
#include <Public/More_Batter/ONB.h>
#include <Public/More_Batter/FlipFace.h>

namespace Define
{
	const std::string str_Chapter = "04_More_Batter";
	const std::string str_Subchapter = "01_Triangle_Mesh";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;
}
#endif // !_DEFINES_H_
