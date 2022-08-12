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
};

#endif