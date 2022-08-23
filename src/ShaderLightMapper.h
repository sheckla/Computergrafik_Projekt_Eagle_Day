/*
 * ShaderLightMapper
 * - statische Instanz zum hinzufuegen von Lichteinheiten fuer den ShadowMapGenerator
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include <vector>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "lights.h"

#define MaxLightCount 14
typedef std::vector<BaseLight*> LightList;

class ShaderLightMapper
{
protected:
	struct ShaderLight
	{
		int Type; Vector padding5;
		Color Color; float padding0;
		Vector Position; float padding1;
		Vector Direction; float padding2;
		Vector Attenuation; float padding3;
		Vector SpotRadius;
		int ShadowIndex;

	};

	struct ShaderLightBlock
	{
		int LightCount; Vector padding0;
		ShaderLight lights[MaxLightCount];

	};

	ShaderLightMapper();
	ShaderLightMapper(const ShaderLightMapper& m) {}
	~ShaderLightMapper();
	LightList Lights;
	ShaderLightBlock ShaderLights;
	GLuint UBO;
	static ShaderLightMapper* pMapper;
public:

	void addLight(BaseLight* Light);
	const LightList& lights() const { return Lights; }
	void clear();

	void activate();
	void deactivate();
	static ShaderLightMapper& instance();
	GLuint uniformBlockID() { return UBO; }
};

#endif

