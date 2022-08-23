/*
 * BaseShader
 * - dient als BasisShaderEinheit fuer ihre Ableitungen
 * - lieftert wichtige Initialisierungsfunktionen fuer die Shader in OpenGL
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef BaseShader_h
#define BaseShader_h

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "color.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"

class BaseShader
{
public:
    BaseShader();
    virtual ~BaseShader() {}
    virtual void modelTransform(const Matrix& m) { ModelTransform = m; }
    virtual const Matrix& modelTransform() const { return ModelTransform; }

    virtual void activate(const BaseCamera& Cam) const;
    virtual void activate() const;
    virtual void deactivate() const;

    bool load(const char* VertexShaderFile, const char* FragmentShaderFile);
    GLint initUniformParameter(const char* ParamenterName) const;
    GLuint getBlockID(const char* BlockName) const;

    void setBlock(GLuint ID, GLuint UniformBufferID) const;
    void setUniformParameter(GLint ID, float Param) const;
    void setUniformParameter(GLint ID, int Param) const;
    void setUniformParameter(GLint ID, const Vector& Param) const;
    void setUniformParameter(GLint ID, const Color& Param) const;
    void setUniformParameter(GLint ID, const Matrix& Param) const;

    GLuint openGLProgramID() { return ShaderProgram; }
protected:
    char* loadFile(const char* File, unsigned int& Filesize);
    GLuint createShaderProgram(const char* VScode, const char* FScode);
    Matrix ModelTransform;
    GLuint ShaderProgram;
    GLuint LightUniformBuffer;

    static const BaseShader* ShaderInPipe;
};

#endif
