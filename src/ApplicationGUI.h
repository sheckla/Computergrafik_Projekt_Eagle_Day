/*
 * ApplicationGUI
 * - Hauptsteuerungsklasse fuer alle GUI-technische Aspekte der Anwendung
 * - beinhaltet als Instanze alle Instanzen der anderen GUI-Typen
 * - Schaltet die Freigabe von den Anderen GUIS
 * - Legt fest, wann welche GUI's gezeichnet werden sollen
 */

#ifndef ApplicationGUI_h
#define ApplicationGUI_h

#include "EscapeMenuGUI.h"
#include "GameplayGUI.h"
#include "LoadingScreenGUI.h"
#include "OptionsGUI.h"
#include "PostProcessingBuffer.h"
#include "PressListener.h"
#include "StartScreenGUI.h"

const enum APPLICATION_GUI_TYPES { LOADING_SCREEN_GUI, STARTSCREEN_GUI, GAMEPLAY_GUI, ESCAPE_MENU_GUI };

struct ApplicationGUIStatus
{
	ApplicationGUIStatus(bool active, bool active1, bool active2, bool active3, bool active4) :
	gameplay(active), escapeMenu(active1), startscreenGUI(active2), loadingScreen(active3), optionsGUI(active4) {}
	bool gameplay;
	bool escapeMenu;
	bool startscreenGUI;
	bool loadingScreen;
	bool optionsGUI;
};

class ApplicationGUI
{
	LoadingScreenGUI loadingScreenGUI;
	PressListener escapeMenuPressListener;
public:
	static ApplicationGUI* AppGUI;
	PostProcessingBuffer* ppBuffer;
	GLFWwindow* Window;

	StartScreenGUI* startScreenGUI;
	GameplayGUI* gameplayGUI;
	EscapeMenuGUI* escapeMenuGUI;
	OptionsGUI* optionsGUI;

	ApplicationGUI(GLFWwindow* window);
	~ApplicationGUI();

	void draw();
	void updateInputs(float delta);

	void setGUIStatus(APPLICATION_GUI_TYPES guiState, bool active);
	void attachPostProcessingBuffer(PostProcessingBuffer* ppBuffer);
	ApplicationGUIStatus status();
	void drawOptions();
};

#endif;