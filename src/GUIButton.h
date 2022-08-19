#ifndef GUIButton_h
#define GUIButton_h
#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"

class GUIButton : public GUIBaseComponent
{
	GUIConstantQuad* area;
	Vector StartPixel;
	int Width, Height;
public:
	GUIButton(int startPixelX, int startPixelY, int width, int height);
	~GUIButton() override;
	void draw() override;
};

#endif