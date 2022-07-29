#include "ModelLoader.h"

#include "ConstantShader.h"
#include "Globals.h"
#include "LinePlaneModel.h"
#include "PhongShader.h"
#include "PlaneLoader.h"
#include "PlaneLoaderImpl.h"
#include "ShaderLightMapper.h"
#include "TextureShader.h"
#include "TrianglePlaneModel.h"
#include "TriangleSphereModel.h"

ModelLoader* ModelLoader::pModelLoader = nullptr;
Plane* ModelLoader::pPlayerPlane = nullptr;

ModelLoader& ModelLoader::instance()
{
    if (!pModelLoader) pModelLoader = new ModelLoader();
    return *pModelLoader;
}

// via ShaderLightMappe also kein return-typ
void ModelLoader::loadDirLight()
{
    print("loading light", "Directional");
    DirectionalLight* dl = new DirectionalLight();
    dl->direction(Vector(0.2f, -1, 1));
    dl->color(Color(0.25, 0.25, 1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);
    print("loading light", "finished");
    printDivider();
}

BaseModel* ModelLoader::loadLinePlane()
{
    print("loading lineplane", "constantShader");
    LinePlaneModel* pModel = new LinePlaneModel(1000, 1000, 100, 100);
    ConstantShader* pConstShader = new ConstantShader();
    pModel->shader(pConstShader, true);
    pModel->shadowCaster(true);
    print("loading lineplane", "finished");
    printDivider();
    return pModel;
}

BaseModel* ModelLoader::loadSkyBox()
{

    print("loading skybox", "skybox.obj");
    BaseModel* pModel1 = new Model(ASSETS "/models/skybox/skybox.obj", true);
    TextureShader* tShader = new TextureShader();
    pModel1->shader(tShader);
    pModel1->shadowCaster(true);
    print("loading skybox", "finished");
    printDivider();
    return pModel1;
}

BaseModel* ModelLoader::loadPlane()
{
    print("loading plane", "player plane");
    PlaneLoader* pl = new PlaneLoaderImpl();
    Plane* p = pl->loadPlayerPlane(ASSETS "models/spitfire");
    delete pl;
    printDivider();
    ModelLoader::instance().pPlayerPlane = p;
    return p;
}

BaseModel* ModelLoader::loadClouds()
{
    // TODO
	return nullptr;
}

BaseModel* ModelLoader::loadSphere()
{
    print("loading sphere", "phongshader");
	TriangleSphereModel* p = new TriangleSphereModel(10, 1000, 1000);
    PhongShader* ps = new PhongShader();
    p->shader(ps);
    print("loading sphere", "finished");
    printDivider();
    return p;
}

BaseModel* ModelLoader::loadSimpleWater()
{
    int SxS = 12 * 2;
    TrianglePlaneModel* lpm = new TrianglePlaneModel(10 * 2 * SxS, 10 * 2 * SxS, 1, 1);
    ConstantShader* underwater = new ConstantShader();
    underwater->color(Color(0.023f, 0.25f, 0.45f));
    lpm->shader(underwater);

    Matrix m;
    m.translation(Vector(9.94f * 2 * SxS / 2 - 9.94f * 2 / 2, 0, 9.94f * 2 * SxS / 2 - 9.94f * 2 / 2));
    lpm->transform(m);
    return lpm;
}
