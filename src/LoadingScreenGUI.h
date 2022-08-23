/*
 * LoadingScreenGUI
 * - wird bei Start der Anwendung angezeigt
 * - ist fuer das Laden der relevanten Anwendungskomponenten zustaendig
 * - Zentrale Instanzen und Komponenten der Anwendung werden hier initialisiert und zusammengefuehrt
 */

#ifndef LoadingScreenGUI_h
#define LoadingScreenGUI_h

#include "Globals.h"
#include "ApplicationGUIPrototype.h"
#include "GUIChar.h"
#include "GUILoadingMeter.h"
#include "GUIText.h"

constexpr int tasks = 12;

const enum LOADING_TASKS
{
	GUI_LOADING,
	GUI_STARTSCREEN,
	GUI_GAMEPLAY,
	GUI_ESCAPEMENU,
	GUI_FINALIZE,

	MODELS_LIGHT,
	MODELS_SKYBOX,
	MODELS_WATER,
	MODELS_PLANEPARTS,
	MODELS_CLOUDS,
	MODELS_ONLINE,

	MODELS_GUI_FINALIZE,
};

class LoadingScreenGUI : public ApplicationGUIPrototype
{
	int currentTask = 0;
	GUILoadingMeter* loadingMeter;
	GUIText* text;
public:
	LoadingScreenGUI(GLFWwindow* window);
	~LoadingScreenGUI() override;
	void update(float delta) override;
	void init() override;
	void draw() override;
};

#endif