#include "CloudShader.h"
#include "Matrix.h"
#include <sstream>


CloudShader::CloudShader() : PhongShader()
{
    // cloud fragment/vertex 
    std::string assets = ASSETS;
    std::string VSFile = assets + "vscloud.glsl";
    std::string FSFile = assets + "fscloud.glsl";

    // debug - max_layers
    GLint max_layers;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_layers);
    std::cout << max_layers;

    if (!load(VSFile.c_str(), FSFile.c_str()))
        throw std::exception();
    PhongShader::assignLocations();
    specularColor(Color(0, 0, 0));


    ViewMatrixLoc = glGetUniformLocation(ShaderProgram, "InverseViewMatrix");
    ProjectionMatrixLoc = glGetUniformLocation(ShaderProgram, "InverseProjectionMatrix");
    boundsMinLoc = glGetUniformLocation(ShaderProgram, "boundsMin");
    boundsMaxLoc = glGetUniformLocation(ShaderProgram, "boundsMax");
    NoiseLoc = glGetUniformLocation(ShaderProgram, "noise");

    for (int i = 0; i < WORLEY_AMOUNT; i++) {
        DetailTex[i] = NULL;
        std::string s;
        s += "DetailTex[" + std::to_string(i) + "]";
        std::cout << s << std::endl;
        DetailTexLoc[i] = glGetUniformLocation(ShaderProgram, s.c_str());
        DetailTexLoc[i] = getParameterID(s.c_str());
    }
}

CloudShader::~CloudShader()
{
}

void CloudShader::activate(const BaseCamera& Cam) const
{
    PhongShader::activate(Cam);

    // Worley
    int slot = 0;
    for (int i = 0; i < WORLEY_AMOUNT; i++) {
        activateTex(DetailTex[i], DetailTexLoc[i], slot++);
    }

    // Noise
    activateTex(this->Noise, this->NoiseLoc, slot++);

    Matrix iv = Cam.getViewMatrix();
    iv.invert();
    Matrix ip = Cam.getProjectionMatrix();
    ip.invert();

    setParameter(ViewMatrixLoc, iv);
    setParameter(ProjectionMatrixLoc, ip);
    setParameter(boundsMinLoc, bounds->Min);
    setParameter(boundsMaxLoc, bounds->Max);
}

void CloudShader::deactivate() const
{
    PhongShader::deactivate();
    for (int i = WORLEY_AMOUNT - 1; i >= 0; i--) {
        if (DetailTex[i] && DetailTexLoc[i] >= 0) DetailTex[i]->deactivate();

    }
    Noise->deactivate();
}

void CloudShader::activateTex(const Texture* pTex, GLint Loc, int slot) const {
    if (pTex && Loc >= 0) {
        pTex->activate(slot);
        setParameter(Loc, slot);
    }
}

void CloudShader::noise(const Texture* n)
{
    this->Noise = n;
}

void CloudShader::detailTex(unsigned int dtk, const Texture* tex)
{
    this->DetailTex[dtk] = tex;
}

void CloudShader::aabb(AABB* bounds)
{
    this->bounds = bounds;
}
