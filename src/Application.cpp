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
#include "PlaneLoader.h"
#include "WaterLoader.h"
#include "PlayerPlaneControls.h"
#include "ModelLoader.h"
#include "TextureShader.h"

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), ShadowGenerator(1024, 1024)
{
    print("Application loading start", "");
    // Models
    ModelLoader loader = ModelLoader::instance();
    loader.loadDirLight();
    Models.push_back(loader.loadSkyBox());
    Models.push_back(loader.loadPlane());
    Models.push_back(loader.loadLinePlane());
    Models.push_back(loader.loadSphere());
    Models.push_back(loader.loadSimpleWater());

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
    ShadowGenerator.generate(Models);

    double deltaTime = glfwGetTime() - last; // delta = 1s/hhz, bei 165 = 0.006
    last = glfwGetTime();

    // Spitfire Controls
    if (ModelLoader::pPlayerPlane) {
        PlayerPlaneControls player(pWindow, ModelLoader::pPlayerPlane, &Cam, false);
		player.update(deltaTime);
    }

    Cam.update();
}

void Application::draw()
{
    //std::cout << "DRAW..." << std::endl;
    // 1. clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ShaderLightMapper::instance().activate();
    // 2. setup shaders and draw models
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
    {
        (*it)->draw(Cam);
    }
    ShaderLightMapper::instance().deactivate();
    // 3. check once per frame for opengl errors
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