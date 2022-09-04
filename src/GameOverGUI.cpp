#include "GameOverGUI.h"

#include "ApplicationGUI.h"
#include "ApplicationSettings.h"
#include "FNTManager.h"
#include "GUIButton.h"
#include "GUITexture.h"
#include "ModelLoader.h"

GameOverGUI::GameOverGUI(GLFWwindow* window) : ApplicationGUIPrototype(window)
{

}

GameOverGUI::~GameOverGUI()
{
}

void GameOverGUI::init()
{
	// Restart button
	GUITexture* exitButtonTex = new GUITexture(ApplicationSettings::WIDTH / 2, 400, new Texture(ASSETS "img/button_simple_gray.png"), true);
	exitButtonTex->scale(Vector(0.3, 0.3, 0));
	exitButtonTex->startPixel(Vector(exitButtonTex->startPixel().X - exitButtonTex->width() / 2, exitButtonTex->startPixel().Y, 0));
	exitButtonTex->mouseoverHighlight(true);
	returnButton = new GUIButton(Window, exitButtonTex, "Restart", ARIAL_BLACK, true);
	Components.push_back(returnButton);

	// Gameover Image
	GUITexture* gameOver = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 300, new Texture(ASSETS "img/gameover.png"), true);
	gameOver->centred(true);
	Components.push_back(gameOver);
}

void GameOverGUI::update(float delta)
{
	ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(true);
	ApplicationGUI::AppGUI->ppBuffer->hp(0);
	if (returnButton->mouseInside() && returnButton->listen() == RELEASE)
	{
		// Reset game state
		ModelLoader::pPlayerPlane->hp = 100;
		if(ModelLoader::pEnemyPlane)ModelLoader::pEnemyPlane->hp = 100;
		if (ModelLoader::pAIPlane) ModelLoader::pAIPlane->hp = 100;
		ModelLoader::pPlayerPlane->initModelTranslation();
		ApplicationGUI::AppGUI->escapeMenuGUI->active(false);
		ApplicationGUI::AppGUI->optionsGUI->active(false);
		ApplicationGUI::AppGUI->gameplayGUI->active(true);
		ModelLoader::pPlayerPlane->startEngine();
		if (ModelLoader::pAIPlane) ModelLoader::pAIPlane->initModelTranslation();
		active(false);
	}
	for (auto component : Components) component->update(delta);
}
