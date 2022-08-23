//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#include "CloudShader.h"
#include "GUILoader.h"
#include "PlaneLoader.h"
#include "WaterLoader.h"
#include "PlayerPlaneControls.h"
#include "ModelLoader.h"
#include "TextureShader.h"
#include "MouseLogger.h"
#include "AudioManager.h"
#include "WaterLoaderImpl.h"

PlayerPlaneControls* Application::planeControls = nullptr;
Camera* Application::Cam;
EnemyPlane* Application::enemyPlane = nullptr;


Application::Application(GLFWwindow* pWin) : pWindow(pWin), ShadowGenerator(1024, 1024), AppGUI(new ApplicationGUI(pWin))
{
    // ----------- START ----------- 
    std::cout << "[Eagle Day Application] Starting..." << std::endl;

    // ----------- MODEL INIT ----------- 
    ModelLoader::instance().init(&Models, &Cloud_Box, &Ocean);


    // ----------- GUI INIT -----------
    AppGUI->setGUIStatus(LOADING_SCREEN_GUI, true);

    // ----------- CONTROLS INIT ----------- 
    // -> CamFollowPlane = true setzen fuer verfolgende Kamera
    //planeControls = new PlayerPlaneControls(pWindow, ModelLoader::pPlayerPlane, &Cam, true);


    /// TODO Als asset einfuegen :p
    /*Model* ship = new Model("C:/Users/Computer/Desktop/hms_victorious.obj");
    ship->shader(new PhongShader, true);
    Matrix sm,ss;
    sm.translation(Vector(0, 10, 0));
    ss.scale(30);
    ship->transform(sm * ss);
    Models.push_back(ship)*/;

    // ----------- FINISH ----------- 
    print("Application loading finished", "");
    printDivider(70);
}

void Application::start()
{
    glEnable(GL_DEPTH_TEST); // postProcessingEffects depth-testing
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

    // ----------- 'Singleton' MouseLogger update ----------- 
    double x,y;
    glfwGetCursorPos(pWindow, &x, &y);
    MouseLogger::instance().update(x, y);

    // ----------- GUI Keyboard Input update ----------- 
    AppGUI->updateInputs(deltaTime);
    if (AppGUI->status().loadingScreen ||AppGUI->status().escapeMenu) return;

    // ----------- (GAME) Plane Control handler ----------- 
    if (ModelLoader::pPlayerPlane)
    {
        this->planeControls->update(deltaTime);
        if (APPLICATION_ONLINE_MODE)this->enemyPlane->update(deltaTime);
    }


    // ----------- Model Update ----------- 
    ModelLoader::pWaterLoader->updateOcean(Cam, deltaTime);
    CloudShader::TimeTranslation = last;

    // Scale > 6 gibt clipping Probleme, evtl Kamera anpassen ( oder andere sizes von anderen Objekten)
    ModelLoader::pSkyBox->transform(Matrix().translation(Vector(ModelLoader::pPlayerPlane->getPosition().X, 0, ModelLoader::pPlayerPlane->getPosition().Z)) * Matrix().scale(6));

    Cam->update();
}

void Application::draw()
{
    // ----------- FRAME START ----------- 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!AppGUI->status().startscreenGUI && !AppGUI->status().loadingScreen) {
        // ----------- SHADOW MAPPPING ----------- 
        ShadowGenerator.generate(Models);
        ShaderLightMapper::instance().activate();

        // ----------- PostProc. Init & 3D SCENE ----------- 
        AppGUI->ppBuffer->preDraw();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
        {
            (*it)->draw(*Cam);
        }

        for (std::list<BaseModel*>::iterator it = Ocean.begin(); it != Ocean.end(); it++)
        {
            (*it)->draw(*Cam);
        }

        ModelLoader::pPlayerPlane->drawParticles(*Cam);

        for (auto cloud : Cloud_Box)
        {
            cloud->draw(*Cam);
        }


        AppGUI->ppBuffer->postDraw();

        // ----------- PostProc. DRAW ----------- 
        AppGUI->ppBuffer->draw(*Cam);
    }

    // ----------- GUI DRAW ----------- 
    AppGUI->draw();


    // ----------- ERROR HANDLING ----------- 
    GLenum Error = glGetError();
    glErrorHandler(Error);
    assert(Error == 0);

    // ----------- FRAME FINISH ----------- 

}
void Application::end()
{
    for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;
    for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;

    for (std::list<BaseModel*>::iterator it = this->Ocean.begin(); it != this->Ocean.end(); ++it)
        delete* it;
    Ocean.clear();
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
