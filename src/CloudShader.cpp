#include "CloudShader.h"
#include "Matrix.h"
#include <sstream>

#include "ApplicationSettings.h"

float CloudShader::TimeTranslation = 0.0f;

CloudShader::CloudShader() : PhongShader()
{
    std::cout << "[Clouds] Loading Volumetric Clouds..." << std::endl;
    // cloud fragment/vertex 
    std::string assets = ASSETS;
    std::string VSFile = SHADERS "cloud/vscloud.glsl";
    std::string FSFile = SHADERS "cloud/fscloud.glsl";

    // debug - max_layers
    GLint max_layers;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_layers);
    //std::cout << max_layers<<std::endl;

    if (!load(VSFile.c_str(), FSFile.c_str()))
        throw std::exception();
    PhongShader::assignLocations();
    specularColor(Color(0, 0, 0));


    ViewMatrixLoc = glGetUniformLocation(ShaderProgram, "InverseViewMatrix");
    ProjectionMatrixLoc = glGetUniformLocation(ShaderProgram, "InverseProjectionMatrix");
    boundsMinLoc = glGetUniformLocation(ShaderProgram, "boundsMin");
    boundsMaxLoc = glGetUniformLocation(ShaderProgram, "boundsMax");
    NoiseLoc = glGetUniformLocation(ShaderProgram, "noise");
    AspectHeightLoc = glGetUniformLocation(ShaderProgram, "AspectHeight");
    AspectWidthLoc = glGetUniformLocation(ShaderProgram, "AspectWidth");
    MaxTextureLoc = glGetUniformLocation(ShaderProgram, "MaxTexture");

    //Time translation for cloud movement
    TimeTranslationLoc = glGetUniformLocation(ShaderProgram, "TimeTranslation");

    //Loading of noise-textures
    for (int i = 0; i < WORLEY_AMOUNT; i++) {
        if (i < max_layers)
        {
	        
        DetailTex[i] = NULL;
        std::string s;
        s += "DetailTex[" + std::to_string(i) + "]";
        //std::cout << s << std::endl;
        DetailTexLoc[i] = glGetUniformLocation(ShaderProgram, s.c_str());
        DetailTexLoc[i] = initUniformParameter(s.c_str());
        }
    }

    std::cout << "[Clouds] Loading done..." << std::endl;
}

CloudShader::~CloudShader()
{
}

void CloudShader::activate(const BaseCamera& Cam) const
{
    PhongShader::activate(Cam);
    GLint max_layers;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_layers);
    setUniformParameter(MaxTextureLoc, max_layers);
    // Worley
    int slot = 0;
    for (int i = 0; i < WORLEY_AMOUNT; i++) {
        if (i < max_layers) activateTex(DetailTex[i], DetailTexLoc[i], slot++);
    }

    // Noise
    activateTex(this->Noise, this->NoiseLoc, slot++);

    Matrix iv = Cam.getViewMatrix();
    iv.invert();
    Matrix ip = Cam.getProjectionMatrix();
    ip.invert();

    //Matrices for ray- and position-calculations
    setUniformParameter(ViewMatrixLoc, iv);
    setUniformParameter(ProjectionMatrixLoc, ip);
    setUniformParameter(boundsMinLoc, bounds->Min);
    setUniformParameter(boundsMaxLoc, bounds->Max);
    setUniformParameter(AspectWidthLoc, ApplicationSettings::WIDTH);
    setUniformParameter(AspectHeightLoc, ApplicationSettings::HEIGHT);
    setUniformParameter(TimeTranslationLoc, CloudShader::TimeTranslation);
}

void CloudShader::deactivate() const
{
    PhongShader::deactivate();
    for (int i = 16 - 1; i >= 0; i--) {
        if (DetailTex[i] && DetailTexLoc[i] >= 0) DetailTex[i]->deactivate();

    }
    Noise->deactivate();
}

void CloudShader::activateTex(const Texture* pTex, GLint Loc, int slot) const {
    if (pTex && Loc >= 0) {
        pTex->activate(slot);
        setUniformParameter(Loc, slot);
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