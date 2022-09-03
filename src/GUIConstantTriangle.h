/*
 * GUI-Element - Dreieck mit fester Farbe
 * - aehnlich zu GUIConstantQuad, nur bounds & buffers sind veraendert
 */

#ifndef GUIConstantTriangle_h
#define GUIConstantTriangle_h
#include "GUIConstantQuad.h"

class GUIConstantTriangle : public GUIConstantQuad
{
protected:
	bool Flip;
	void updateBuffers() override;
public:
	GUIConstantTriangle(int startPixelX, int startPixelY, int width, int height, bool flip);
	~GUIConstantTriangle() override;
	void rotate180();
};

#endif