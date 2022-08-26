/*
 * WaterLoaderImplementation
 * - Zustaendig fuer das korrekte Laden des Oceans innerhalb des Programmes
 */

#ifndef WaterLoaderImpl_HPP
#define WaterLoaderImpl_HPP

#include "WaterLoader.h";
#include "OceanSegment.h"
#include "OceanShader.h"

class WaterLoaderImpl : public WaterLoader
{
private:
	OceanSegment* water[80][80];
	OceanSegment* OceanCenter;
	HeightMapStorage* heightMaps;
	BaseShader* pTerrainShader;
	double last = 0;
	std::list<BaseModel*> Models;

public:
	WaterLoaderImpl();
	~WaterLoaderImpl();

	OceanSegment* createWater(std::list<BaseModel*>* models);

	void generateSegment(int i, int j, int resolution);

	void createOceanSegments();

	void OceanLoaderThread(int num);

	void updateOcean(Camera* cam, double deltaTime);

};

#endif

