#include "OceanShader.h"
#include <string>
#include "Texture.h"
#include "Matrix.h"

OceanShader::OceanShader(const std::string& AssetDirectory) : PhongShader(),
Scaling(1, 1, 1), MixTex(NULL)
{
    if (!load(SHADERS "ocean/vsOcean.glsl", SHADERS "ocean/fsOcean.glsl"))
        throw std::exception();
    PhongShader::assignLocations();
    specularColor(Color(0, 0, 0));

    MixTexLoc = initUniformParameter("MixTex");
    ScalingLoc = initUniformParameter("Scaling");

    PerlinNoiseLoc = initUniformParameter("Perlin");

    ResolutionLoc = initUniformParameter("Resolution");
    ViewMatrixLoc = initUniformParameter("InvViewMatrix");
    ProjMatrixLoc = initUniformParameter("InvProjMatrix");
    

    for (int i = 0; i < DETAILTEX_COUNT; i++)
    {
        DetailTex[i] = NULL;
        std::string s;
        s += "DetailTex[" + std::to_string(i) + "]";
        DetailTexLoc[i] = initUniformParameter(s.c_str());
    }

    std::string pNoise = AssetDirectory + "/img/noise/perlin_noise.jpg";
    PerlinTex = new Texture(pNoise.c_str());
}

void OceanShader::activate(const BaseCamera& Cam) const
{
    PhongShader::activate(Cam);
    //std::cout << "Scaling of shader: " << this->Scaling << "\n";
    int slot = 0;
    activateTex(MixTex, MixTexLoc, slot++);

    activateTex(PerlinTex, PerlinNoiseLoc, slot++);

    for (int i = 0; i < DETAILTEX_COUNT; i++)
        activateTex(DetailTex[i], DetailTexLoc[i], slot++);


    setUniformParameter(ScalingLoc, Scaling);
    setUniformParameter(ResolutionLoc, Resolution);

    Matrix iv = Cam.getViewMatrix();
    iv.invert();
    Matrix ip = Cam.getProjectionMatrix();
    ip.invert();
    setUniformParameter(ViewMatrixLoc, iv);
    setUniformParameter(ProjMatrixLoc, ip);
}

void OceanShader::deactivate() const
{
    PhongShader::deactivate();
    for (int i = DETAILTEX_COUNT - 1; i >= 0; i--)
        if (DetailTex[i] && DetailTexLoc[i] >= 0) DetailTex[i]->deactivate();
    if (MixTex) MixTex->deactivate();

    if (PerlinTex) PerlinTex->deactivate();
}

void OceanShader::activateTex(const Texture* pTex, GLint Loc, int slot) const
{
    if (pTex && Loc >= 0)
    {
        pTex->activate(slot);
        setUniformParameter(Loc, slot);
    }

}
