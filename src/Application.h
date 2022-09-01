/* 
 * Application Haupt-Steuerungsklasse
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef Application_hpp
#define Application_hpp

#include <list>

#include "AIPlaneController.h"
#include "Camera.h"
#include "PhongShader.h"
#include "BaseModel.h"
#include "ShadowMapGenerator.h"
#include "PlayerPlaneControls.h"
#include "ApplicationGUI.h"
#include "GUIBaseComponent.h"
#include "EnemyPlane.h"
#include "WaterLoader.h"
#include "vector.h"
constexpr auto APPLICATION_ONLINE_MODE = false;

class Application
{
protected:
    ShadowMapGenerator ShadowGenerator;

    // Models
    std::list<BaseModel*> Models;
    std::vector<BaseModel*> Cloud_Box;
    std::list<BaseModel*> Ocean;
    WaterLoader* pWaterLoader;

    // GUI
    ApplicationGUI* AppGUI; // Steuereinheit
    std::list<GUIBaseComponent*> guis;

    // Delta Time Update
    double last=0;
    double xPosTmp = 0;
    double yPosTmp = 0;
public:
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
    void glErrorHandler(GLenum err);

    // Static-Public fuer Loader-Klassen!
    static PlayerPlaneControls* planeControls;
    static AIPlaneController* aiControls;
    static Camera* Cam;
    static EnemyPlane* enemyPlane;
    static GLFWwindow* pWindow;

    static PostProcessingBuffer* testBuffer;
};

#endif /* Application_hpp */
