#include "trancing.h"
#include "Defines.h"
#include <vector>

#pragma pack(4)
using namespace Define;
struct Sphere
{
    RayVec3 center = RayVec3(0.0f);
    float radius = 1.0f;
};

void initWorld(std::vector<Sphere>& world)
{
#if 0
    world.resize(3);
    world[0].radius = 0.5f * random();
    world[0].center = 0.5f * (1.0f + randomVec3());
    world[0].center.z = -world[0].center.z;

    world[1].radius = 0.5f * random();
    world[1].center = 0.5f * (-1.0f + randomVec3());
    world[1].center.z = -1.5 + random();

    world[2].radius = 0.5 * random();
    world[2].center.z = -1.5 + random();
#else
    world.resize(2);
    world[0].radius = 0.5f;
    world[0].center.z = -1.0;

    world[1].radius = 100.0f;
    world[1].center = RayVec3(0.0f, -100.5f, -1.0f);
#endif
}

std::vector<Sphere> world;
void Trancing::initBuffers()
{
    //´´½¨»­²¼
    std::vector<GLfloat> buff(m_width * m_height * 4, 0.0f);
    glGenBuffers(1, &pixelVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pixelVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, buff.size() * sizeof(GLfloat), &buff[0], GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    initWorld(world);
    glGenBuffers(1, &shapeVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shapeVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, world.size() * sizeof(Sphere), &world[0], GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Trancing::renderEx()
{
    // Camera
    const auto aspect_ratio = 1.0f * m_width / m_height;
    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    auto origin = RayVec3(0.0f, 0.0f, 0.0f);
    auto horizontal = RayVec3(viewport_width, 0.0f, 0.0f);
    auto vertical = RayVec3(0, viewport_height, 0.0f);
    auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - RayVec3(0.0f, 0.0f, focal_length);

    computeProg.use();
    computeProg.setUniform("viewSize", glm::vec2(m_width, m_height));
    computeProg.setUniform("viewSizeInv", 1.0f / glm::vec2(m_width, m_height));
    computeProg.setUniform("origin", origin);
    computeProg.setUniform("horizontal", horizontal);
    computeProg.setUniform("vertical", vertical);
    computeProg.setUniform("lower_left_corner", lower_left_corner);
    computeProg.setUniform("shpereNum", (int)world.size());
    computeProg.setUniform("currentFrame", m_currentFrame);
    computeProg.setUniform("recursionDepth", m_maxDepth);
    glDispatchCompute((m_width + 7) >> 3, (m_height + 7) >> 3, 1);
}

void Trancing::compileAndLinkShaderEx()
{
    try
    {
        std::string path = ROOT_PATH + Define::shaderPrefix;
        computeProg.compileShader(path + "trancing.comp");
        computeProg.link();

    }
    catch (GLSLProgramException & e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main()
{
    SceneRunner runner(Define::windowTitle, std::unique_ptr<Scene>(new Trancing()));
    return runner.run();
}