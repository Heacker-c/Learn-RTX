#include "trancing.h"
#include "Defines.h"
#include <vector>

void Trancing::initBuffers()
{
    std::vector<GLfloat> buff(width() * height() * 4, 0.0f);
    glGenBuffers(1, &pixelVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pixelVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, buff.size() * sizeof(GLfloat), &buff[0], GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, pixelVBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    maxFrame(99999);
}

void Trancing::renderEx()
{
    computeProg.use();
    computeProg.setUniform("viewSize", glm::vec2(width(), height()));
    computeProg.setUniform("currentFrame", currentFrame());
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
    SceneRunner runner(Define::windowTitle);
    return runner.run(std::unique_ptr<Scene>(new Trancing()));
}