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
#include "GUIConstantQuad.h"
#include "PlayerPlaneControls.h"
#include "PostProcessingBuffer.h"


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
};

#endif /* Application_hpp */
