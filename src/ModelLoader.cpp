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

ModelLoader* ModelLoader::pModelLoader = nullptr;
Plane* ModelLoader::pPlayerPlane = nullptr;
std::list<BaseModel*>* ModelLoader::Models;

ModelLoader& ModelLoader::instance()
{
    if (!pModelLoader) pModelLoader = new ModelLoader();
    return *pModelLoader;
}

bool ModelLoader::init(std::list<BaseModel*>* Models)
{
    instance().Models = Models;
    return true;
}

// via ShaderLightMapper also kein return-typ
bool ModelLoader::loadDirLight()
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

bool ModelLoader::loadLinePlane()
{
    print("loading lineplane", "constantShader");

    LinePlaneModel* pModel = new LinePlaneModel(100, 100, 100, 100);
    ConstantShader* pConstShader = new ConstantShader();
    pModel->shader(pConstShader, true);
    pModel->shadowCaster(true);
    Models->push_back(pModel);

    print("loading lineplane", "finished");
    printDivider();

    return true;
}

bool ModelLoader::loadSkyBox()
{

    print("loading skybox", "skybox.obj");

    BaseModel* pModel1 = new Model(ASSETS "/models/skybox/skybox.obj", true);
    TextureShader* tShader = new TextureShader();
    pModel1->shader(tShader);
    pModel1->shadowCaster(false);
    Models->push_back(pModel1);


    print("loading skybox", "finished");
    printDivider();

    return true;
}

bool ModelLoader::loadPlanePartsOnline(std::string ip, int port)
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

bool ModelLoader::loadPlaneParts()
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

bool ModelLoader::loadClouds()
{
    // TODO
    return true;
}

bool ModelLoader::loadSphere()
{
    print("loading sphere", "phongshader");

    TriangleSphereModel* p = new TriangleSphereModel(10, 1000, 1000);
    PhongShader* ps = new PhongShader();
    p->shader(ps);
    Models->push_back(p);

    print("loading sphere", "finished");
    printDivider();
    return true;
}

bool ModelLoader::loadTestScene()
{
    Model* pModel = new Model(ASSETS "models/shadowcube/shadowcube.obj", false);
    pModel->shader(new PhongShader(), true);
    Models->push_back(pModel);

    pModel = new Model(ASSETS "models/bunny.dae", false);
    pModel->shader(new PhongShader(), true);
    Models->push_back(pModel);
    pModel->transform(Matrix().translation(Vector(0, -0.01, 0)));

    pModel = new Model(ASSETS "models/bunny.dae", false);
    pModel->shader(new PhongShader(), true);
    Models->push_back(pModel);
    pModel->transform(Matrix().translation(Vector(0.25, 1, 1)));

    return true;
}

bool ModelLoader::loadSimpleWater()
{
    TrianglePlaneModel* lpm = new TrianglePlaneModel(20, 20, 1, 1);
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
        ModelLoader::instance().Models->push_back(cloud);
        Matrix m;
        m.translation(Vector(0, 100, 0));
        cloud->transform(m);
    }
    return true;
}

bool ModelLoader::loadEnemyPlane(std::string ip, int port)
{
    EnemyPlane* ep = new EnemyPlane(ip.c_str(), port);
    ep->loadModels(ASSETS "models/messerschmitt");
    Models->push_back(ep);
    return true;
}
