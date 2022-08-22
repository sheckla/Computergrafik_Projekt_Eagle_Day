#include "LoadingScreenGUI.h"

#include "Application.h"
#include "ApplicationGUI.h"
#include "GUIChar.h"
#include "GUILoadingMeter.h"
#include "GUIText.h"
#include "GUITexture.h"
#include "ModelLoader.h"

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
	case GUI_LOADING:
		init();
		break;
	case GUI_STARTSCREEN:
		print("STARTSCREEN", "");
		text->text("Loading StartscreenMenu");
		ApplicationGUI::AppGUI->startScreenGUI->init();
		break;
	case GUI_GAMEPLAY:
		print("GUI", "");
		text->text("Loading GameplayMenu");
		ApplicationGUI::AppGUI->gameplayGUI->init();
		break;
	case GUI_ESCAPEMENU:
		print("ESCAPE", "");
		text->text("Loading EscapeMenu");
		ApplicationGUI::AppGUI->escapeMenuGUI->init();
		break;
	case GUI_FINALIZE:
		text->text("Finalizing GUI");
		ApplicationGUI::AppGUI->attachPostProcessingBuffer(new PostProcessingBuffer(ASPECT_WIDTH, ASPECT_HEIGHT));
		break;
	case MODELS_LIGHT:
		print("DIR", "");
		text->text("Loading DirLight");
		ModelLoader::loadDirLight();
		break;
	case MODELS_SKYBOX:
		text->text("Loading SkyBox");
		ModelLoader::loadSkyBox();
		break;
	case MODELS_SIMPLEWATER:
		text->text("Loading Water");
		ModelLoader::loadSimpleWater();
		break;
	case MODELS_PLANEPARTS:
		text->text("Loading PlayerPlane");
		ModelLoader::loadPlaneParts();
		break;
	case MODELS_CLOUDS:
		text->text("Loading Clouds");
		ModelLoader::clouds();
		break;
	case MODELS_GUI_FINALIZE:
		text->text("Preparing Start...");
		Application::Cam = new Camera(ApplicationGUI::AppGUI->Window);
		Application::planeControls = new PlayerPlaneControls(ApplicationGUI::AppGUI->Window, ModelLoader::pPlayerPlane, Application::Cam , true);
		ApplicationGUI::AppGUI->startScreenGUI->active(true);
		active(false);
		break;
	}
	currentTask++;
	float taskPercentage = (float)currentTask / tasks;
	loadingMeter->percentage(taskPercentage);
}

void LoadingScreenGUI::init()
{
	print("Loading Unit", "start");
	GUITexture* bg = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2, new Texture(ASSETS "bg_color_4k.png"), true, true);
	Components.push_back(bg);

	loadingMeter = new GUILoadingMeter(ASPECT_WIDTH/2 - 400, 20, 55, 800);
	Components.push_back(loadingMeter);

	GUIText* text = new GUIText(ASPECT_WIDTH/2- 400, 90, "Hello World!!! IT WOOORKS");
	this->text = text;
	Components.push_back(text);

	active(true);
}

void LoadingScreenGUI::draw()
{
	for (auto component : Components) component->draw();
}
