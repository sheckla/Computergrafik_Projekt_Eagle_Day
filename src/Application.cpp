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
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "terrainshader.h"
#include "HeightMapStorage.h"
#include <thread>
#include "CloudShader.h"


#ifdef WIN32
#define ASSETS "../../assets/"
#else
#define ASSETS "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
    std::cout << "Loading..." << std::endl;
   
    loadLinePlane();
    loadSkyBox();
    loadSimpleWater();
    loadClouds();
    //loadWaterSegments();
    loadBattleship();

    
    
    std::cout << "------------------------------------------------------------------------" << std::endl;
}

void Application::loadLinePlane() {
    LinePlaneModel* pModel = new LinePlaneModel(10, 10, 10, 10);
    ConstantShader* pConstShader = new ConstantShader();
    pConstShader->color(Color(1, 0, 0));
    pModel->shader(pConstShader, true);
    Models.push_back(pModel);
}

void Application::loadSkyBox() {
    BaseModel* pModel1 = new Model(ASSETS "skybox.obj", true);
    pModel1->shader(new PhongShader(), true);
    Models.push_back(pModel1);
}

void Application::loadSimpleWater() {
    int SxS = 12*2;
    TrianglePlaneModel* lpm = new TrianglePlaneModel(10*2 * SxS,10*2 * SxS,1,1);
    ConstantShader* underwater = new ConstantShader();
    underwater->color(Color(0.023f, 0.25f, 0.45f));
    lpm->shader(underwater);
    Models.push_back(lpm);

    Matrix m;
    m.translation(Vector(9.94f*2 * SxS /2 - 9.94f*2 / 2, 0, 9.94f *2 * SxS / 2 - 9.94f*2 / 2));
    lpm->transform(m);
}

void Application::loadWaterSegments() {
    heightMaps = new HeightMapStorage(ASSETS "waves2/");
    pTerrainShader = new TerrainShader(ASSETS);

    createOceanSegments();
}

void Application::loadClouds() {
    CloudShader* cloudShader = new CloudShader(ASSETS);
    Texture* tex = new Texture;
    tex->load(ASSETS "worley0.bmp");
    cloudShader->SetWorleyTexture(tex);
    TriangleBoxModel* clouds = new TriangleBoxModel(10.0f, 1.0f, 10.0f);
    clouds->shader(cloudShader, true);
    Models.push_back(clouds);

    Matrix mvClouds;
    mvClouds.translation(Vector(0, .6f, 0));
    clouds->transform(mvClouds);
}

void Application::loadBattleship() {
    Model* mod = new Model(ASSETS "battleship2.obj", false);
    mod->shader(new PhongShader(), true);
    Models.push_back(mod);
    Matrix up;
    up.translation(Vector(0, .6f, 0));
    mod->transform(up);
}

void Application::start()
{
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime)
{
    
    //std::cout << "UPDATE" << std::endl;
    double deltaTime = glfwGetTime() - last;
    last = glfwGetTime();
    /*
    //std::cout << deltaTime << std::endl;

    int SegmXSegm = 80;

    heightMaps->time+=18.0f * deltaTime;
    if (heightMaps->time >= 100)heightMaps->time = 0;
    
    //std::cout << OceanCenter->getTransform().m03 << " " << OceanCenter->getTransform().m13 << " " << OceanCenter->getTransform().m23 << std::endl;
    //std::cout << Cam.position().X << std::endl;
    if (Cam.position().X > OceanCenter->getTransform().m03 + 20.0f * SegmXSegm/2) {
        //Matrix transl;
       // transl.translation(Vector(OceanCenter->getTransform().m03 + 20.0f, OceanCenter->getTransform().m13, OceanCenter->getTransform().m23));
       //     OceanCenter->transform(transl);

            for (int i = 0; i < SegmXSegm; i++) {
                for (int j = 0; j < SegmXSegm; j++) {
                    Matrix transl2;
                    transl2.translation(Vector(water[i][j]->getTransform().m03 + 20.0f, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23));
                    water[i][j]->transform(transl2);
                }
            }
    }
    
    if (Cam.position().X -20.0f * SegmXSegm/2 < OceanCenter->getTransform().m03) {
       // Matrix transl;
       // transl.translation(Vector(OceanCenter->getTransform().m03 - 20.0f, OceanCenter->getTransform().m13, OceanCenter->getTransform().m23));
        //OceanCenter->transform(transl);

        for (int i = 0; i < SegmXSegm; i++) {
            for (int j = 0; j < SegmXSegm; j++) {
                Matrix transl2;
                transl2.translation(Vector(water[i][j]->getTransform().m03 - 20.0f, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23));
                water[i][j]->transform(transl2);
            }
        }
    }

    if (Cam.position().Z > OceanCenter->getTransform().m23 + 20.0f * SegmXSegm/2) {
       // Matrix transl;
       // transl.translation(Vector(OceanCenter->getTransform().m03, OceanCenter->getTransform().m13, OceanCenter->getTransform().m23 + 20.0f));
      //  OceanCenter->transform(transl);
        for (int i = 0; i < SegmXSegm; i++) {
            for (int j = 0; j < SegmXSegm; j++) {
                Matrix transl2;
                transl2.translation(Vector(water[i][j]->getTransform().m03, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23 + 20.0f));
                water[i][j]->transform(transl2);
            }
        }
    }

    if (Cam.position().Z - 20.0f * SegmXSegm/2 < OceanCenter->getTransform().m23) {
       // Matrix transl;
       // transl.translation(Vector(OceanCenter->getTransform().m03, OceanCenter->getTransform().m13, OceanCenter->getTransform().m23 - 20.0f));
      //  OceanCenter->transform(transl);
        for (int i = 0; i < SegmXSegm; i++) {
            for (int j = 0; j < SegmXSegm; j++) {
                Matrix transl2;
                transl2.translation(Vector(water[i][j]->getTransform().m03, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23 - 20.0f));
                water[i][j]->transform(transl2);
            }
        }
    }

    */
    Cam.update();
}


void Application::generateSegment(int i,int j, int resolution) {
    Terrain* pTerrain = new Terrain(heightMaps, (int)resolution);
    pTerrain->shader(pTerrainShader, true);
    pTerrain->width(20);
    pTerrain->depth(20);
    pTerrain->height(20);
    //pTerrain->load();
    this->water[i][j] = pTerrain;
    //Models.push_back(pTerrain);

    if (i == 0 && j == 0)
        OceanCenter = pTerrain;

    Matrix tr;
    tr.translation(Vector(20.0f * i, 0, 20.0f * j));
    pTerrain->transform(tr);

    //std::cout << pTerrain->getTransform().m03 << " " << pTerrain->getTransform().m13 << " " << pTerrain->getTransform().m23 << std::endl;
}


void Application::createOceanSegments()
{
    float resolution = 32.0f;
    float lowestResolution = 8.0f;
    int SegmXSegm = 80;
    float abst = 20.0f;

    for (int i = 0; i < SegmXSegm; i++) {
        for (int j = 0; j < SegmXSegm; j++) {
            //std::cout << i << " " << j << std::endl;
            
            if ((i > 20 && i < 60) && (j > 20 && j < 60)) { resolution = 16.0f; }

            if ((i > 25 && i < 55) && (j > 25 && j < 55)) { resolution = 32.0f; }

            if ((i > 28 && i < 52) && (j > 28 && j < 52)) { resolution = 64.0f; }

            if ((i > 32 && i < 48) && (j > 32 && j < 48)) { resolution = 128.0f; }

            if ((i > 35 && i < 45) && (j > 35 && j < 45)) { resolution = 256.0f;  }

            if ((i > 37 && i < 43) && (j > 37 && j < 43)) { resolution = 512.0f; }
            
          
            //if (j < 10 ) { resolution = 128.0f; std::cout << "-------------------------------" << std::endl; }
            
            generateSegment(i, j, resolution);
            //std::cout <<"RES: " << resolution << std::endl;
          
            //std::cout << pTerrain->getTransform().m03 << " " << pTerrain->getTransform().m13 << " " << pTerrain->getTransform().m23 << " " << std::endl;
            resolution = lowestResolution;
        }
    }

    std::thread t1(&Application::TestMethode, this,  0);  
    std::thread t2(&Application::TestMethode, this, 10);
    std::thread t3(&Application::TestMethode, this, 20);
    std::thread t6(&Application::TestMethode, this, 50);
    std::thread t7(&Application::TestMethode, this, 60);
    std::thread t8(&Application::TestMethode, this, 70);
  
    t1.join();
    t2.join();
    t3.join();
    t6.join();
    t7.join();
    t8.join();
    std::cout << "[LOD] loading done..." << std::endl;

    std::thread t30(&Application::TestMethode, this, 30);    //SLOW, further seperation needed
    std::thread t35(&Application::TestMethode, this, 35);
    std::thread t40(&Application::TestMethode, this, 40);    //SLOW, further seperation needed
    std::thread t45(&Application::TestMethode, this, 45);

    t30.join();
    t35.join();
    t40.join();
    t45.join();

    std::cout << "loading done, initializing buffers..." << std::endl;
    
    for (int i = 0; i < SegmXSegm; i++) {
        for (int j = 0; j < SegmXSegm; j++) {
            water[i][j]->finishVB();
            water[i][j]->finishIB();
        }
    }
    
    std::cout << "buffers initialized..." << std::endl;
    
    for (int i = 0; i < SegmXSegm; i++) {
        for (int j = 0; j < SegmXSegm; j++) {
            Models.push_back(water[i][j]);
        }
    }
    
   // std::cout << water[79][78]->getTransform().m03 << " " << water[79][78]->getTransform().m13 << " " << water[79][78]->getTransform().m23 << std::endl;
    //Models.push_back(water[79][78]);
}

void Application::TestMethode(int num) {
    std::string s = "Thread ";
    s += std::to_string(num);
    s += "\n";
    std::cout << s;

    if (!(num >= 30 && num < 50)) {

        for (int i = num; i < num + 10; i++) {
            for (int j = 0; j < 80; j++) {
                //std::cout <<"TM:"<<num<<"  " << i << " " << j << std::endl;
                water[i][j]->load();
            }
        }

    }
    else {
        for (int i = num; i < num + 5; i++) {
            for (int j = 0; j < 80; j++) {
                //std::cout <<"TM:"<<num<<"  " << i << " " << j << std::endl;
                water[i][j]->load();
            }
        }
    }

    std::string sEnd = "Thread ";
    sEnd += std::to_string(num);
    sEnd += " end\n";
    std::cout << sEnd;
}

void Application::draw()
{
    //std::cout << "DRAW..." << std::endl;
    // 1. clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    // 2. setup shaders and draw models
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
    {
        (*it)->draw(Cam);
    }

    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    /*
    switch (Error)
    {
        // opengl 2 errors (8)
    case GL_NO_ERROR:
        std::cout << "GL_NO_ERROR" << std::endl;
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
    */
    assert(Error == 0);
}
void Application::end()
{
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;

    Models.clear();
}