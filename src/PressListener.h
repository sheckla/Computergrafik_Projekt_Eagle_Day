/*
 * PressListener
 * - erlaubt das abhoeren von Keyboard- oder Mausklicks
 */

#ifndef PressListener_h
#define PressListener_h

#include "Globals.h"

const enum PRESS_STATE{PRESS, HOLD, RELEASE, NONE};
const enum PRESS_TYPE{KEYBOARD, MOUSE};

class PressListener
{
	GLFWwindow* Window;
	int GLFW_KEY_ID;
	PRESS_TYPE PressType;

	bool isPressed = false;
public:
	PressListener(GLFWwindow* window, int GLFW_KEY, PRESS_TYPE pressType) : Window(window), GLFW_KEY_ID(GLFW_KEY), PressType(pressType) {}

	PRESS_STATE listen()
	{
		switch (PressType)
		{
			case KEYBOARD:
				if (!isPressed && glfwGetKey(Window, GLFW_KEY_ID) == GLFW_PRESS)
				{
					isPressed = true;
					return PRESS_STATE::PRESS;
				} else if (isPressed && glfwGetKey(Window, GLFW_KEY_ID) == GLFW_RELEASE)
				{
					isPressed = false;
					return PRESS_STATE::RELEASE;
				}
				break;
			case MOUSE:
				if (!isPressed && glfwGetMouseButton(Window, GLFW_KEY_ID) == GLFW_PRESS)
				{
					isPressed = true;
					return PRESS_STATE::PRESS;
				}
				else if (isPressed && glfwGetMouseButton(Window, GLFW_KEY_ID) == GLFW_RELEASE)
				{
					isPressed = false;
					return PRESS_STATE::RELEASE;
				}
				break;
			}
	}
};

#endif