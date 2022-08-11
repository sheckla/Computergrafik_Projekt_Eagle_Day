//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "terrainshader.h"
#include "HeightMapStorage.h"
#include <thread>
#include "CloudShader.h"
#include "PlaneLoader.h"
#include "PlaneLoaderImpl.h"
#include "VolumetricCloudsLoader.h"
#include "VolumetricCloudsLoaderImpl.h"
#include "WaterLoader.h"
#include "WaterLoaderImpl.h"
#include "PlayerPlaneControls.h"
#include <vector>
#include "CloudBox.h"

#include "EnemyPlane.h"
#include "NetworkConnector.h"
#include "NetworkSender.h"

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
    std::cout << "[Eagle Day Application] Starting..." << std::endl;
   
    //loadLinePlane();
    //loadSimpleWater();
    //loadBattleship();
    loadPlane("127.0.0.1", 19411);
    loadSkyBox();
    loadClouds();
    
    pEnemy = new EnemyPlane("127.0.0.1", 19413);
    pEnemy->loadModels(ASSETS "models/messerschmitt");
    Models.push_back(pEnemy);
    
    std::cout << "------------------------------------------------------------------------" << std::endl;
}


void Application::loadLinePlane() {
    LinePlaneModel* pModel = new LinePlaneModel(1000, 1000, 1000, 1000);
    ConstantShader* pConstShader = new ConstantShader();
    pConstShader->color(Color(0.5, 0.5, 0.5));
    pModel->shader(pConstShader, true);
    Models.push_back(pModel);
}

void Application::loadSkyBox() {
    BaseModel* pModel1 = new Model(ASSETS "models/skybox/skybox.obj", true);
    pModel1->shader(new PhongShader(), true);
    Models.push_back(pModel1);
}

void Application::loadSimpleWater() {
    int SxS = 12*2;
    TrianglePlaneModel* lpm = new TrianglePlaneModel(10*2 * SxS,10*2 * SxS,1,1);
    ConstantShader* underwater = new ConstantShader();
    underwater->color(Color(0.023f, 0.25f, 0.45f));
    lpm->shader(underwater);
    Models.push_back(lpm);

    Matrix m;
    m.translation(Vector(9.94f*2 * SxS /2 - 9.94f*2 / 2, 0, 9.94f *2 * SxS / 2 - 9.94f*2 / 2));
    lpm->transform(m);
}

void Application::loadClouds() {
    VolumetricCloudsLoader* vcs_loader;
    vcs_loader = new VolumetricCloudsLoaderImpl(ASSETS "worley/", ASSETS "noise/");
    std::vector<CloudBox*> clouds = vcs_loader->createClouds(990,40,990);

    for (auto cloud : clouds) {
        Models.push_back(cloud);
        Matrix m;
        m.translation(Vector(0, 100, 0));
        cloud->transform(m);
    }
    Cam.setPosition(Vector(10, 5, 10));
}

// Testfunktion
void Application::loadBattleship() {
    Model* mod = new Model(ASSETS "battleship2.obj", false);
    mod->shader(new PhongShader(), true);
    Models.push_back(mod);
    Matrix up;
    up.translation(Vector(0, .6f, 0));
    mod->transform(up);
}

void Application::loadPlane(const char* srv_Adr, int port) {
    PlaneLoader* pl = new PlaneLoaderImpl();
    Plane* p = pl->createPlane(ASSETS "models/spitfire",srv_Adr,port);
    Models.push_back(p);
    this->pPlane = p;
    delete pl;
}

void Application::start()
{
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime)
{
    //std::cout << "UPDATE" << std::endl;
    double deltaTime = glfwGetTime() - last; // delta = 1s/hhz, bei 165 = 0.006
    last = glfwGetTime();

     //Spitfire Controls
     //PlayerPlaneControls player(pWindow, pPlane, &Cam);
     //player.update(deltaTime);

    this->pPlane->Sender->SendData(&Cam);
    this->pEnemy->update(deltaTime);

    //std::cout <<"[AppLoop] " << this->pEnemy->Enemy_Position.X << " | " << this->pEnemy->Enemy_Position.Y << " | " << this->pEnemy->Enemy_Position.Z << std::endl;

    Cam.update();
}

void Application::draw()
{
    //std::cout << "DRAW..." << std::endl;
    // 1. clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    // 2. setup shaders and draw models
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
    {
        (*it)->draw(Cam);
    }

    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    /*
    switch (Error)
    {
        // opengl 2 errors (8)
    case GL_NO_ERROR:
        std::cout << "GL_NO_ERROR" << std::endl;
        break;

    case GL_INVALID_ENUM:
        std::cout << "GL_INVALID_ENUM" << std::endl;
        break;
    case GL_INVALID_VALUE:
        std::cout << "GL_INVALID_VALUE" << std::endl;
        break;
    case GL_INVALID_OPERATION:
        std::cout << "GL_INVALID_OPERATION" << std::endl;
        break;
    case GL_STACK_OVERFLOW:
        std::cout << "GL_STACK_OVERFLOW" << std::endl;
        break;
    case GL_STACK_UNDERFLOW:
        std::cout << "GL_STACK_UNDERFLOW" << std::endl;
        break;
    case GL_OUT_OF_MEMORY:
        std::cout << "GL_OUT_OF_MEMORY" << std::endl;
        break;
    case GL_TABLE_TOO_LARGE:
        std::cout << "GL_TABLE_TOO_LARGE" << std::endl;
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
        break;
    }
    */
    assert(Error == 0);
}
void Application::end()
{
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;

    Models.clear();
}