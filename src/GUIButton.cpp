#include "GUIButton.h"

#include "ApplicationSettings.h"
#include "GUITexture.h"
#include "MouseLogger.h"

GUIButton::GUIButton(GLFWwindow* window, GUIConstantQuad* quad, const char* text, FONT_NAMES font, bool centred) : 
mousePressListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE), Area(quad)
{
	this->window = window;
	GUIText* tmp = new GUIText(quad->startPixel().X + quad->width() / 2, quad->startPixel().Y + quad->height() / 2, text, font); // needed for width/height adjusts
	if (centred)
	{
		tmp->charSpace(1.2);
		buttonText = new GUIText(quad->startPixel().X + quad->width() / 2 - tmp->totalWidth() / 2, quad->startPixel().Y + quad->height() / 2 - tmp->height() / 3, text, font);
		buttonText->charSpace(1.2);
	} else
	{
		buttonText = new GUIText(quad->startPixel().X + 10, quad->startPixel().Y + quad->height() / 2 - tmp->height() / 3, text, font);
	}
	delete tmp;
	quad->mousePressColor(COL_VERY_LIGHT);
}

GUIButton::~GUIButton()
{
	delete buttonText;
	delete Area;
}

void GUIButton::draw()
{
	Area->draw();
	buttonText->draw();
}

void GUIButton::update(float delta)
{
	PRESS_STATE state = mousePressListener.listen();
	State = state;
	switch (state)
	{
	case PRESS:
		Area->mousePress(true);
		break;
	case RELEASE:
		Area->mousePress(false);
		break;
	}

	Area->update(delta);
}

bool GUIButton::pressed()
{
	return Area->mousePress();
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

GUIText* GUIButton::pText()
{
	return buttonText;
}

void GUIButton::area(GUIConstantQuad* area)
{
	Area = area;
}

void GUIButton::texture(Texture* tex)
{
	Area = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT / 2, tex, true, false);
}

bool GUIButton::mouseInside()
{
	return Area->mouseInside();
}
