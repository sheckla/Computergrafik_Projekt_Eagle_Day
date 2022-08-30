/*
 * GUIButton
 * - Displayelement bestehend aus GUIConstantQuad(s)
 * - Funktionen bieten Moeglichkeit, um zu Wissen ob der Knopf gedrueckt wurde, oder nicht
 */

#ifndef GUIButton_h
#define GUIButton_h
#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"
#include "GUIText.h"
#include "GUITexture.h"
#include "PressListener.h"

class GUIButton : public GUIBaseComponent
{
	GLFWwindow* window;
	GUIConstantQuad* area;
	GUIText* buttonText;
	Vector StartPixel;
	PressListener mousePressListener;
	int Width, Height;

public:
	GUIButton(GLFWwindow* window, GUIConstantQuad* quad, const char* text);
	~GUIButton() override;
	void draw() override;
	void update(float delta);
	void texture(Texture* tex);
	bool pressed();
	bool mouseInside();
	PRESS_STATE listen();
};

#endif