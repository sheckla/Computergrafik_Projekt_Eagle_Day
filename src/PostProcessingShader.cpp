//
//  PhongShader.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "PostProcessingShader.h"

#include <string>
#include "BaseModel.h"
#include "Model.h"
#include "EscapeMenuGUI.h"
#include <sstream>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

PostProcessingShader::PostProcessingShader() : GaussianBlur(false), ElapsedTime(0)
{
	bool loaded = load(SHADERS "postprocessing/vsPostProcessing.glsl",
		SHADERS "postprocessing/fsPostProcessing.glsl");
	if (!loaded)
		throw std::exception();

	EyePosLoc = initUniformParameter("EyePos");
	//assert(EyePosLoc != -1);
	ModelMatLoc = initUniformParameter("ModelMat");
	//assert(ModelMatLoc != -1);
	ModelViewProjMatLoc = initUniformParameter("ModelViewProjMat");
	ScreenTextureLoc = initUniformParameter("ScreenTexture");
	GaussianBlurLoc = initUniformParameter("GaussianBlur");
	ElapsedTimeLoc = initUniformParameter("ElapsedTime");
	TimeMaxPostProcessingLoc = initUniformParameter("TimeMaxPostProcessing");
	ShakeLoc = initUniformParameter("Shake");
	ShakeTimeLoc = initUniformParameter("ShakeTime");
	HPLoc = initUniformParameter("HP");
}

void PostProcessingShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
	Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	setUniformParameter(ModelMatLoc, modelTransform());
	setUniformParameter(ModelViewProjMatLoc, ModelViewProj);

	Vector EyePos = Cam.position();
	setUniformParameter(EyePosLoc, EyePos);
	ScreenTexture->activate();
	glUniform1i(ScreenTextureLoc, 0);
	glUniform1i(GaussianBlurLoc, GaussianBlur);
	glUniform1i(ShakeLoc, Shake);
	glUniform1f(ShakeTimeLoc, ShakeTime);
	setUniformParameter(ElapsedTimeLoc, ElapsedTime);
	setUniformParameter(TimeMaxPostProcessingLoc, TIME_MAX_POST_PROCESSING_EFFECTS);
	glUniform1f(HPLoc, HP);
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