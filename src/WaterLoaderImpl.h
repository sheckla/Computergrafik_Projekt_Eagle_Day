#ifndef WaterLoaderImpl_HPP
#define WaterLoaderImpl_HPP

#include "WaterLoader.h";
#include "Terrain.h"
#include "TerrainShader.h"
#include <stdio.h>
#include <string>
#include <thread>


#define ASSETS "../../assets/"
class WaterLoaderImpl : public WaterLoader
{
private:
	Terrain* water[80][80];
	Terrain* OceanCenter;
	HeightMapStorage* heightMaps;
	TerrainShader* pTerrainShader;
	double last = 0;
	std::list<BaseModel*> Models;

public:
	WaterLoaderImpl();
	~WaterLoaderImpl();
	Terrain* createWater(std::list<BaseModel*> models);

	void generateSegment(int i, int j, int resolution);

	void createOceanSegments();

	void TestMethode(int num);

};

#endif

