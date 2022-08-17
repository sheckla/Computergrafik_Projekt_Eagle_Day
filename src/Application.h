//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <list>
#include "Camera.h"
#include "PhongShader.h"
#include "BaseModel.h"
#include "ShadowMapGenerator.h"
#include "Framebuffer.h"
#include "guiElement.h"
#include "Model.h"
#include "PlayerPlaneControls.h"
#include "PostProcessingBuffer.h"
#include "ScreenQuadModel.h"
#include "ParticleLoader.h"
#include "AudioManager.h"
#include "EnemyPlane.h"
#include "WaterLoader.h"


class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    typedef std::list<GUIElement*> GUIList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
protected:
    Camera Cam;
    ModelList Models;
    GUIList guis;
    GLFWwindow* pWindow;

    double last=0;
    double xPosTmp = 0;
    double yPosTmp = 0;

    ShadowMapGenerator ShadowGenerator;
    PlayerPlaneControls* planeControls;
    PostProcessingBuffer* ppBuffer;
    ParticleLoader* particleSystem;
    AudioManager* soundManager;
    EnemyPlane* enemy;
    WaterLoader* pWaterLoader;

    std::list<BaseModel*> Ocean;
};

#endif /* Application_hpp */
