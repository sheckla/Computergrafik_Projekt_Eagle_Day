
/*
 * GUIChar
 * - einzelnes Zeichen
 * - FontData muss mittels einer entsprechenden .fnt Datei uebergeben werden
 */
#ifndef GUIChar_h
#define GUIChar_h
#include <vector>

#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"
#include "GUITexture.h"

struct FontData
{
	FontData(int id, int x, int y, int width, int height) : id(id), x(x), y(y), width(width), height(height){}
	FontData(){}
	int id;
	int x;
	int y;
	int width;
	int height;
};

class GUIChar : public GUITexture
{
	FontData data;
	std::vector<GUIChar*> chars;
public:
	void updateBuffers() override;
	GUIChar(int startX, int startY, int width, int height, FontData data);
	virtual ~GUIChar();
};

#endif