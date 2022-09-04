/*
 * TextureShader
 * - simpler Shader der zusaetzlich Texturen erlaubt
 */
#ifndef TextureShader_h
#define TextureShader_h

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "camera.h"
#include "baseshader.h"
#include "Texture.h"

class TextureShader : public BaseShader
{
public:
    TextureShader();
    virtual ~TextureShader();
    void activate(const BaseCamera& Cam) const override;
    void diffuseTexture(const Texture* pTex);
private:
    GLuint ShaderProgram;
    GLint ModelViewProjLoc;
    GLint DiffuseTextureLoc;
    const Texture* DiffuseTexture;

    mutable bool diffTexChanged = false;
};

#endif
