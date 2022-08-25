#pragma once
#include <iostream>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include <ROOT_PATH.h>
#include <Public/Ingredients/timer.h>

#define EXPOTE(name, type)                  \
type name() { return m_##name; }            \
void name(type name) { m_##name = name; }

class Scene
{
public:
    Scene(int w = 800, int h = 640) :m_width(w), m_height(h) {}
    virtual ~Scene() {}
    void initScene()
    {
        compileAndLinkShader();
        initBuffers();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        GLfloat verts[] =
        {
            -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
        };
        // Set up the buffers
        GLuint handle;
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        // Set up the vertex array object
        glGenVertexArrays(1, &quad);
        glBindVertexArray(quad);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        timer.Start();
    }

    virtual void update(float t)
    {
        if (!m_needDraw)
            return;

        m_currentFrame += 1;

        if (m_currentFrame > m_maxFrame)
        {
            m_needDraw = false;
            return;
        }
        float curStep = m_currentFrame / (float)m_maxFrame * 100;
        float wholeTime = (float)timer.GetWholeTime();
        float speed = m_currentFrame / wholeTime;
        float needTime = (m_maxFrame - m_currentFrame) / speed;
        float sumTime = wholeTime + needTime;
        printf("\rINFO: %.2f%%, %.2f loop / s, use %.2f s, need %.2f s, sum %.2f s          ",
            curStep, speed, wholeTime, needTime, sumTime);
    }

    void render()
    {
        if (m_needDraw)
        {
            renderEx();
        }
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderProg.use();
		renderProg.setUniform("width", m_width);
		glBindVertexArray(quad);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        glBindVertexArray(0);
    }
    void animate(bool value) { m_animate = value; }
    bool animating() { return m_animate; }
    int width() { return m_width; }
    int height() { return m_height; }

    virtual void renderEx() = 0;
    virtual void initBuffers() = 0;
    virtual void compileAndLinkShaderEx() = 0;
    EXPOTE(currentFrame, int);
    EXPOTE(maxFrame, int);
    EXPOTE(maxDepth, int);
protected:
    void compileAndLinkShader()
    {
        compileAndLinkShaderEx();
        try
        {
            std::string vsPath = ROOT_PATH + std::string("/data/shaders/Common/");
            renderProg.compileShader(vsPath + "basic2T.vert");
            renderProg.compileShader(vsPath + "basic2T.frag");
            renderProg.link();
        }
        catch (GLSLProgramException & e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
protected:
    bool m_animate = true;
    bool m_needDraw = true;
    int m_currentFrame = -1;
    int m_maxFrame = 500;
    int m_maxDepth = 10;
    int m_width;
    int m_height;
    GLuint quad = 0;
    GLSLProgram renderProg;
    Timer timer;
};
