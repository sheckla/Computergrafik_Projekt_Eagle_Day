#ifndef GUIButton_h
#define GUIButton_h
#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"
#include "GUITexture.h"
#include "PressListener.h"

class GUIButton : public GUIBaseComponent
{
	GLFWwindow* window;
	GUIConstantQuad* area;
	Vector StartPixel;
	PressListener mousePressListener;
	int Width, Height;

public:
	GUIButton(GLFWwindow* window, GUIConstantQuad* quad);
	~GUIButton() override;
	void draw() override;
	void texture(Texture* tex);
	bool pressed();
	bool mouseInside();
	PRESS_STATE listen();
};

#endif