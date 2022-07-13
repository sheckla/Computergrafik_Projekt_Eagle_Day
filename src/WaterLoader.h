#ifndef WaterLoader_HPP
#define WaterLoader_HPP

#include "Terrain.h"
#include "BaseModel.h"
#include <list>

class WaterLoader
{
public:
	virtual ~WaterLoader() = default;
	virtual Terrain* createWater(std::list<BaseModel*> models) = 0;
};

#endif