#ifndef SCENERUNNER_H
#define SCENERUNNER_H

#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

#include "cookbookogl.h"
#include "scene.h"
#include "glutils.h"

class SceneRunner
{
public:
    SceneRunner(const std::string& windowTitle, std::unique_ptr<Scene> scene, int samples = 0)
    {
        m_scene = std::move(scene);
#if !_DEBUG
        debug = false;
#endif
        // Initialize GLFW
        if (!glfwInit()) exit(EXIT_FAILURE);
#ifdef __APPLE__
        // Select OpenGL 4.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
        // Select OpenGL 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        if (debug)
        {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        }
        if (samples > 0)
        {
            glfwWindowHint(GLFW_SAMPLES, samples);
        }
        // Open the window
        m_window = glfwCreateWindow(m_scene->width(), m_scene->height(), windowTitle.c_str(), NULL, NULL);
        if (!m_window)
        {
            std::cerr << "Unable to create OpenGL context." << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(m_window);
        // Load the OpenGL functions.
        if (!gladLoadGL())
        {
            exit(-1);
        }
        GLUtils::dumpGLInfo();

#ifndef __APPLE__
        if (debug)
        {
            glDebugMessageCallback(GLUtils::debugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
                GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
        }
#endif
    }
    int run()
    {
        // Enter the main loop
        mainLoop(m_window);

#ifndef __APPLE__
        if (debug)
        {
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1,
                GL_DEBUG_SEVERITY_NOTIFICATION, -1, "End debug");
        }
#endif

        // Close window and terminate GLFW
        glfwTerminate();

        // Exit program
        return EXIT_SUCCESS;
    }

    static std::string parseCLArgs(int argc, char** argv, std::map<std::string, std::string>& sceneData)
    {
        if (argc < 2) {
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        std::string recipeName = argv[1];
        auto it = sceneData.find(recipeName);
        if (it == sceneData.end())
        {
            printf("Unknown recipe: %s\n\n", recipeName.c_str());
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        return recipeName;
    }

private:
    static void printHelpInfo(const char* exeFile, std::map<std::string, std::string>& sceneData)
    {
        printf("Usage: %s recipe-name\n\n", exeFile);
        printf("Recipe names: \n");
        for (auto it : sceneData)
        {
            printf("  %11s : %s\n", it.first.c_str(), it.second.c_str());
        }
    }

    void mainLoop(GLFWwindow* window)
    {
        m_scene->initScene();

        while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
#if !_DEBUG
            //GLUtils::checkForOpenGLError(__FILE__,__LINE__);
#endif
            m_scene->update(float(glfwGetTime()));
            m_scene->render();
            glfwSwapBuffers(window);

            glfwPollEvents();
            int state = glfwGetKey(window, GLFW_KEY_SPACE);
            if (state == GLFW_PRESS)
                m_scene->animate(!m_scene->animating());
        }
    }
private:
    GLFWwindow* m_window = nullptr;
    std::unique_ptr<Scene> m_scene;
    bool debug = true;           // Set true to enable debug messages
};
#endif