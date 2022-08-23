#include "TextureShader.h"

TextureShader::TextureShader() : BaseShader()
{
    unsigned int vs_i = 0;
    unsigned int fs_i = 0;
    char* vs = loadFile(SHADERS "texture/vsTexture.glsl", vs_i);
    char* fs = loadFile(SHADERS "texture/fsTexture.glsl", fs_i);

    if (!vs || !fs)
    {
        print("TextureShader()", "init failed", true);
        throw std::exception();
    }

    ShaderProgram = createShaderProgram(vs, fs);
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    DiffuseTextureLoc = glGetUniformLocation(ShaderProgram, "DiffuseTexture");
}
TextureShader::~TextureShader()
{
    delete DiffuseTexture;
}
void TextureShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);

    Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
    Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);

    DiffuseTexture->activate(0);
    if (diffTexChanged)
    {
		glUniform1i(DiffuseTextureLoc, 0);
    }

    diffTexChanged = false;
}
void TextureShader::diffuseTexture(const Texture* pTex)
{
		this->DiffuseTexture = pTex;
        this->diffTexChanged = true;
    // TODO Clear memory before assignment?
}
