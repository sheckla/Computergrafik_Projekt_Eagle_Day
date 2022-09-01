#include "EscapeMenuGUI.h"

#include "ApplicationGUI.h"
#include "GUIButton.h"
#include "ModelLoader.h"

EscapeMenuGUI::EscapeMenuGUI(GLFWwindow* window) : ApplicationGUIPrototype(window)
{
	escapeButtonPressListener = new PressListener(window, GLFW_KEY_ESCAPE, KEYBOARD);
}

EscapeMenuGUI::~EscapeMenuGUI()
{
}

void EscapeMenuGUI::attachPostProcessingBuffer(PostProcessingBuffer* ppBusffer)
{
	this->ppBuffer = ppBuffer;
}

void EscapeMenuGUI::postProcessingEffects(bool b)
{
	PostProcessingInit = b;
	ApplicationGUI::AppGUI->ppBuffer->gaussianBlur(true);
	ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
	//if (!b) ElapsedTime = 0;
}

bool EscapeMenuGUI::postProcessingInit()
{
	return PostProcessingInit;
}

void EscapeMenuGUI::update(float delta)
{
	if (returnStartButton->listen() == RELEASE)
	{
		active(false);
		ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
		ApplicationGUI::AppGUI->setGUIStatus(STARTSCREEN_GUI, true);
	}

	if (leaveEscapeMenuButton->listen() == RELEASE)
	{
		ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
		ModelLoader::pPlayerPlane->startEngine();
		active(false);
	}
	for (auto component : Components) component->update(delta);
}

void EscapeMenuGUI::init()
{
	GUITexture* logo = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT- 300, new Texture(ASSETS "img/logo.png"), true, false);
	logo->centred(true);
	logo->scale(Vector(0.3, 0.3, 0));
	Components.push_back(logo);

	GUITexture* returnButtonTexture = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT - 600, new Texture(ASSETS "img/button_simple.png"), true);
	returnButtonTexture->scale(Vector(0.3, 0.3, 0));
	returnButtonTexture->startPixel(Vector(returnButtonTexture->startPixel().X - returnButtonTexture->width() / 2, returnButtonTexture->startPixel().Y, 0));
	returnButtonTexture->mouseoverHighlight(true);
	returnStartButton = new GUIButton(Window, returnButtonTexture, "Main Menu");
	Components.push_back(returnStartButton);

	GUITexture* leaveEscapeMenuTex = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT - 750, new Texture(ASSETS "img/button_simple.png"), true);
	leaveEscapeMenuTex->scale(Vector(0.3, 0.3, 0));
	leaveEscapeMenuTex->startPixel(Vector(leaveEscapeMenuTex->startPixel().X - leaveEscapeMenuTex->width() / 2, leaveEscapeMenuTex->startPixel().Y, 0));
	leaveEscapeMenuTex->mouseoverHighlight(true);
	leaveEscapeMenuButton = new GUIButton(Window, leaveEscapeMenuTex, "Leave");
	Components.push_back(leaveEscapeMenuButton);

}

void EscapeMenuGUI::listen()
{
	if (escapeButtonPressListener->listen() == PRESS)
	{
		if (Active)
			// turn off
		{
			ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
			ModelLoader::pPlayerPlane->startEngine();
			print("off", "");
			active(false);
		}
		else
			// turn on
		{
			ApplicationGUI::AppGUI->ppBuffer->gaussianBlur(true);
			ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(true);
			ModelLoader::pPlayerPlane->stopEngine();
			active(true);
		}
	}
}

