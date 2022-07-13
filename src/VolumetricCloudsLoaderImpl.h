#ifndef VolumetricCloudsLoaderImpl_h
#define VolumetricCloudsLoaderImpl_h

#include "TriangleBoxModel.h"
#include "VolumetricCloudsLoader.h"
#include "CloudShader.h"

#define ASSETS "../../assets/"

class VolumetricCloudsLoaderImpl : public VolumetricCloudsLoader {
public:
	VolumetricCloudsLoaderImpl();
	~VolumetricCloudsLoaderImpl();
	TriangleBoxModel* createClouds(const char* path);
};

#endif