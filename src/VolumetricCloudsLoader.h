/*
 * Interface
 * Trennung: Application -> VolumetricClouds
 */

#ifndef VolumetricCloudsLoader_h
#define VolumetricCloudsLoader_h

#include "TriangleBoxModel.h"
#include "BaseModel.h"
#include "CloudBox.h";

class VolumetricCloudsLoader {
public:
	virtual ~VolumetricCloudsLoader() = default;
	virtual std::vector<CloudBox*> createClouds(float width, float height, float depth) = 0;


};

#endif