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
#include "ScreenQuadModel.h"
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "terrain.h"
#include "Plane.h"
#include "HeightMapStorage.h"
#include "vector.h"
#include "EnemyPlane.h"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    typedef std::list<GUIElement*> GUIList;
    Application(GLFWwindow* pWin);
    void createShadowTestScene();
    void start();
    void update(float dtime);
    void draw();
    void end();
    Framebuffer buffer;
protected:
    Camera Cam;
    ModelList Models;
    GUIList guis;
    GLFWwindow* pWindow;

    double last=0;
    double xPosTmp = 0;
    double yPosTmp = 0;
    ShadowMapGenerator ShadowGenerator;
    Texture tex;
    ScreenQuadModel screen;
    PlayerPlaneControls* planeControls;
};

#endif /* Application_hpp */
