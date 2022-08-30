#include "LoadingScreenGUI.h"

#include <sstream>

#include "Application.h"
#include "ApplicationGUI.h"
#include "GUIChar.h"
#include "GUILoadingMeter.h"
#include "GUITexture.h"
#include "GUITexture.h"
#include "ModelLoader.h"

std::string LoadingScreenGUI::stringifyTask(const char* taskText)
{
	std::stringstream ss;
	ss << "[" << currentTask << "/" << tasks << "] " << taskText;
	return ss.str();
}

void LoadingScreenGUI::printLoadStartText(const char* text)
{
	print(text, "Loading start.");
}

void LoadingScreenGUI::printLoadFinishText(const char* text)
{
	print(text, "Loading finished!");
	printDivider();
}

LoadingScreenGUI::LoadingScreenGUI(GLFWwindow* window) : ApplicationGUIPrototype(window)
{
}

LoadingScreenGUI::~LoadingScreenGUI()
{
}

void LoadingScreenGUI::update(float delta)
{
	if (currentTask >= tasks) return;
	LOADING_TASKS task = (LOADING_TASKS)currentTask;

	switch (task)
	{
		// GUI
	case GUI_LOADING:
		printLoadStartText("LOADING_SCREEN_GUI");

		FNTManager::instance().loadFont(ASSETS "typography/arial/arial.fnt",
			ASSETS "typography/arial/arial.png", FONT_NAMES::ARIAL);
		FNTManager::instance().loadFont(ASSETS "typography/ww2/ww2.fnt",
			ASSETS "typography/ww2/ww2.png", FONT_NAMES::WW2);
		FNTManager::instance().loadFont(ASSETS "typography/typewriter/typewriter.fnt",
			ASSETS "typography/typewriter/typewriter.png", FONT_NAMES::TYPEWRITER);
		FNTManager::instance().loadFont(ASSETS "typography/arial_black/black.fnt",
			ASSETS "typography/arial_black/black.png", FONT_NAMES::ARIAL_BLACK);

		init();
		loadingProgressText->text(stringifyTask("Loading LoadingScreenGUI").c_str());

		printLoadFinishText("LOADING_SCREEN_GUI");
		break;
		// ----
	case GUI_STARTSCREEN:
		printLoadStartText("STARTSCREEN_GUI");

		loadingProgressText->text(stringifyTask("Loading StartScreenGUI").c_str());
		ApplicationGUI::AppGUI->startScreenGUI->init();

		printLoadFinishText("STARTSCREEN_GUI");
		break;
		// ----
	case GUI_GAMEPLAY:
		printLoadStartText("GAMEPLAY_GUI");

		loadingProgressText->text(stringifyTask("Loading GameplayGUI").c_str());
		ApplicationGUI::AppGUI->gameplayGUI->init();

		printLoadFinishText("GAMEPLAY_GUI");
		break;
		// ----
	case GUI_ESCAPEMENU:
		printLoadStartText("ESCAPEMENU_GUI");

		loadingProgressText->text(stringifyTask("Loading EscapeMenuGUI").c_str());
		ApplicationGUI::AppGUI->escapeMenuGUI->init();

		printLoadFinishText("ESCAPEMENU_GUI");
		break;
		// ----
	case GUI_OPTIONS:
		printLoadStartText("OPTIONS_GUI");

		ApplicationGUI::AppGUI->optionsGUI->init();

		printLoadFinishText("OPTIONS_GUI");
		break;
		// ----
	case GUI_FINALIZE:
		printLoadStartText("GUI_FINALIZE");

		loadingProgressText->text(stringifyTask("Finalizing GUI").c_str());
		ApplicationGUI::AppGUI->attachPostProcessingBuffer(new PostProcessingBuffer(ASPECT_WIDTH, ASPECT_HEIGHT));

		printLoadFinishText("GUI_FINALIZE");
		break;

		// MODELS
	case MODELS_LIGHT:
		printLoadStartText("DIR_LIGHT");

		loadingProgressText->text(stringifyTask("Loading DirLight").c_str());
		ModelLoader::dirLight();

		printLoadFinishText("DIR_LIGHT");
		break;
		// ----
	case MODELS_SKYBOX:
		printLoadStartText("SKYBOX");

		loadingProgressText->text(stringifyTask("Loading SkyBox").c_str());
		ModelLoader::skyBox();

		printLoadFinishText("SKYBOX");
		break;
		// ----
	case MODELS_PLANEPARTS:
		printLoadStartText("PLANE_PARTS");

		loadingProgressText->text(stringifyTask("Loading PlayerPlane").c_str());
		ModelLoader::planeParts();
		if (APPLICATION_ONLINE_MODE)
		{
			ModelLoader::planePartsOnline("127.0.0.1", 19411);
			Application::enemyPlane = ModelLoader::enemyPlane("127.0.0.1", 19413);
		}
		ModelLoader::planePartsShadowArea();
//		ModelLoader::aiPlaneParts();

		printLoadFinishText("PLANE_PARTS");
		break;
		// ----
	case MODELS_CLOUDS:
		printLoadStartText("VOLUMETRIC_CLOUDS");

		loadingProgressText->text(stringifyTask("Loading Clouds").c_str());
		ModelLoader::clouds();

		printLoadFinishText("VOLUMETRIC_CLOUDS");

		// Hier schon aufrufen da Ocean ueber Threads laeuft
		loadingProgressText->text(stringifyTask("Loading Ocean").c_str());

		break;
		// ----
	case MODELS_WATER:
		printLoadStartText("OCEAN");

		ModelLoader::ocean();

		printLoadFinishText("OCEAN");
		break;

		// FINALIZE
	case MODELS_GUI_FINALIZE:
		printLoadStartText("MODELS_GUI_FINALIZE");

		loadingProgressText->text(stringifyTask("Preparing Application start...").c_str());
		Application::Cam = new Camera(ApplicationGUI::AppGUI->Window);
		Application::planeControls = new PlayerPlaneControls(ApplicationGUI::AppGUI->Window, ModelLoader::pPlayerPlane, Application::Cam , true);
		ApplicationGUI::AppGUI->startScreenGUI->active(true);
		active(false);

		printLoadFinishText("MODELS_GUI_FINALIZE");
		break;
	}
	currentTask++;
	float taskPercentage = (float)currentTask / tasks;
	loadingMeter->percentage(taskPercentage);
	loadingMeter->update(delta);
}

void LoadingScreenGUI::init()
{
	// Hintergrundbild
	GUITexture* backgroundImageGUI = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2, new Texture(ASSETS "img/bg_loading.png"), true, true);
	Components.push_back(backgroundImageGUI);

	// Lademeter
	loadingMeter = new GUILoadingMeter(20, 20, ASPECT_WIDTH - 40, 55, 10);
	Components.push_back(loadingMeter);

	GUIText* pLoadingProgressText = new GUIText(20, 85, "Preparing to Load Application...       ");
	this->loadingProgressText = pLoadingProgressText;
	Components.push_back(pLoadingProgressText);

	GUIText* topLeftTextInfo = new GUIText(20, ASPECT_HEIGHT-70, "[EAGLE DAY - UNRELEASED ]");
	Components.push_back(topLeftTextInfo);

	active(true); // GUI wird gezeichnet
}

void LoadingScreenGUI::draw()
{
	for (auto component : Components) component->draw();
}
