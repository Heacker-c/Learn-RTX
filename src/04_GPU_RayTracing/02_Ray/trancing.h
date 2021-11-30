#ifndef TRANCING_H
#define TRANCING_H

#include "Public/Ingredients/scene.h"
#include "Public/Ingredients/scenerunner.h"

class Trancing : public Scene
{
public:
    virtual void renderEx();
    virtual void initBuffers();
    virtual void compileAndLinkShaderEx();
private:
    GLSLProgram computeProg;
    GLuint pixelVBO;
};
#endif //TRANCING_H
