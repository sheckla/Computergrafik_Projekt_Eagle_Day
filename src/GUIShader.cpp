
#include "GUIShader.h"

#include <string>
#include "BaseModel.h"
#include "Model.h"
#include <sstream>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

GUIShader::GUIShader() : Col(-1, -1, -1)
{
	bool loaded = load(SHADERS "gui/vsgui.glsl",
		SHADERS "gui/fsgui.glsl");
	if (!loaded)
		throw std::exception();

	EyePosLoc = getParameterID("EyePos");
	//assert(EyePosLoc != -1);
	ModelMatLoc = getParameterID("ModelMat");
	//assert(ModelMatLoc != -1);
	ModelViewProjMatLoc = getParameterID("ModelViewProjMat");
	TexLoc = getParameterID("Tex");
	ColorLoc = getParameterID("Color");
}

GUIShader::~GUIShader()
{
	pTex->deactivate();
}

void GUIShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
	Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	setParameter(ModelMatLoc, modelTransform());
	setParameter(ModelViewProjMatLoc, ModelViewProj);

	Vector EyePos = Cam.position();
	setParameter(EyePosLoc, EyePos);
	pTex->activate();
	glUniform1i(TexLoc, 0);
	glUniform3f(ColorLoc, Col.R, Col.G, Col.B);
}

void GUIShader::texture(Texture* tex)
{
	this->pTex = tex;
}

void GUIShader::color(Color c)
{
	this->Col = c;
}
