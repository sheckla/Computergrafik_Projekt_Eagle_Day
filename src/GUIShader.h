
#ifndef GUIShader_h
#define GUIShader_h

#include <stdio.h>
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <iostream>
#include <assert.h>
#include "color.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"
#include "baseshader.h"
#include "texture.h"
#include "ShaderLightMapper.h"
#include "Texture.h"

class GUIShader : public BaseShader
{
public:
    GUIShader();
    ~GUIShader();
    virtual void activate(const BaseCamera& Cam) const;
    void texture(Texture* tex);
    void color(Color c);
private:
    GLint EyePosLoc;
    GLint ModelMatLoc;
    GLint ModelViewProjMatLoc;
    GLint TexLoc;
    GLint ColorLoc;

    Texture* pTex;
    Color Col;
};

#endif
