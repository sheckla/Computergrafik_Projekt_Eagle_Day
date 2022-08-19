#ifndef GUIConstantTriangle_h
#define GUIConstantTriangle_h
#include "GUIConstantQuad.h"

/*
 * GUI-Element - Dreieck mit fester Farbe
 * - aehnlich zu GUIConstantQuad, nur bounds & buffers sind veraendert
 */
class GUIConstantTriangle : public GUIConstantQuad
{
protected:


	void updateBuffers() override;
	void updateBounds() override;
public:
	GUIConstantTriangle(int startPixelX, int startPixelY, int width, int height);
	~GUIConstantTriangle() override;
	void draw() override;
};

#endif