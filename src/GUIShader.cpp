#include "GUIShader.h"
#include "BaseModel.h"
#include "Model.h"
#include <sstream>

GUIShader::GUIShader() : pTex(nullptr), Col(1, 1, 1), ConstantColorMode(false), IsGUITex(false), MouseoverHighlight(false), MouseInside(false), IsFont(false)
{
	bool loaded = load(SHADERS "gui/vsgui.glsl",
		SHADERS "gui/fsgui.glsl");
	if (!loaded)
		throw std::exception();

	TexLoc = initUniformParameter("Tex");
	ColorLoc = initUniformParameter("Color");
	ConstantColorModeLoc = initUniformParameter("ConstantColorMode");
	IsGUITexLoc = initUniformParameter("IsGUITex");
	MouseoverHighlightLoc = initUniformParameter("MouseoverHighlight");
	MouseInsideLoc = initUniformParameter("MouseInside");
	IsFontLoc = initUniformParameter("IsFont");
}

GUIShader::~GUIShader()
{
	pTex->deactivate();
}

void GUIShader::activate() const
{
    BaseShader::activate();
	if (pTex) pTex->activate();
	glUniform1i(TexLoc, 0);
	glUniform3f(ColorLoc, Col.R, Col.G, Col.B);
	glUniform1i(ConstantColorModeLoc, ConstantColorMode);
	glUniform1i(IsGUITexLoc, IsGUITex);
	glUniform1i(MouseoverHighlightLoc, MouseoverHighlight);
	glUniform1i(MouseInsideLoc, MouseInside);
	glUniform1i(IsFontLoc, IsFont);
}

void GUIShader::texture(Texture* tex)
{
	this->pTex = tex;
}

void GUIShader::color(Color c)
{
	this->Col = c;
}

void GUIShader::constantColorMode(bool b)
{
	ConstantColorMode = b;
}

void GUIShader::isGUITex(bool b)
{
	IsGUITex = b;
}

void GUIShader::mouseoverHighlight(bool b)
{
	MouseoverHighlight = b;
}

void GUIShader::mouseInside(bool b)
{
	MouseInside = b;
}

void GUIShader::isFont(bool b)
{
	IsFont = b;
}
