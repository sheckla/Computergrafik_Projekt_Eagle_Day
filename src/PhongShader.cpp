#include "PhongShader.h"
#include <string>
#include "BaseModel.h"
#include "Model.h"
#include <sstream>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

PhongShader::PhongShader(bool loadPhongShaderCode) :
    DiffuseColor(0.8f, 0.8f, 0.8f),
    SpecularColor(0.5f, 0.5f, 0.5f),
    AmbientColor(0.2f, 0.2f, 0.2f),
    SpecularExp(20.0f),
    LightPos(20.0f, 20.0f, 20.0f),
    LightColor(1, 1, 1),
    DiffuseTexture(Texture::defaultTex()),
    NormalTexture(Texture::defaultNormalTex()),
    ShadowOnly(false),
    UpdateState(0xFFFFFFFF)
{
    if (!loadPhongShaderCode) return;

    bool loaded = load(ASSET_DIRECTORY"shaders/phong/vsphong.glsl",
        ASSET_DIRECTORY"shaders/phong/fsphong.glsl");
    if (!loaded)
    {
        print("PhongShader", "init failed", true);
        throw std::exception();
    }
    assignLocations();

    print("loading PhongShader", "default Cubemap");
    const Texture* cubeMap = Texture::defaultCubeMap();
    CubeMapTexture = cubeMap;

}
void PhongShader::assignLocations()
{
    DiffuseColorLoc = glGetUniformLocation(ShaderProgram, "DiffuseColor");
    AmbientColorLoc = glGetUniformLocation(ShaderProgram, "AmbientColor");
    SpecularColorLoc = glGetUniformLocation(ShaderProgram, "SpecularColor");
    SpecularExpLoc = glGetUniformLocation(ShaderProgram, "SpecularExp");
    DiffuseTexLoc = glGetUniformLocation(ShaderProgram, "DiffuseTexture");
    NormalTexLoc = glGetUniformLocation(ShaderProgram, "NormalTexture");
    LightPosLoc = glGetUniformLocation(ShaderProgram, "LightPos");
    LightColorLoc = glGetUniformLocation(ShaderProgram, "LightColor");
    EyePosLoc = glGetUniformLocation(ShaderProgram, "EyePos");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    CubeMapTextureLoc = glGetUniformLocation(ShaderProgram, "CubeMapTexture");
    ShadowOnlyLoc = glGetUniformLocation(ShaderProgram, "ShadowOnly");
    CubemappingLoc = glGetUniformLocation(ShaderProgram, "Cubemapping");
    PhongDiffLoc = glGetUniformLocation(ShaderProgram, "PhongDiff");
    IgnoreLightLoc = glGetUniformLocation(ShaderProgram, "IgnoreLight");

    for (int i = 0; i < MaxLightCount; ++i)
    {
        std::string smt = "ShadowMapTexture[" + std::to_string(i) + "]";
        std::string smm = "ShadowMapMat[" + std::to_string(i) + "]";
        ShadowMapTextureLoc[i] = initUniformParameter(smt.c_str());
        ShadowMapMatLoc[i] = initUniformParameter(smm.c_str());
        ShadowMapTexture[i] = NULL;
        ShadowMapMat[i].identity();
    }
}
void PhongShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);


    // update uniforms if necessary
    if (UpdateState & DIFF_COLOR_CHANGED)
        glUniform3f(DiffuseColorLoc, DiffuseColor.R, DiffuseColor.G, DiffuseColor.B);
    if (UpdateState & AMB_COLOR_CHANGED)
        glUniform3f(AmbientColorLoc, AmbientColor.R, AmbientColor.G, AmbientColor.B);
    if (UpdateState & SPEC_COLOR_CHANGED)
        glUniform3f(SpecularColorLoc, SpecularColor.R, SpecularColor.G, SpecularColor.B);
    if (UpdateState & SPEC_EXP_CHANGED)
        glUniform1f(SpecularExpLoc, SpecularExp);
    glUniform1i(CubemappingLoc, Cubemapping);
    glUniform1f(PhongDiffLoc, PhongDiff);
    glUniform1i(IgnoreLightLoc, IgnoreLight);

    int TexSlotIdx = 0;
    DiffuseTexture->activate(TexSlotIdx++);
    if (UpdateState & DIFF_TEX_CHANGED && DiffuseTexture)
        glUniform1i(DiffuseTexLoc, TexSlotIdx - 1);

	NormalTexture->activate(TexSlotIdx++);
	if (UpdateState & NORM_TEX_CHANGED && NormalTexture)
		glUniform1i(NormalTexLoc, TexSlotIdx - 1);

    CubeMapTexture->activateCubeMap(TexSlotIdx++);
    if (UpdateState & CUBE_TEX_CHANGED && CubeMapTexture)
        glUniform1i(CubeMapTextureLoc, TexSlotIdx - 1);

    if (UpdateState & LIGHT_COLOR_CHANGED)
        glUniform3f(LightColorLoc, LightColor.R, LightColor.G, LightColor.B);

    if (UpdateState & LIGHT_POS_CHANGED)
        glUniform3f(LightPosLoc, LightPos.X, LightPos.Y, LightPos.Z);

    // always update matrices
    Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
    glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);

    Vector EyePos = Cam.position();
    glUniform3f(EyePosLoc, EyePos.X, EyePos.Y, EyePos.Z);

    glUniform1i(ShadowOnlyLoc, ShadowOnly);

    for (int i = 0; i < MaxLightCount; ++i)
    {
        if (ShadowMapTexture[i] && (ShadowMapMatLoc[i] != -1))
        {
            ShadowMapTexture[i]->activate(TexSlotIdx);
            setUniformParameter(ShadowMapTextureLoc[i], TexSlotIdx++);
            setUniformParameter(ShadowMapMatLoc[i], ShadowMapMat[i]);
        }
    }

    UpdateState = 0x0;
}
void PhongShader::shadowMap(unsigned int slot, const Texture* pTex, const Matrix& Mtx)
{
    if (slot >= MaxLightCount)
        return;

    ShadowMapTexture[slot] = pTex;
    ShadowMapMat[slot] = Mtx;
}
void PhongShader::shadowOnly(bool b)
{
    ShadowOnly = b;
}
void PhongShader::cubemapping(bool b)
{
    Cubemapping = b;
}
void PhongShader::phongDiff(float f)
{
    PhongDiff = f;
}
void PhongShader::ignoreLight(int i)
{
    IgnoreLight = i;
}
void PhongShader::diffuseColor(const Color& c)
{
    DiffuseColor = c;
    UpdateState |= DIFF_COLOR_CHANGED;
}
void PhongShader::ambientColor(const Color& c)
{
    AmbientColor = c;
    UpdateState |= AMB_COLOR_CHANGED;
}
void PhongShader::specularColor(const Color& c)
{
    SpecularColor = c;
    UpdateState |= SPEC_COLOR_CHANGED;
}
void PhongShader::specularExp(float exp)
{
    SpecularExp = exp;
    UpdateState |= SPEC_EXP_CHANGED;
}
void PhongShader::lightPos(const Vector& pos)
{
    LightPos = pos;
    UpdateState |= LIGHT_POS_CHANGED;
}
void PhongShader::lightColor(const Color& c)
{
    LightColor = c;
    UpdateState |= LIGHT_COLOR_CHANGED;
}

void PhongShader::diffuseTexture(const Texture* pTex)
{
    DiffuseTexture = pTex;
    if (!DiffuseTexture)
        DiffuseTexture = Texture::defaultTex();

    UpdateState |= DIFF_TEX_CHANGED;
}

void PhongShader::normalTexture(const Texture* pTex)
{
    NormalTexture = pTex;
    if (!NormalTexture)
        NormalTexture = Texture::defaultNormalTex();

    UpdateState |= NORM_TEX_CHANGED;
}

void PhongShader::cubeMap(const Texture* pTex)
{
    this->CubeMapTexture = pTex;
    if (!CubeMapTexture)
        CubeMapTexture = Texture::defaultCubeMap();

    UpdateState |= CUBE_TEX_CHANGED;
}





