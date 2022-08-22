#ifndef ModelLoader_h
#define ModelLoader_h

#include <list>

#include "BaseModel.h"
#include "Plane.h"
#include "EnemyPlane.h"

class ModelLoader
{

    static ModelLoader* pModelLoader;
public:
    static Plane* pPlayerPlane;

    static std::list<BaseModel*>* Models;
    static std::vector<BaseModel*>* CloudVector;

    static ModelLoader& instance();
    static bool init(std::list<BaseModel*>* Models, std::vector<BaseModel*>* Cloud_Box);

    static bool loadDirLight();
    static bool loadTestScene();
    static bool loadLinePlane();
    static bool loadSkyBox();
    static bool loadPlaneParts();
    static bool loadPlanePartsOnline(std::string ip, int port);
    static bool loadClouds();
    static bool loadSphere();
    static bool loadSimpleWater();
    static EnemyPlane* loadEnemyPlane(std::string ip, int port);
    static bool clouds();

};

#endif