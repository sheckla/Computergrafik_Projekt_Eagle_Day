#ifndef GUIMeter_h
#define GUIMeter_h

#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"
#include "GUIConstantTriangle.h"
#include "GUITexture.h"
#include "ModelLoader.h"

class GUIMeter : public GUIBaseComponent
{
public:
	std::vector<std::vector<GUIConstantQuad*>*> groups;
	Plane* plane = ModelLoader::pPlayerPlane;
	std::vector<GUIConstantQuad*> quads;
	std::vector<GUITexture*> numbers;

	GUIConstantQuad* meter;
	GUIConstantTriangle* pointer;
	int maxHeight = 0;
	int totalQuads = 0;
	GUIMeter(int startX, int startY);
	void draw() override;
	~GUIMeter() override;
};

#endif