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

set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/AABB.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/BVHList.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Camera.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Defines.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Dielectrics.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Hitable.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/HitableList.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Lambertian.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/LambertianNoise.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Material.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Metal.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/MoveCamera.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/MoveSphere.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Ray.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/RayColor.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Sphere.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Texture.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/TextureNoise.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Perlin.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/DiffuseLight.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Rectangle.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Box.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/ConstantMedium.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Transform.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/Isotropic.h")
set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${CMAKE_SOURCE_DIR}/include/Public/The_Next_Week/GlobalMain.h")

#多个库文件用 [空格] 分隔，如果为空，就输入[一个空格]
#如：set(STR_TARGET_LIBS "lib1.lib lib2.lib")
set(STR_TARGET_LIBS "Math HeapObj Image Config")

SETUP_PROJECT(${MODE} ${TARGET_NAME} ${STR_TARGET_SOURCES} ${STR_TARGET_LIBS})