#include "trancing.h"
#include "Defines.h"
#include <vector>

#pragma pack(4)
using namespace Define;

struct Sphere
{
    RayVec3 center = RayVec3(0.0f);
    float radius = 1.0f;
    RayVec3 empty = RayVec3(0.0f);
    int materialIndex = 1;
};

struct Material
{
    RayVec3 albedo = RayVec3(0.0f);
    int materialType = 0;
    RayVec2 empty = RayVec2(0.0f);
    float refractScale = 0.0f;
    float fuzz = 0.0f;
};

std::vector<Sphere> world;
void initWorld(std::vector<Sphere>& world)
{
    world.resize(5);
    world[0].radius = 0.5f;
    world[0].center = RayVec3(-1.1f, 0.0f, -1.0f);
    world[0].materialIndex = 1;

    world[4].radius = -0.495f;
    world[4].center = RayVec3(0.0f, 0.0f, -1.0f);
    world[4].materialIndex = 2;

    world[1].radius = 0.5f;
    world[1].center = RayVec3(0.0f, 0.0f, -1.0f);
    world[1].materialIndex = 2;

    world[2].radius = 0.5f;
    world[2].center = RayVec3(1.1f, 0.0f, -1.0f);
    world[2].materialIndex = 3;

    world[3].radius = 100.0f;
    world[3].center = RayVec3(0.0f, -100.5f, -1.0f);
    world[3].materialIndex = 4;
}

std::vector<Material> material;
void initMaterial(std::vector<Material>& material)
{
    //第一个定义为空白材质进行占位
    material.resize(4 + 1);

    material[0].materialType = 0;
    material[0].albedo = RayVec3(1.0f, 0.0f, 1.0f);

    material[1].materialType = 2;
    material[1].albedo = RayVec3(0.98f, 0.839f, 0.35f);
    material[1].fuzz = 0.05f;

    material[2].materialType = 3;
    material[2].albedo = RayVec3(1.0f);
    material[2].refractScale = 1.42f;
	material[1].fuzz = 0.05f;

    material[3].materialType = 2;
    material[3].albedo = RayVec3(0.8f);
    material[3].fuzz = 0.25f;

    material[4].materialType = 1;
    material[4].albedo = RayVec3(0.0f, 0.8f, 0.8f);
}
void Trancing::initBuffers()
{
    //创建画布
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

    initMaterial(material);
    glGenBuffers(1, &materialVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, materialVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, material.size() * sizeof(Material), &material[0], GL_DYNAMIC_COPY);
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
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
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