#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <glm/glm.hpp>

#include <string>
#include <random>

typedef glm::vec2 RayVec2;
typedef glm::vec3 RayVec3;

namespace Define
{
    const std::string chapter = "04_GPU_RayTracing";
    const std::string subchapter = "09_Final_Render";
    const std::string windowTitle = chapter + "_" + subchapter;
    const std::string shaderPrefix = "/data/shaders/" + chapter + "/" + subchapter + "/";

    inline float random()
    {
        static std::uniform_real_distribution<float> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return (float)distribution(generator);
    }

    inline float random(float min, float max)
    {
        return min + (max - min) * random();
    }

    inline RayVec3 randomVec3()
    {
        return RayVec3(random(), random(), random());
    }

    inline RayVec3 randomVec3(float min, float max)
    {
        return RayVec3(random(min, max), random(min, max), random(min, max));
    }

}

#endif // !_DEFINES_H_
