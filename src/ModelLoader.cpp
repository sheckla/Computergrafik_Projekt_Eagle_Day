#include "ModelLoader.h"

#include "CloudBox.h"
#include "GUIShader.h"
#include "PhongShader.h"
#include "PlaneLoader.h"
#include "PlaneLoaderImpl.h"
#include "ShaderLightMapper.h"
#include "TextureShader.h"
#include "TrianglePlaneModel.h"
#include "TriangleSphereModel.h"
#include "VolumetricCloudsLoader.h"
#include "VolumetricCloudsLoaderImpl.h"
#include "EnemyPlane.h"
#include "WaterLoaderImpl.h"
#include "CollisionDetector.h"

ModelLoader* ModelLoader::pModelLoader = nullptr;
Plane* ModelLoader::pPlayerPlane = nullptr;
EnemyPlane* ModelLoader::pAIPlane = nullptr;
EnemyPlane* ModelLoader::pEnemyPlane = nullptr;
BaseModel* ModelLoader::pSkyBox = nullptr;
BaseModel* ModelLoader::PlayerPlaneShadowArea = nullptr;
std::list<BaseModel*>* ModelLoader::Models;
std::vector<BaseModel*>* ModelLoader::CloudVector;
std::list<BaseModel*>* ModelLoader::Ocean;
WaterLoader* ModelLoader::pWaterLoader = nullptr;

ModelLoader& ModelLoader::instance()
{
    if (!pModelLoader) pModelLoader = new ModelLoader();
    return *pModelLoader;
}

bool ModelLoader::init(std::list<BaseModel*>* Models, std::vector<BaseModel*>* Cloud_Box, std::list<BaseModel*>* Ocean)
{
    instance().Models = Models;
    instance().CloudVector = Cloud_Box;
    instance().Ocean = Ocean;
    return true;
}

// via ShaderLightMapper also kein return-typ
bool ModelLoader::dirLight()
{
    DirectionalLight* dl = new DirectionalLight();
    dl->direction(Vector(0.2f, 1, 1));
    dl->color(Color(1, 1, 1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    DirectionalLight* dl2 = new DirectionalLight();
    dl2->direction(Vector(0, -1, 1));
    dl2->color(Color(0.9, 0.9, 0.9));
    dl2->castShadows(true);
    //ShaderLightMapper::instance().addLight(dl2);
    return true;
}

bool ModelLoader::skyBox()
{
    BaseModel* pModel1 = new Model(ASSETS "/models/skybox/skybox.obj", false);
    TextureShader* tShader = new TextureShader();
    pModel1->shader(tShader);
    pModel1->shadowCaster(false);
    Models->push_back(pModel1);

    instance().pSkyBox = pModel1;
    Matrix m = Matrix().scale(10);
    pModel1->transform(pModel1->transform() * m);

    return true;
}

bool ModelLoader::planePartsOnline(std::string ip, int port)
{
    PlaneLoader* pl = new PlaneLoaderImpl();
    Model** planeParts = new Model * [PLANE_PARTS];
    Plane* p = pl->loadPlayerPlaneOnline(ASSETS "models/spitfire", planeParts, ip.c_str(), port);
    ModelLoader::instance().pPlayerPlane = p;
    delete pl;

    for (int i = 0; i < PLANE_PARTS; i++) {
        Models->push_back(planeParts[i]);
    }

    Models->push_back(p->dot);
    return true;
}

bool ModelLoader::planeParts()
{
    PlaneLoader* pl = new PlaneLoaderImpl();
    Model** planeParts = new Model * [PLANE_PARTS];
    Plane* p = pl->loadPlayerPlane(ASSETS "models/spitfire", planeParts);
    ModelLoader::instance().pPlayerPlane = p;
    delete pl;

    for (int i = 0; i < PLANE_PARTS; i++) {
        Models->push_back(planeParts[i]);
    }
    Models->push_back(p->dot);
    //Models->push_back(p->horizon);
    return true;
}

bool ModelLoader::planePartsShadowArea()
{
    TrianglePlaneModel* lpm = new TrianglePlaneModel(22, 22, 10, 10);
    PhongShader* pShader = new PhongShader();
    pShader->shadowOnly(true);
    lpm->shadowCaster(true);
    lpm->shader(pShader);
    instance().PlayerPlaneShadowArea = lpm;
    lpm->transform(Matrix().translation(0, 0, -30));
    return true;
}

bool ModelLoader::aiPlaneParts()
{
    EnemyPlane* ep = new EnemyPlane("", 0);
    ep->loadModels(ASSETS "models/messerschmitt");
    Models->push_back(ep);

    ModelLoader::instance().pAIPlane = ep;
    return ep;

    return true;
}

bool ModelLoader::clouds()
{
    VolumetricCloudsLoader* vcs_loader;
    vcs_loader = new VolumetricCloudsLoaderImpl(ASSETS "img/noise/worley/", ASSETS "img/noise/");
    std::vector<CloudBox*> clouds = vcs_loader->createClouds(990, 40, 990);

    for (auto cloud : clouds) {
        ModelLoader::instance().CloudVector->push_back(cloud);
        Matrix m;
        m.translation(Vector(0, 100, 0));
        cloud->transform(m);
    }




    /////////////////////////////////////
    /*
    Model* ship_B = new Model(ASSETS "models/battleship/battleshipBottom.obj", false);
    Model* ship_T = new Model(ASSETS "models/battleship/battleshipTop.obj", false);

    PhongShader* pShader = new PhongShader();
    pShader->specularExp(0);

    ship_B->shader(pShader);
    ship_B->shadowCaster(false);
    ship_T->shader(pShader);
    ship_T->shadowCaster(false);
    Models->push_back(ship_B);
    Models->push_back(ship_T);

    AABB aabb = ship_B->boundingBox(); //Correct

    AABB* hitbox1 = new AABB();
    hitbox1->Max = aabb.Max;
    hitbox1->Min = aabb.Min;

    CollisionDetector::AddHitbox(hitbox1); 

    AABB aabb2 = ship_T->boundingBox();

    AABB* hitbox2 = new AABB();
    hitbox2->Max = aabb2.Max;
    hitbox2->Min = aabb2.Min;

    CollisionDetector::AddHitbox(hitbox2);
    */
    /////////////////////////////////////



    return true;
}

bool ModelLoader::ship() 
{
    Model* ship_B = new Model(ASSETS "models/battleship/battleshipBottom.obj", false);
    Model* ship_T = new Model(ASSETS "models/battleship/battleshipTop.obj", false);

    PhongShader* pShader = new PhongShader();
    pShader->specularExp(2000);
    pShader->phongDiff(3);
    pShader->ambientColor(Color(0, 0, 0));
    pShader->cubemapping(false);

    ship_B->shader(pShader);
    ship_B->shadowCaster(false);
    ship_T->shader(pShader);
    ship_T->shadowCaster(false);
    Models->push_back(ship_B);
    Models->push_back(ship_T);

    AABB aabb = ship_B->boundingBox();

    AABB* hitbox1 = new AABB();
    hitbox1->Max = aabb.Max;
    hitbox1->Min = aabb.Min;

    CollisionDetector::AddHitbox(hitbox1);

    AABB aabb2 = ship_T->boundingBox();

    AABB* hitbox2 = new AABB();
    hitbox2->Max = aabb2.Max;
    hitbox2->Min = aabb2.Min;

    CollisionDetector::AddHitbox(hitbox2);
    return true;
}

EnemyPlane* ModelLoader::enemyPlane(std::string ip, int port)
{
    EnemyPlane* ep = new EnemyPlane(ip.c_str(), port);
    ep->loadModels(ASSETS "models/messerschmitt");
    Models->push_back(ep);

    ModelLoader::instance().pEnemyPlane = ep;
    return ep;
}

bool ModelLoader::ocean()
{
    WaterLoader* pWaterLoader = new WaterLoaderImpl();
    instance().pWaterLoader = pWaterLoader;
    pWaterLoader->createWater(Ocean);
    return true;
}
