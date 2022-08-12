#ifndef ModelLoader_h
#define ModelLoader_h

#include <list>

#include "BaseModel.h"
#include "Plane.h"

class ModelLoader
{

    static ModelLoader* pModelLoader;
public:
    static Plane* pPlayerPlane;
    static std::list<BaseModel*>* Models;

    static ModelLoader& instance();
    static bool init(std::list<BaseModel*>* Models);

    static bool loadDirLight();
    static bool loadLinePlane();
    static bool loadSkyBox();
    static bool loadPlaneParts();
    static bool loadClouds();
    static bool loadSphere();
    static bool loadSimpleWater();

};

#endif