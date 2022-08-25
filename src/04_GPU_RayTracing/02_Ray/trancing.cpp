#include "trancing.h"
#include "Defines.h"
#include <vector>

void Trancing::initBuffers()
{
    std::vector<GLfloat> buff(width() * height() * 4, 0.0f);
    glGenBuffers(1, &pixelVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pixelVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, buff.size() * sizeof(GLfloat), &buff[0], GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Trancing::renderEx()
{
    // Camera
    const auto aspect_ratio = 1.0f * width() / height();
    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    auto origin = RayVec3(0.0f, 0.0f, 0.0f);
    auto horizontal = RayVec3(viewport_width, 0.0f, 0.0f);
    auto vertical = RayVec3(0, viewport_height, 0.0f);
    auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - RayVec3(0.0f, 0.0f, focal_length);

    computeProg.use();
    computeProg.setUniform("viewSize", glm::vec2(width(), height()));
    computeProg.setUniform("viewSizeInv", 1.0f / glm::vec2(width(), height()));
    computeProg.setUniform("origin", origin);
    computeProg.setUniform("horizontal", horizontal);
    computeProg.setUniform("vertical", vertical);
    computeProg.setUniform("lower_left_corner", lower_left_corner);
    glDispatchCompute((width() + 7) >> 3, (height() + 7) >> 3, 1);
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