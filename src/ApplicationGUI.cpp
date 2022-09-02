#include "ApplicationGUI.h"

#include "GUIConstantQuad.h"

ApplicationGUI* ApplicationGUI::AppGUI = nullptr;

ApplicationGUI::ApplicationGUI(GLFWwindow* window) :
Window(window), loadingScreenGUI(window), startScreenGUI(new StartScreenGUI(window)), optionsGUI(new OptionsGUI(window)), ppBuffer(nullptr),
	gameplayGUI(new GameplayGUI(window)), escapeMenuGUI(new EscapeMenuGUI(window)), escapeMenuPressListener(window, GLFW_KEY_ESCAPE, KEYBOARD)
{
	AppGUI = this;
}

ApplicationGUI::~ApplicationGUI()
{
	delete ppBuffer;
}

void ApplicationGUI::draw()
{

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

	// Draw everything else

	if (gameplayGUI->active())
		gameplayGUI->draw();

	if (escapeMenuGUI->active())
		escapeMenuGUI->draw();
}

void ApplicationGUI::updateInputs(float delta)
{
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

	escapeMenuGUI->listen();

	// Update everything else
	if (gameplayGUI->active()) gameplayGUI->update(delta);
	if (escapeMenuGUI->active()) escapeMenuGUI->update(delta);
}

ApplicationGUIStatus ApplicationGUI::status()
{
	return { gameplayGUI->active(), escapeMenuGUI->active(), startScreenGUI->active(), loadingScreenGUI.active(), optionsGUI->active()};
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
