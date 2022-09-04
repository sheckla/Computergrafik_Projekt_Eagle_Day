/*
 * Application Modell-Ladeeinheit
 * - statische BaseModel* listen welche von Application Initialisiert werden
 * - Modell lassen sich so auﬂerhalb von Application reinladen
 * - zentrale Modelle wie PlayerPlane oder EnemyPlane sind als statische Instanzen vorhanden zum abrufen von ausserhalb
 */

#ifndef ModelLoader_h
#define ModelLoader_h

#include <list>
#include "BaseModel.h"
#include "Plane.h"
#include "EnemyPlane.h"
#include "WaterLoader.h"

class ModelLoader
{
    static ModelLoader* pModelLoader;
public:
    // Muessen initialisiert werden!
    static std::list<BaseModel*>* Models;
    static std::vector<BaseModel*>* CloudVector;
    static std::list<BaseModel*>* Ocean;

    static Plane* pPlayerPlane;
    static EnemyPlane* pAIPlane;
    static EnemyPlane* pEnemyPlane;
    static BaseModel* pSkyBox;
    static BaseModel* PlayerPlaneShadowArea;
    static BaseModel* PlayerPlaneSpotArea;
    static WaterLoader* pWaterLoader;

    static ModelLoader& instance();
    static bool init(std::list<BaseModel*>* Models, std::vector<BaseModel*>* Cloud_Box, std::list<BaseModel*>* Ocean);

    static bool dirLight();
    static bool skyBox();
    static bool planeParts();
    static bool planePartsOnline(std::string ip, int port);
    static bool planePartsShadowArea();
    static bool aiPlaneParts();
    static EnemyPlane* enemyPlane(std::string ip, int port);
    static bool clouds();
    static bool ocean();
    static bool ship();

};

#endif