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
        glBufferData(GL_SHADER_STORAGE_BUFFER, renderBuff.size() * sizeof(GLfloat), &renderBuff[0], GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, pixelVBO);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    virtual void renderEx()
    {
        updatebuff(*this);
        glBindBuffer(GL_ARRAY_BUFFER, pixelVBO);
        void* buf = glMapBufferRange(GL_ARRAY_BUFFER, 0, renderBuff.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        memcpy(buf, &renderBuff[0], renderBuff.size() * sizeof(GLfloat));
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    virtual void compileAndLinkShaderEx() {}
    std::vector<GLfloat> renderBuff;
private:
    GLSLProgram computeProg;
    GLuint pixelVBO;
};

int main()
{
    SceneRunner runner(Define::str_WindowTitle);
    return runner.run(std::unique_ptr<Scene>(new Canvas()));
}
#endif