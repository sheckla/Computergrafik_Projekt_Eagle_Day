#include "WaterLoaderImpl.h"

#include <thread>
#include "Globals.h"

WaterLoaderImpl::WaterLoaderImpl()
{
}

WaterLoaderImpl::~WaterLoaderImpl()
{
    delete[] water;
}

OceanSegment* WaterLoaderImpl::createWater(std::list<BaseModel*>* models)
{
    print("HeightMapStorage", "loading");
	heightMaps = new HeightMapStorage(ASSETS "img/noise/waves/");
    print("HeightMapStorage", "finished");
	pTerrainShader = new OceanShader(ASSETS);
    createOceanSegments();

    for(int i=0;i<80;i++)
        for (int j = 0; j < 80; j++)
            models->push_back(this->water[i][j]);

    return (this->water[0][0]);
}

void WaterLoaderImpl::generateSegment(int i, int j, int resolution) {
    OceanSegment* pTerrain = new OceanSegment(heightMaps, (int)resolution);
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

void WaterLoaderImpl::createOceanSegments()
{
    float resolution = 32.0f;
    float lowestResolution = 8.0f;
    int SegmXSegm = 80;
    float abst = 20.0f;

    float res_ring_0 = 256.0f;
    float res_ring_1 = 128.0f;
    float res_ring_2 = 64.0f;
    float res_ring_3 = 64.0f;
    float res_ring_4 = 32.0f;
    float res_ring_5 = 16.0f;


    int q = QUALITY;
    std::cout << "Loading Ocean with Qualitysetting: " << q << std::endl;

    if (q == 0) 
    {
        res_ring_0 = 8.0f;
        res_ring_1 = 8.0f;
        res_ring_2 = 8.0f;
        res_ring_3 = 8.0f;
        res_ring_4 = 8.0f;
        res_ring_5 = 8.0f;
    }

    if (q == 1)
    {
        res_ring_0 = 64.0f;
        res_ring_1 = 32.0f;
        res_ring_2 = 16.0f;
        res_ring_3 = 8.0f;
        res_ring_4 = 8.0f;
        res_ring_5 = 8.0f;
    }

    if (q == 2)
    {
        res_ring_0 = 128.0f;
        res_ring_1 = 64.0f;
        res_ring_2 = 32.0f;
        res_ring_3 = 16.0f;
        res_ring_4 = 8.0f;
        res_ring_5 = 8.0f;
    }

    if (q == 3)
    {
        res_ring_0 = 128.0f;
        res_ring_1 = 128.0f;
        res_ring_2 = 64.0f;
        res_ring_3 = 32.0f;
        res_ring_4 = 16.0f;
        res_ring_5 = 16.0f;
    }

    for (int i = 0; i < SegmXSegm; i++) {
        for (int j = 0; j < SegmXSegm; j++) {

            if ((i > 20 && i < 60) && (j > 20 && j < 60)) { resolution = res_ring_5; }

            if ((i > 25 && i < 55) && (j > 25 && j < 55)) { resolution = res_ring_4; }

            if ((i > 28 && i < 52) && (j > 28 && j < 52)) { resolution = res_ring_3; }

            if ((i > 32 && i < 48) && (j > 32 && j < 48)) { resolution = res_ring_2; }

            if ((i > 36 && i < 44) && (j > 36 && j < 44)) { resolution = res_ring_1; }

            if ((i > 38 && i < 42) && (j > 38 && j < 42)) { resolution = res_ring_0; }// inner ring


            //if (j < 10 ) { resolution = 128.0f; std::cout << "-------------------------------" << std::endl; }

            generateSegment(i, j, resolution);
            //std::cout <<"RES: " << resolution << std::endl;

            //std::cout << pTerrain->getTransform().m03 << " " << pTerrain->getTransform().m13 << " " << pTerrain->getTransform().m23 << " " << std::endl;
            resolution = lowestResolution;
        }
    }

    print("Ocean", "creating Threads...");
    std::thread t1(&WaterLoaderImpl::OceanLoaderThread, this, 0);
    std::thread t2(&WaterLoaderImpl::OceanLoaderThread, this, 10);
    std::thread t3(&WaterLoaderImpl::OceanLoaderThread, this, 20);
    std::thread t6(&WaterLoaderImpl::OceanLoaderThread, this, 50);
    std::thread t7(&WaterLoaderImpl::OceanLoaderThread, this, 60);
    std::thread t8(&WaterLoaderImpl::OceanLoaderThread, this, 70);

    t1.join();
    t2.join();
    t3.join();
    t6.join();
    t7.join();
    t8.join();

    std::thread t30(&WaterLoaderImpl::OceanLoaderThread, this, 30);    //SLOW, further seperation needed
    std::thread t35(&WaterLoaderImpl::OceanLoaderThread, this, 35);
    std::thread t40(&WaterLoaderImpl::OceanLoaderThread, this, 40);    //SLOW, further seperation needed
    std::thread t45(&WaterLoaderImpl::OceanLoaderThread, this, 45);

    t30.join();
    t35.join();
    t40.join();
    t45.join();

    std::cout << "Threads loading done, initializing buffers..." << std::endl;

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

    std::cout << "Ocean finished" << std::endl;
    // std::cout << water[79][78]->getTransform().m03 << " " << water[79][78]->getTransform().m13 << " " << water[79][78]->getTransform().m23 << std::endl;
     //Models.push_back(water[79][78]);
}

void WaterLoaderImpl::OceanLoaderThread(int num) { // bad alloc
    std::string s = "Thread ";
    s += std::to_string(num);
    s += " begin";
    //print("OceanSegmentThread", s);

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
    s += " end";
    //print("OceanSegmentThread", sEnd);
}

void WaterLoaderImpl::updateOcean(Camera* Cam, double deltaTime) 
{ 
    heightMaps->time += 18.0f * deltaTime;
    if (heightMaps->time >= 100)heightMaps->time = 0;

    int SegmXSegm = 80;
    //std::cout << "updateOcean"<<std::endl; 
    if (Cam->position().X > OceanCenter->getTransform().m03 + 20.0f * SegmXSegm / 2) {
        for (int i = 0; i < SegmXSegm; i++) {
            for (int j = 0; j < SegmXSegm; j++) {
                Matrix transl2;
                transl2.translation(Vector(water[i][j]->getTransform().m03 + 20.0f, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23));
                water[i][j]->transform(transl2);
            }
        }
    }

    if (Cam->position().X - 20.0f * SegmXSegm / 2 < OceanCenter->getTransform().m03) {
        for (int i = 0; i < SegmXSegm; i++) {
            for (int j = 0; j < SegmXSegm; j++) {
                Matrix transl2;
                transl2.translation(Vector(water[i][j]->getTransform().m03 - 20.0f, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23));
                water[i][j]->transform(transl2);
            }
        }
    }

    if (Cam->position().Z > OceanCenter->getTransform().m23 + 20.0f * SegmXSegm / 2) {
        for (int i = 0; i < SegmXSegm; i++) {
            for (int j = 0; j < SegmXSegm; j++) {
                Matrix transl2;
                transl2.translation(Vector(water[i][j]->getTransform().m03, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23 + 20.0f));
                water[i][j]->transform(transl2);
            }
        }
    }

    if (Cam->position().Z - 20.0f * SegmXSegm / 2 < OceanCenter->getTransform().m23) {
        for (int i = 0; i < SegmXSegm; i++) {
            for (int j = 0; j < SegmXSegm; j++) {
                Matrix transl2;
                transl2.translation(Vector(water[i][j]->getTransform().m03, water[i][j]->getTransform().m13, water[i][j]->getTransform().m23 - 20.0f));
                water[i][j]->transform(transl2);
            }
        }
    }
}