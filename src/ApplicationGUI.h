#ifndef ApplicationGUI_h
#define ApplicationGUI_h

#include "EscapeMenuGUI.h"
#include "GameplayGUI.h"
#include "LoadingScreenGUI.h"
#include "PostProcessingBuffer.h"
#include "PressListener.h"
#include "StartScreenGUI.h"

const enum APPLICATION_GUI_TYPES { LOADING_SCREEN_GUI, STARTSCREEN_GUI, GAMEPLAY_GUI, ESCAPE_MENU_GUI };

struct ApplicationGUIStatus
{
	ApplicationGUIStatus(bool active, bool active1, bool active2, bool active3) : gameplay(active), escapeMenu(active1), startscreenGUI(active2), loadingScreen(active3) {}
	bool gameplay;
	bool escapeMenu;
	bool startscreenGUI;
	bool loadingScreen;
};

class ApplicationGUI
{
	LoadingScreenGUI loadingScreenGUI;
	PressListener escapeMenuPressListener;
	static ApplicationGUI* pApplicationGUI;
public:
	PostProcessingBuffer* ppBuffer;
	static ApplicationGUI* AppGUI;
	GLFWwindow* Window;
	StartScreenGUI* startScreenGUI;
	GameplayGUI* gameplayGUI;
	EscapeMenuGUI* escapeMenuGUI;
	ApplicationGUI(GLFWwindow* window);
	~ApplicationGUI();

	void setGUIStatus(APPLICATION_GUI_TYPES guiState, bool active);
	void draw();
	void updateInputs(float delta);
	void attachPostProcessingBuffer(PostProcessingBuffer* ppBuffer);
	ApplicationGUIStatus status();
};

#endif;