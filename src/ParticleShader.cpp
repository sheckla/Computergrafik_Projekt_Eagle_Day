// 20.07 Leichte Veraenderung: unterstuetzt nun DiffuseTexture und Texcoord

#include "ParticleShader.h"

ParticleShader::ParticleShader() : BaseShader()
{
    unsigned int vs_i = 0;
    unsigned int fs_i = 0;
    char* vs = loadFile(SHADERS "sprite/vsSprite.glsl", vs_i);
    char* fs = loadFile(SHADERS "sprite/fsSprite.glsl", fs_i);

    if (!vs || !fs)
    {
        print("ParticleShader()", "init failed", true);
        throw std::exception();
    }

    ShaderProgram = createShaderProgram(vs, fs);
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    DiffuseTextureLoc = glGetUniformLocation(ShaderProgram, "DiffuseTexture");

    this->Transparency = glGetUniformLocation(ShaderProgram, "transparency_value");
}
ParticleShader::~ParticleShader()
{
    delete DiffuseTexture;
}
void ParticleShader::activate(const BaseCamera& Cam) const
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

    setUniformParameter(this->Transparency, this->transparency_value);

    diffTexChanged = false;
}
void ParticleShader::diffuseTexture(const Texture* pTex)
{
		this->DiffuseTexture = pTex;
        this->diffTexChanged = true;
    // TODO Clear memory before assignment?
}
