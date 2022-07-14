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

#ifdef WIN32
#define ASSETS "../../assets/"
#else
#define ASSETS "../assets/"
#endif

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void loadLinePlane();
    void loadSkyBox();
    void loadSimpleWater();
    void loadWaterSegments();
    void loadClouds();
    void loadBattleship();
    void loadPlane();
    void start();
    void update(float dtime);
    void draw();
    void end();

protected:
    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;
    //Terrain* pTerrain;
    //Terrain* pTerrain2;
    HeightMapStorage* heightMaps;
    void Application::generateSegment(int i, int j, int resolution);
    void Application::createOceanSegments();
    TerrainShader* pTerrainShader;
    double last=0;

    void Application::TestMethode(int num);

    Terrain* water[80][80];
    Terrain* OceanCenter;


    double x_pos_tmp = 0;
    double y_pos_tmp = 0;
    Plane* pPlane;
};

#endif /* Application_hpp */
