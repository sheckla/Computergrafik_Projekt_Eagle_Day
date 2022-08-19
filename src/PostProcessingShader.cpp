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
#include <sstream>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

PostProcessingShader::PostProcessingShader()
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

}

void PostProcessingShader::screenTexture(Texture* tex)
{
	this->ScreenTexture = tex;
}
