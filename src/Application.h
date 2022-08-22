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
#include "PlayerPlaneControls.h"
#include "PostProcessingBuffer.h"
#include "ApplicationGUI.h"
#include "GUIBaseComponent.h"
#include "ScreenQuadModel.h"
#include "EnemyPlane.h"
#include "WaterLoader.h"
#include "vector.h"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    typedef std::list<GUIBaseComponent*> GUIList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
    void glErrorHandler(GLenum err);
    static PlayerPlaneControls* planeControls;
    static Camera* Cam;
protected:
    ModelList Models;
    GUIList guis;
    GLFWwindow* pWindow;

    double last=0;
    double xPosTmp = 0;
    double yPosTmp = 0;

    ShadowMapGenerator ShadowGenerator;
    ApplicationGUI* AppGUI;
};

#endif /* Application_hpp */
