
#ifndef ParticleShader_h
#define ParticleShader_h

#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "camera.h"
#include "baseshader.h"
#include "Texture.h"


class ParticleShader : public BaseShader
{
public:
    ParticleShader();
    virtual ~ParticleShader();
    void activate(const BaseCamera& Cam) const override;
    void diffuseTexture(const Texture* pTex);

    void setTransparency(float transparency) { this->transparency_value = transparency; }
private:
    GLuint ShaderProgram;
    GLint ModelViewProjLoc;
    GLint DiffuseTextureLoc;
    GLint Transparency;
    const Texture* DiffuseTexture;

    float transparency_value = 1.0f;

    mutable bool diffTexChanged = false;
};

#endif
