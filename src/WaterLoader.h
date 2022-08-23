/*
 * Interface Trennung: Application -> Ocean
 *
 */

#ifndef WaterLoader_HPP
#define WaterLoader_HPP

#include "OceanSegment.h"
#include "BaseModel.h"
#include <list>
#include "Camera.h"

class WaterLoader
{
public:
	virtual ~WaterLoader() = default;
	virtual OceanSegment* createWater(std::list<BaseModel*>* models) = 0;
	virtual void updateOcean(Camera* cam, double deltaTime)=0;
};

#endif