#include "GUIButton.h"

#include "GUITexture.h"

GUIButton::GUIButton(GLFWwindow* window, GUIConstantQuad* quad) : 
mousePressListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE), area(quad)
{
	this->window = window;

}



GUIButton::~GUIButton()
{
	
}


void GUIButton::draw()
{
	// first press
	PRESS_STATE state = mousePressListener.listen();

	switch (state)
	{
		case PRESS:
			area->mousePress(true);
			break;
		case RELEASE:
			area->mousePress(false);
			break;
	}

	area->draw();
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

void GUIButton::texture(Texture* tex)
{
	area = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2, tex, true, false);
}

bool GUIButton::mouseInside()
{
	return area->mouseInside();
}
