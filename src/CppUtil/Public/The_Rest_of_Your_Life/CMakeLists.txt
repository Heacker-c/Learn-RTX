#项目名，默认为目录名
GET_DIR_NAME(DIRNAME)
set(TARGET_NAME ${DIRNAME})
#多个源文件用 [空格] 分隔
#如：set(STR_TARGET_SOURCES "main.cpp src_2.cpp")
file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
)
set(STR_TARGET_SOURCES "")
foreach(SOURCE ${ALL_SOURCES})
	set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${SOURCE}")
endforeach(SOURCE ${ALL_SOURCES})

set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/AABB.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/BVHList.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Camera.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Defines.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Dielectrics.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Hitable.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/HitableList.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Lambertian.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/LambertianNoise.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Material.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Metal.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/MoveCamera.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/MoveSphere.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Ray.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/RayColor.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Sphere.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Texture.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/TextureNoise.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Perlin.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/DiffuseLight.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Rectangle.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Box.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/ConstantMedium.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Transform.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/Isotropic.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/GlobalMain.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/ONB.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Rest_of_Your_Life/FlipFace.h")

#多个库文件用 [空格] 分隔，如果为空，就输入[一个空格]
#如：set(STR_TARGET_LIBS "lib1.lib lib2.lib")
set(STR_TARGET_LIBS "Math HeapObj Image Config")

SETUP_PROJECT(${MODE} ${TARGET_NAME} ${STR_TARGET_SOURCES} ${STR_TARGET_LIBS})
