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

#include "CloudShader.h"
#include "guiElement.h"
#include "PlaneLoader.h"
#include "WaterLoader.h"
#include "PlayerPlaneControls.h"
#include "ModelLoader.h"
#include "TextureShader.h"
#include "ScreenQuadModel.h"
#include "guiElement.h"
#include "GUILoader.h"
#include "MouseLogger.h"


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), ShadowGenerator(1024, 1024)
{
    print("Application loading start", "");

    // GUI
    GUILoader gui = GUILoader::instance();
    gui.init(&this->guis);
    gui.crossHair();
    gui.GUI();

    // Models
    ModelLoader loader = ModelLoader::instance();
    loader.init(&Models);
    loader.loadDirLight();
    loader.loadSkyBox();
    loader.loadSimpleWater();
    loader.loadPlaneParts();

    // Controls
    planeControls = new PlayerPlaneControls(pWindow, ModelLoader::pPlayerPlane, &Cam, false);

    // Post Processing
    ppBuffer = new PostProcessingBuffer(ASPECT_WIDTH, ASPECT_HEIGHT);;

    print("Application loading finished", "");
    printDivider(70);
}

void Application::createShadowTestScene()
{
    Model* pModel = new Model(ASSETS "models/shadowcube/shadowcube.obj", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);

    pModel = new Model(ASSETS "models/bunny.dae", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);
    pModel->transform(Matrix().translation(Vector(0, -0.01, 0)));

    pModel = new Model(ASSETS "models/bunny.dae", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);
    pModel->transform(Matrix().translation(Vector(0.25, 1, 1)));

    pModel = new Model(ASSETS "models/spitfire/backwing_right.obj");
    pModel->shader(new PhongShader(), true);
    pModel->transform(Matrix().translation(Vector(0, 1, 1)));
    Models.push_back(pModel);

	pModel = new Model(ASSETS "models/spitfire/backwing_right.obj");
    pModel->shader(new PhongShader(), true);
    pModel->transform(Matrix().translation(Vector(0, 2, 2.1)));
    Models.push_back(pModel);

    // directional lights
    DirectionalLight* dl = new DirectionalLight();
    dl->direction(Vector(0, -1, -1));
    dl->color(Color(0.5, 0.5, 0.5));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    SpotLight* sl = new SpotLight();
    sl->position(Vector(2, 2, 0));
    sl->color(Color(0.5, 0.5, 0.5));
    sl->direction(Vector(-1, -1, 0));
    sl->innerRadius(10);
    sl->outerRadius(13);
    sl->castShadows(true);
    ShaderLightMapper::instance().addLight(sl);
}

void Application::start()
{
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void Application::update(float dtime)
{

    double deltaTime = glfwGetTime() - last; // delta = 1s/hhz, bei 165 = 0.006
    last = glfwGetTime();

    // Spitfire Controls
    if (ModelLoader::pPlayerPlane) {
		planeControls->update(deltaTime);
    }

    // Update Mouse-Pos
    double x,y;
    glfwGetCursorPos(pWindow, &x, &y);
    MouseLogger::instance().update(x, y);

    // Finally update Cam
    Cam.update();
}

void Application::draw()
{
    // Shadow Mapping
    ShadowGenerator.generate(Models);
    ShaderLightMapper::instance().activate();

    // Main Buffer
    ppBuffer->preDraw();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
    {
        (*it)->draw(Cam);
    }
    ppBuffer->postDraw();

    ppBuffer->draw(Cam);

    for (GUIList::iterator it = guis.begin(); it != guis.end(); ++it)
    {
        (*it)->draw(Cam);
    }

    GLenum Error = glGetError();
    
    switch (Error)
    {
        // opengl 2 errors (8)
    case GL_NO_ERROR:
        //std::cout << "GL_NO_ERROR" << std::endl;
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
    
    assert(Error == 0);
}
void Application::end()
{
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;

    Models.clear();
}