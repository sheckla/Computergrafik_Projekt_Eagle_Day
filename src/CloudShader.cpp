#include "CloudShader.h"

void CloudShader::activate(const BaseCamera& Cam) const
{
    PhongShader::activate(Cam);
    //std::cout << "Scaling of shader: " << this->Scaling << "\n";

    int slot = 0;
    WorleyTexture->activate(slot++);
    glUniform1i(WorleyLoc, slot);

}

void CloudShader::deactivate() const
{
    if (WorleyTexture) WorleyTexture->deactivate();
    PhongShader::deactivate();
}

CloudShader::CloudShader(const std::string& AssetDirectory) : PhongShader(false)
{
    std::string VSFile = AssetDirectory + "vscloud.glsl";
    std::string FSFile = AssetDirectory + "fscloud.glsl";
    
    if (!load(VSFile.c_str(), FSFile.c_str()))
        throw std::exception();
    PhongShader::assignLocations();
    specularColor(Color(0, 0, 0));
    

    WorleyLoc = glGetUniformLocation(ShaderProgram, "WorleyTexture");
}
