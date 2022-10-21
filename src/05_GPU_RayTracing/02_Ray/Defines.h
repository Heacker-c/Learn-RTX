#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <string> 

typedef glm::vec2 RayVec2;
typedef glm::vec3 RayVec3;

namespace Define
{
    const std::string chapter = "04_GPU_RayTracing";
    const std::string subchapter = "02_Ray";
    const std::string windowTitle = chapter + "_" + subchapter;
    const std::string shaderPrefix = "/data/shaders/" + chapter + "/" + subchapter + "/";
}

#endif // !_DEFINES_H_
