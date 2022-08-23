/*
 * PostProcessingBuffer
 * - Wird vor dem Rendern der 3D-Szene aktiviert und danach wieder deaktiviert
 * - Danach draw() aufrufen -> Post Processing Effekte werden auf die gerenderte Szene angewendet
 * - zentrales Objekt ist ScreenQuadModel
 */

#ifndef PostProcessingBuffer_h
#define PostProcessingBuffer_h
#include "texture.h"
#include "Framebuffer.h"
#include "ScreenQuadModel.h"

class PostProcessingBuffer
{
	Texture screenTex;
	Framebuffer buffer;
	ScreenQuadModel screenQuad;
public:
	PostProcessingBuffer();
	PostProcessingBuffer(int width, int height);
	void draw(Camera cam);
	void postDraw();
	void preDraw();

	void gaussianBlur(bool b);
	void elapseTime(float delta);
};

#endif