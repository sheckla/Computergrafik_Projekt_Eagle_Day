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

#include "CloudBox.h"
#include "CloudShader.h"
#include "GUIButton.h"
#include "GUILoader.h"
#include "PlaneLoader.h"
#include "WaterLoader.h"
#include "PlayerPlaneControls.h"
#include "ModelLoader.h"
#include "TextureShader.h"
#include "MouseLogger.h"
#include "GUIMeter.h"


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), ShadowGenerator(1024, 1024)
{
    // ----------- START ----------- 
    std::cout << "[Eagle Day Application] Starting..." << std::endl;


    // ----------- GUI INIT ----------- 
    GUILoader gui = GUILoader::instance();
    gui.init(&this->guis);
    gui.crossHair();
    //gui.GUI();
    GUIMeter* meter = new GUIMeter(50,20);
    GUIButton* button = new GUIButton(1000, 500, 100, 100);
    guis.push_back(meter);
    guis.push_back(button);

    // ----------- POST PROCESSING INIT ----------- 
    ppBuffer = new PostProcessingBuffer(ASPECT_WIDTH, ASPECT_HEIGHT);


    // ----------- MODEL INIT ----------- 
    ModelLoader loader = ModelLoader::instance();
    loader.init(&Models);
    loader.loadDirLight();
    loader.loadSkyBox();
    loader.loadSimpleWater();
    loader.loadPlaneParts();
    //loader.loadPlanePartsOnline("127.0.0.1", 19411);
    //loader.loadEnemyPlane("127.0.0.1", 19411);
    loader.clouds();


    // ----------- CONTROLS INIT ----------- 
    // -> CamFollowPlane = true setzen fuer verfolgende Kamera
    planeControls = new PlayerPlaneControls(pWindow, ModelLoader::pPlayerPlane, &Cam, true);


    // attach plane to GUI
    meter->plane = ModelLoader::pPlayerPlane;

    // ----------- FINISH ----------- 
    print("Application loading finished", "");
    printDivider(70);
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
    // ----------- DELTA TIME ----------- 
    double deltaTime = glfwGetTime() - last; // delta = 1s/hhz, bei 165 = 0.006
    last = glfwGetTime();



    // ----------- Plane Control handler ----------- 
    if (ModelLoader::pPlayerPlane)
    {
        this->planeControls->update(deltaTime);
    }


    // ----------- 'Singleton' MouseLogger update ----------- 
    double x,y;
    glfwGetCursorPos(pWindow, &x, &y);
    MouseLogger::instance().update(x, y);


    Cam.update();
}

void Application::draw()
{
    // ----------- FRAME START ----------- 


    // ----------- SHADOW MAPPPING ----------- 
    ShadowGenerator.generate(Models);
    ShaderLightMapper::instance().activate();


    // ----------- PostProc. Init & 3D SCENE ----------- 
    ppBuffer->preDraw();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
    {
        (*it)->draw(Cam);
    }
    ppBuffer->postDraw();

    // ----------- PostProc. DRAW ----------- 
    ppBuffer->draw(Cam);

    // ----------- GUI DRAW ----------- 
    for (GUIList::iterator it = guis.begin(); it != guis.end(); ++it)
    {
        (*it)->draw(); // no cam needed
    }

    // ----------- ERROR HANDLING ----------- 
    GLenum Error = glGetError();
    glErrorHandler(Error);
    assert(Error == 0);

    // ----------- FRAME FINISH ----------- 

}
void Application::end()
{
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;

    Models.clear();
}

void Application::glErrorHandler(GLenum err)
{
    switch (err)
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
}
