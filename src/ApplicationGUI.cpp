#include "ApplicationGUI.h"

#include "GUIConstantQuad.h"

ApplicationGUI* ApplicationGUI::AppGUI = nullptr;

ApplicationGUI::ApplicationGUI(GLFWwindow* window) :
Window(window), loadingScreenGUI(window), startScreenGUI(new StartScreenGUI(window)), 
	gameplayGUI(new GameplayGUI(window)), escapeMenuGUI(new EscapeMenuGUI(window)), escapeMenuPressListener(window, GLFW_KEY_ESCAPE, KEYBOARD)
{
	AppGUI = this;
	loadingScreenGUI.init();
}

ApplicationGUI::~ApplicationGUI()
{
}

void ApplicationGUI::draw()
{
	if (loadingScreenGUI.active())
	{
		loadingScreenGUI.draw();
		return;
	}

	if (startScreenGUI->active())
	{
		startScreenGUI->draw();
		return;
	}

	if (gameplayGUI->active())
	{
		gameplayGUI->draw();
	}

	if (escapeMenuGUI->active())
	{
		escapeMenuGUI->draw();
	}
}

void ApplicationGUI::updateInputs(float delta)
{
	if (loadingScreenGUI.active()) {
		loadingScreenGUI.update(delta);
		return;
	} else
	{
		
	}
	if (startScreenGUI->active()) startScreenGUI->update(delta);
	if (gameplayGUI->active()) gameplayGUI->update(delta);
	if (escapeMenuGUI->active()) escapeMenuGUI->update(delta);
	PRESS_STATE state = escapeMenuPressListener.listen();
	switch (state)
	{
	case PRESS:
		(status().escapeMenu) ? escapeMenuGUI->active(false) : escapeMenuGUI->active(true);
		break;
	}

	(status().escapeMenu) ? escapeMenuGUI->postProcessingEffects(true) : escapeMenuGUI->postProcessingEffects(false);
	if (escapeMenuGUI->postProcessingInit()) escapeMenuGUI->elapseTime(delta);
	if (!escapeMenuGUI->postProcessingInit()) escapeMenuGUI->elapseTime(-delta);
}

ApplicationGUIStatus ApplicationGUI::status()
{
	return { gameplayGUI->active(), escapeMenuGUI->active(), startScreenGUI->active(), loadingScreenGUI.active()};
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
