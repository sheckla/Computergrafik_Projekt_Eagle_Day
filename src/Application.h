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


class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void createShadowTestScene();
    void start();
    void update(float dtime);
    void draw();
    void end();
protected:
    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;

    double last=0;
    double xPosTmp = 0;
    double yPosTmp = 0;
    ShadowMapGenerator ShadowGenerator;
};

#endif /* Application_hpp */
