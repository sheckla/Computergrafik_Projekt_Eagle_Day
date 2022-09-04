#include "ApplicationGUI.h"
#include "GUIConstantQuad.h"
#include "ModelLoader.h"

ApplicationGUI* ApplicationGUI::AppGUI = nullptr;

ApplicationGUI::ApplicationGUI(GLFWwindow* window) :
Window(window), loadingScreenGUI(window), startScreenGUI(new StartScreenGUI(window)), optionsGUI(new OptionsGUI(window)), ppBuffer(nullptr),
	gameplayGUI(new GameplayGUI(window)), escapeMenuGUI(new EscapeMenuGUI(window)), gameOverGUI(new GameOverGUI(window)),
escapeMenuPressListener(window, GLFW_KEY_ESCAPE, KEYBOARD)
{
	AppGUI = this;
}

ApplicationGUI::~ApplicationGUI()
{
	print("ApplicationGUI", "Deleting");
	delete startScreenGUI;
	delete gameplayGUI;
	delete escapeMenuGUI;
	delete optionsGUI;
	delete gameOverGUI;
	delete ppBuffer;
}

void ApplicationGUI::draw()
{
	// Only Gameover screen if active
	if (gameOverGUI->active())
	{
		gameOverGUI->draw();
		return;
	}

	// Only LoadingScreen if active
	if (loadingScreenGUI.active())
	{
		loadingScreenGUI.draw();
		return;
	}

	// Only StartScreen if active
	if (startScreenGUI->active())
	{
		startScreenGUI->draw();
		return;
	}

	if (gameplayGUI->active() && ModelLoader::pPlayerPlane && ModelLoader::pPlayerPlane->hp > 0)
		gameplayGUI->draw();

	if (escapeMenuGUI->active())
		escapeMenuGUI->draw();
}

void ApplicationGUI::updateInputs(float delta)
{
	if (ModelLoader::pPlayerPlane && ModelLoader::pPlayerPlane->hp <= 0) {
		gameplayGUI->active(false);
		gameOverGUI->active(true);
		ModelLoader::pPlayerPlane->stopEngine();
	}

	// Only update LoadingScreen if active
	if (loadingScreenGUI.active()) {
		loadingScreenGUI.update(delta);
		return;
	}

	optionsGUI->update(delta);
	if (optionsGUI->active()) return;
	if (startScreenGUI->active())
	{
		startScreenGUI->update(delta);
		return;
	}

	if (gameplayGUI->active()) gameplayGUI->update(delta);
	escapeMenuGUI->listen();
	if (escapeMenuGUI->active()) escapeMenuGUI->update(delta);
	if (gameOverGUI->active()) gameOverGUI->update(delta);

}

ApplicationGUIStatus ApplicationGUI::status()
{
	return { gameplayGUI->active(), escapeMenuGUI->active(), startScreenGUI->active(),
		loadingScreenGUI.active(), optionsGUI->active(), gameOverGUI->active()};
}

void ApplicationGUI::drawOptions()
{
	optionsGUI->draw();
}

void ApplicationGUI::setGUIStatus(APPLICATION_GUI_TYPES guiState, bool active)
{
	switch (guiState)
	{
	case LOADING_SCREEN_GUI:
		loadingScreenGUI.active(active);
		break;
	case STARTSCREEN_GUI:
		startScreenGUI->active(active);
		break;
	case GAMEPLAY_GUI:
		gameplayGUI->active(active);
		break;
	case ESCAPE_MENU_GUI:
		escapeMenuGUI->active(active);
		break;
	}
}

void ApplicationGUI::attachPostProcessingBuffer(PostProcessingBuffer* ppBuffer)
{
	this->ppBuffer = ppBuffer;
	escapeMenuGUI->attachPostProcessingBuffer(ppBuffer);
}
