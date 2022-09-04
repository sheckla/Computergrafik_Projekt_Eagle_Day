#include "PostProcessingShader.h"
#include "BaseModel.h"
#include "Model.h"
#include "EscapeMenuGUI.h"
#include <sstream>
#include "ApplicationSettings.h"

PostProcessingShader::PostProcessingShader() : GaussianBlur(false), ElapsedTime(0)
{
	bool loaded = load(SHADERS "postprocessing/vsPostProcessing.glsl", SHADERS "postprocessing/fsPostProcessing.glsl");
	if (!loaded) throw std::exception();

	ScreenTextureLoc = initUniformParameter("ScreenTexture");
	GaussianBlurLoc = initUniformParameter("GaussianBlur");
	ElapsedTimeLoc = initUniformParameter("ElapsedTime");
	TimeMaxPostProcessingLoc = initUniformParameter("TimeMaxPostProcessing");
	ShakeLoc = initUniformParameter("Shake");
	ShakeTimeLoc = initUniformParameter("ShakeTime");
	HPLoc = initUniformParameter("HP");
	AspectHeightLoc = glGetUniformLocation(ShaderProgram, "AspectHeight");
	AspectWidthLoc = glGetUniformLocation(ShaderProgram, "AspectWidth");
	SepiaEnabledLoc = glGetUniformLocation(ShaderProgram, "SepiaEnabled");
}

void PostProcessingShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate();
	ScreenTexture->activate();
	glUniform1i(ScreenTextureLoc, 0);
	glUniform1i(GaussianBlurLoc, GaussianBlur);
	glUniform1i(ShakeLoc, Shake);
	glUniform1f(ShakeTimeLoc, ShakeTime);
	setUniformParameter(ElapsedTimeLoc, ElapsedTime);
	setUniformParameter(TimeMaxPostProcessingLoc, TIME_MAX_POST_PROCESSING_EFFECTS);
	glUniform1f(HPLoc, HP);
	setUniformParameter(AspectHeightLoc, ApplicationSettings::HEIGHT);
	setUniformParameter(AspectWidthLoc, ApplicationSettings::WIDTH);
	glUniform1i(SepiaEnabledLoc, ApplicationSettings::SEPIA_POST_PROCESSING);
}

void PostProcessingShader::screenTexture(Texture* tex)
{
	this->ScreenTexture = tex;
}

void PostProcessingShader::gaussianBlur(bool b)
{
	GaussianBlur = b;
}

void PostProcessingShader::elapsedTime(float t)
{
	ElapsedTime = t;
}

void PostProcessingShader::shake(bool b)
{
	Shake = b;
}

void PostProcessingShader::shakeTime(float f)
{
	ShakeTime = f;
}

void PostProcessingShader::hp(float hp)
{
	HP = hp;
}