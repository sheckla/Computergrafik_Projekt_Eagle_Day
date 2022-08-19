#include "GUIButton.h"

GUIButton::GUIButton(int startPixelX, int startPixelY, int width, int height) : StartPixel(Vector(startPixelX, startPixelY, 0)), Width(width), Height(height),
area(new GUIConstantQuad(startPixelX, startPixelY, width, height))
{
	area->mouseoverHighlight(true);
}

GUIButton::~GUIButton()
{
}

void GUIButton::draw()
{
	area->draw();
}
