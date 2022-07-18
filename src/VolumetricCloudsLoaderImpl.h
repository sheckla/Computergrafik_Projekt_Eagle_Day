#ifndef VolumetricCloudsLoaderImpl_h
#define VolumetricCloudsLoaderImpl_h

#include "Globals.h"
#include "TriangleBoxModel.h"
#include "VolumetricCloudsLoader.h"
#include "CloudShader.h"


class VolumetricCloudsLoaderImpl : public VolumetricCloudsLoader {
	const char* worleyPath;
	const char* noisePath;
public:
	VolumetricCloudsLoaderImpl(const char* worleyPath, const char* noisePath);
	~VolumetricCloudsLoaderImpl();
	std::vector<CloudBox*> createClouds(float width, float height, float depth);
};

#endif