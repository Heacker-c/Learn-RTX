#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "defines.h"
#include "scene.h"
#include "scenerunner.h"
#include <iostream>

class Canvas;
void updatebuff(Canvas& canvas);

class Canvas : public Scene
{
public:
    virtual void initBuffers()
    {
        renderBuff.resize(m_width * m_height * 4, 0.0f);
        glGenBuffers(1, &pixelVBO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pixelVBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, renderBuff.size() * sizeof(GLfloat), &renderBuff[0], GL_STREAM_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    virtual void renderEx()
    {
        updatebuff(*this);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, pixelVBO);
        void* buf = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, renderBuff.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
        memcpy(buf, &renderBuff[0], renderBuff.size() * sizeof(GLfloat));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    virtual void compileAndLinkShaderEx() {}
    std::vector<GLfloat> renderBuff;
private:
    GLuint pixelVBO = 0;
    GLSLProgram computeProg;
};

int main()
{
    SceneRunner runner(Define::str_WindowTitle, std::unique_ptr<Scene>(new Canvas()));
    return runner.run();
}
#endif