#include "ModelLoader.h"

#include "CloudBox.h"
#include "ConstantShader.h"
#include "GUIShader.h"
#include "LinePlaneModel.h"
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

ModelLoader* ModelLoader::pModelLoader = nullptr;
Plane* ModelLoader::pPlayerPlane = nullptr;
EnemyPlane* ModelLoader::pEnemyPlane = nullptr;
BaseModel* ModelLoader::pSkyBox = nullptr;
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
    print("loading light", "Directional");

    DirectionalLight* dl = new DirectionalLight();
    dl->direction(Vector(0, -1, -1));
    dl->color(Color(1, 1, 1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    print("loading light", "finished");
    printDivider();
    return true;
}

bool ModelLoader::skyBox()
{
    print("loading skybox", "skybox.obj");

    BaseModel* pModel1 = new Model(ASSETS "/models/skybox/skybox.obj", false);
    TextureShader* tShader = new TextureShader();
    pModel1->shader(tShader);
    pModel1->shadowCaster(false);
    Models->push_back(pModel1);

    instance().pSkyBox = pModel1;
    Matrix m = Matrix().scale(10);
    pModel1->transform(pModel1->transform() * m);

    print("loading skybox", "finished");
    printDivider();

    return true;
}

bool ModelLoader::planePartsOnline(std::string ip, int port)
{
    print("loading plane", "player plane");

    PlaneLoader* pl = new PlaneLoaderImpl();
    Model** planeParts = new Model * [PLANE_PARTS];
    Plane* p = pl->loadPlayerPlaneOnline(ASSETS "models/spitfire", planeParts, ip.c_str(), port);
    ModelLoader::instance().pPlayerPlane = p;
    delete pl;

    for (int i = 0; i < PLANE_PARTS; i++) {
        Models->push_back(planeParts[i]);
    }

    printDivider();
    return true;
}

bool ModelLoader::planeParts()
{
    print("loading plane", "player plane");

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

    printDivider();
    return true;
}

bool ModelLoader::simpleWater()
{
    TrianglePlaneModel* lpm = new TrianglePlaneModel(200, 200, 1, 1);
    lpm->shader(new PhongShader());
    lpm->transform(Matrix().translation(Vector(0, -1, 0)));
    Models->push_back(lpm);
    return true;
}

bool ModelLoader::clouds()
{
    VolumetricCloudsLoader* vcs_loader;
    vcs_loader = new VolumetricCloudsLoaderImpl(ASSETS "worley/", ASSETS "noise/");
    std::vector<CloudBox*> clouds = vcs_loader->createClouds(990, 40, 990);

    for (auto cloud : clouds) {
        ModelLoader::instance().CloudVector->push_back(cloud);
        Matrix m;
        m.translation(Vector(0, 100, 0));
        cloud->transform(m);
    }
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
