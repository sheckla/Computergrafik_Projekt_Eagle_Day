#include "ConstantShader.h"
#include "Globals.h"

ConstantShader::ConstantShader() : Col(1.0f, 1.0f, 1.0f)
{
    unsigned int vs_i = 0;
    unsigned int fs_i = 0;
	char* vs = loadFile(SHADERS "constant/vsConstant.glsl", vs_i);
    char* fs = loadFile(SHADERS "constant/fsConstant.glsl", fs_i);
	ShaderProgram = createShaderProgram(vs, fs);

    if (!vs || !fs)
    {
        print("ConstantShader()", "init failed", true);
        throw std::exception();
    }

    ColorLoc = glGetUniformLocation(ShaderProgram, "Color");
    assert(ColorLoc >= 0);
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    assert(ModelViewProjLoc >= 0);
}
void ConstantShader::color(const Color& c)
{
    this->Col = c;
}
void ConstantShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);

    glUniform3f(ColorLoc, Col.R, Col.G, Col.B);
    // always update matrices
    Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
    Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
}
