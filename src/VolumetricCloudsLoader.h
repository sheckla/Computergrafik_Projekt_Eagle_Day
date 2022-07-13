#ifndef VolumetricCloudsLoader_h
#define VolumetricCloudsLoader_h

#include "TriangleBoxModel.h"
#include "BaseModel.h"

class VolumetricCloudsLoader {
public:
	virtual ~VolumetricCloudsLoader() = default;
	virtual TriangleBoxModel* createClouds(const char* path) = 0;
};

#endif