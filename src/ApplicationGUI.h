/*
 * ApplicationGUI
 * - Hauptsteuerungsklasse fuer alle GUI-technische Aspekte der Anwendung
 * - beinhaltet als Instanz alle Instanzen der anderen GUI-Typen
 * - Schaltet die Freigabe von den Anderen GUIS
 * - Legt fest, wann welche GUI's wann gezeichnet werden sollen
 */

#ifndef ApplicationGUI_h
#define ApplicationGUI_h

#include "EscapeMenuGUI.h"
#include "GameOverGUI.h"
#include "GameplayGUI.h"
#include "LoadingScreenGUI.h"
#include "OptionsGUI.h"
#include "PostProcessingBuffer.h"
#include "PressListener.h"
#include "StartScreenGUI.h"

const enum APPLICATION_GUI_TYPES { LOADING_SCREEN_GUI, STARTSCREEN_GUI, GAMEPLAY_GUI, ESCAPE_MENU_GUI };

struct ApplicationGUIStatus
{
	ApplicationGUIStatus(bool active, bool active1, bool active2, bool active3, bool active4, bool active5) :
	gameplay(active), escapeMenu(active1), startscreenGUI(active2),
	loadingScreen(active3), optionsGUI(active4), gameoverGUI(active5) {}
	bool gameplay;
	bool escapeMenu;
	bool startscreenGUI;
	bool loadingScreen;
	bool optionsGUI;
	bool gameoverGUI;
};

class ApplicationGUI
{
	LoadingScreenGUI loadingScreenGUI;
	PressListener escapeMenuPressListener;
public:
	static ApplicationGUI* AppGUI; // statische Instanz fuer globale Aufrufe

	// Public fuer globale Aufrufe
	GLFWwindow* Window;
	StartScreenGUI* startScreenGUI;
	GameplayGUI* gameplayGUI;
	EscapeMenuGUI* escapeMenuGUI;
	OptionsGUI* optionsGUI;
	GameOverGUI* gameOverGUI;
	PostProcessingBuffer* ppBuffer;

	ApplicationGUI(GLFWwindow* window);
	~ApplicationGUI();

	void draw();
	void updateInputs(float delta);

	void setGUIStatus(APPLICATION_GUI_TYPES guiState, bool active);
	void attachPostProcessingBuffer(PostProcessingBuffer* ppBuffer);
	ApplicationGUIStatus status();

	// OptionGUI wird mittels Buffer gezeichnet deshalb ausserhalb als eigene Funktion
	void drawOptions();
};

#endif;