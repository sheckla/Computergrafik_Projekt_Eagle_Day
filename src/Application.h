//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
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
    Application(GLFWwindow* pWin);
    void loadLinePlane();
    void loadSkyBox();
    void loadSimpleWater();
    void loadClouds();
    void loadBattleship();
    void loadPlane(const char* srv_Adr, int port);
    void start();
    void update(float dtime);
    void draw();
    void end();

    //Vector EnemyPosition;

protected:
    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;
    HeightMapStorage* heightMaps;
    TerrainShader* pTerrainShader;
    double last=0;
    double x_pos_tmp = 0;
    double y_pos_tmp = 0;
    Plane* pPlane;
    EnemyPlane* pEnemy;
};

#endif /* Application_hpp */
