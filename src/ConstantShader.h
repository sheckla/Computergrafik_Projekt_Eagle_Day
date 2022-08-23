/*
 * ConstantShader
 * - einfacher Shader mit statische Farbe
 * - unterstuetzt nun DiffuseTexture und Texcoord
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */
#ifndef ConstantShader_hpp
#define ConstantShader_hpp

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <assert.h>
#include "color.h"
#include "camera.h"
#include "baseshader.h"


class ConstantShader : public BaseShader
{
public:
    ConstantShader();
    void color(const Color& c);
    const Color& color() const { return Col; }
    virtual void activate(const BaseCamera& Cam) const override;
private:
    Color Col;
    GLuint ShaderProgram;
    GLint ColorLoc;
    GLint ModelViewProjLoc;
};

#endif 
