#include "EscapeMenuGUI.h"
#include "ApplicationGUI.h"
#include "ApplicationSettings.h"
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
	// Main Menu Button
	if (returnStartButton->listen() == RELEASE)
	{
		active(false);
		ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
		ApplicationGUI::AppGUI->setGUIStatus(STARTSCREEN_GUI, true);
		ApplicationGUI::AppGUI->gameplayGUI->active(true);
	}

	// Close Button
	if (leaveEscapeMenuButton->listen() == RELEASE)
	{
		ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
		ModelLoader::pPlayerPlane->startEngine();
		ApplicationGUI::AppGUI->gameplayGUI->active(true);
		active(false);
	}

	// Restart Button
	if (restartButton->listen() == RELEASE)
	{
		ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
		ModelLoader::pPlayerPlane->initModelTranslation();
		if (ModelLoader::pAIPlane) {
			ModelLoader::pAIPlane->initModelTranslation();
			ModelLoader::pAIPlane->hp = 100;
		}
		ModelLoader::pPlayerPlane->startEngine();
		ApplicationGUI::AppGUI->gameplayGUI->active(true);
		active(false);
	}

	// Update everything else
	for (auto component : Components) component->update(delta);
}

void EscapeMenuGUI::init()
{
	// Escape Menu Top Logo
	GUITexture* logo = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 200, new Texture(ASSETS "img/logo.png"), true, false);
	logo->centred(true);
	logo->scale(Vector(0.3, 0.3, 0));
	Components.push_back(logo);

	// Return to StartScreen/Main Menu
	GUITexture* returnButtonTexture = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 500, new Texture(ASSETS "img/button_simple_gray.png"), true);
	returnButtonTexture->scale(Vector(0.3, 0.3, 0));
	returnButtonTexture->startPixel(Vector(returnButtonTexture->startPixel().X - returnButtonTexture->width() / 2, returnButtonTexture->startPixel().Y, 0));
	returnButtonTexture->mouseoverHighlight(true);
	returnStartButton = new GUIButton(Window, returnButtonTexture, "Main Menu");
	Components.push_back(returnStartButton);

	// Restart Game State Button
	GUITexture* restartButtonTex = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 630, new Texture(ASSETS "img/button_simple_gray.png"), true);
	restartButtonTex->scale(Vector(0.3, 0.3, 0));
	restartButtonTex->startPixel(Vector(restartButtonTex->startPixel().X - restartButtonTex->width() / 2, restartButtonTex->startPixel().Y, 0));
	restartButtonTex->mouseoverHighlight(true);
	restartButton = new GUIButton(Window, restartButtonTex, "Restart", ARIAL_BLACK, true);
	Components.push_back(restartButton);

	// Close Escape Menu BUtton
	GUITexture* leaveEscapeMenuTex = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 760, new Texture(ASSETS "img/button_simple.png"), true);
	leaveEscapeMenuTex->scale(Vector(0.3, 0.3, 0));
	leaveEscapeMenuTex->startPixel(Vector(leaveEscapeMenuTex->startPixel().X - leaveEscapeMenuTex->width() / 2, leaveEscapeMenuTex->startPixel().Y, 0));
	leaveEscapeMenuTex->mouseoverHighlight(true);
	leaveEscapeMenuButton = new GUIButton(Window, leaveEscapeMenuTex, "Close");
	Components.push_back(leaveEscapeMenuButton);
}

void EscapeMenuGUI::listen()
{
	if (escapeButtonPressListener->listen() == PRESS)
	{
		// turn off
		if (Active)
		{
			ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
			ModelLoader::pPlayerPlane->startEngine();
			active(false);
		}
		// turn on
		else
		{
			ApplicationGUI::AppGUI->ppBuffer->gaussianBlur(true);
			ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(true);
			ApplicationGUI::AppGUI->ppBuffer->shake(false);
			ModelLoader::pPlayerPlane->stopEngine();
			active(true);
		}
	}
}

