#include "GUIButton.h"

#include "GUITexture.h"
#include "MouseLogger.h"

GUIButton::GUIButton(GLFWwindow* window, GUIConstantQuad* quad, const char* text, FONT_NAMES font) : 
mousePressListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE), area(quad)
{
	this->window = window;
	GUIText* test = new GUIText(quad->startPixel().X + quad->width() / 2, quad->startPixel().Y + quad->height() / 2, text, font);
	test->charSpace(1.2);
	buttonText = new GUIText(quad->startPixel().X + quad->width() / 2 - test->totalWidth() / 2, quad->startPixel().Y + quad->height() / 2 - test->height() / 3, text, font);
	buttonText->charSpace(1.2);

	quad->mousePressColor(COL_VERY_LIGHT);
}



GUIButton::~GUIButton()
{
	
}


void GUIButton::draw()
{
	area->draw();
	buttonText->draw();
}

void GUIButton::update(float delta)
{
	PRESS_STATE state = mousePressListener.listen();
	State = state;
	switch (state)
	{
	case PRESS:
		area->mousePress(true);
		break;
	case RELEASE:
		area->mousePress(false);
		break;
	}

	area->update(delta);
}

bool GUIButton::pressed()
{
	return area->mousePress();
}

PRESS_STATE GUIButton::listen()
{
	if (!mouseInside()) return NONE;
	return mousePressListener.listen();
}

PRESS_STATE GUIButton::state()
{
	return State;
}

void GUIButton::texture(Texture* tex)
{
	area = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2, tex, true, false);
}

bool GUIButton::mouseInside()
{
	return area->mouseInside();
}
