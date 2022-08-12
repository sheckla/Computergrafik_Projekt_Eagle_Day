//
//  PhongShader.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef ProcessingShader_h
#define ProcessingShader_h

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

class PostProcessingShader : public BaseShader
{
public:
    PostProcessingShader();
    virtual void activate(const BaseCamera& Cam) const;
    void screenTexture(Texture* tex);
private:
    GLint EyePosLoc;
    GLint ModelMatLoc;
    GLint ModelViewProjMatLoc;
    GLint ScreenTextureLoc;

    Texture* ScreenTexture;
};

#endif
