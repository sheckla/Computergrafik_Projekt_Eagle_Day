#ifndef ModelLoader_h
#define ModelLoader_h

#include "BaseModel.h"
#include "Plane.h"

class ModelLoader
{

    static ModelLoader* pModelLoader;
public:
    static Plane* pPlayerPlane;

    static ModelLoader& instance();

    static void loadDirLight();
    static BaseModel* loadLinePlane();
    static BaseModel* loadSkyBox();
    static BaseModel* loadPlane();
    static BaseModel* loadClouds();
    static BaseModel* loadSphere();
    static BaseModel* loadSimpleWater();
};

#endif