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

	GUITexture* exitButtonTex = new GUITexture(ApplicationSettings::WIDTH / 2, 400, new Texture(ASSETS "img/button_simple_gray.png"), true);
	exitButtonTex->scale(Vector(0.3, 0.3, 0));
	exitButtonTex->startPixel(Vector(exitButtonTex->startPixel().X - exitButtonTex->width() / 2, exitButtonTex->startPixel().Y, 0));
	exitButtonTex->mouseoverHighlight(true);
	returnButton = new GUIButton(Window, exitButtonTex, "Restart", ARIAL_BLACK, true);
	Components.push_back(returnButton);

	GUITexture* gameOver = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 300, new Texture(ASSETS "img/gameover.png"), true);
	gameOver->centred(true);
	Components.push_back(gameOver);
}

void GameOverGUI::update(float delta)
{
	if (ModelLoader::pPlayerPlane && ModelLoader::pPlayerPlane->hp <= 0) active(true);
	if (!Active) return;
	ApplicationGUI::AppGUI->gameplayGUI->active(false);
	ModelLoader::pPlayerPlane->stopEngine();

	if (returnButton->mouseInside() && returnButton->listen() == RELEASE)
	{
		ModelLoader::pPlayerPlane->hp = 100;
		if(ModelLoader::pEnemyPlane)ModelLoader::pEnemyPlane->hp = 100;
		if (ModelLoader::pAIPlane) ModelLoader::pAIPlane->hp = 100;
		ModelLoader::pPlayerPlane->initModelTranslation();
		active(false);
		ApplicationGUI::AppGUI->escapeMenuGUI->active(false);
		ApplicationGUI::AppGUI->optionsGUI->active(false);
		ApplicationGUI::AppGUI->gameplayGUI->active(true);
		ModelLoader::pPlayerPlane->startEngine();
	}
	for (auto component : Components) component->update(delta);
}
