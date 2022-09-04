/*
 * PostProcessingBuffer
 * - Wird vor dem Rendern der 3D-Szene aktiviert und danach wieder deaktiviert
 * - Danach draw() aufrufen -> Post Processing Effekte werden auf die gerenderte Szene angewendet
 * - zentrales Objekt ist ScreenQuadModel (dieses wird gezeichnet)
 */
#ifndef PostProcessingBuffer_h
#define PostProcessingBuffer_h
#include "texture.h"
#include "Framebuffer.h"
#include "ScreenQuadModel.h"

constexpr float TIME_MAX_POST_PROCESSING_EFFECTS = 0.3;

class PostProcessingBuffer
{
	Texture screenTex;
	Framebuffer buffer;
	ScreenQuadModel* screenQuad = nullptr;
	float elapsedTime = 0;
	float shakeTime = 0;
	bool PostProcessingActive = false;
public:
	PostProcessingBuffer(int width, int height);
	void draw(Camera cam);
	void postDraw();
	void preDraw();

	void update(float delta);

	// PP Effect setter
	void postProcessingActive(bool b);
	void gaussianBlur(bool b);
	void shake(bool b);
	void hp(float hp);

};

#endif