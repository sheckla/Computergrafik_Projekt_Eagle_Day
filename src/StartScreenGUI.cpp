#include "StartScreenGUI.h"

#include "GUIButton.h"
#include "GUITexture.h"
#include <sstream>
#include "ApplicationGUI.h"
#include "ApplicationSettings.h"
#include "GUISlider.h"

#include "MathUtil.h"
#include "ModelLoader.h"

StartScreenGUI::StartScreenGUI(GLFWwindow* window) : startButtonListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE),
exitButtonListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE),
ApplicationGUIPrototype(window)
{

}

StartScreenGUI::~StartScreenGUI()
{
	for (auto cloud : clouds) delete cloud;
	delete startButton;
	delete exitButton;
}

void StartScreenGUI::draw()
{
	int layer = 0;
	for (auto component : Components) {
		component->draw();
		if (layer == 1) for (auto cloud : clouds) cloud->draw();
		layer++;
	}


}

void StartScreenGUI::update(float delta)
{
	for (int i = 0; i < clouds.size(); i++)
	{
		GUITexture* cloud = clouds.at(i);
		float xOffset = 90 * delta * cloudParams[i].speed;
		float yOffset = 90 * delta * cloudParams[i].fallSpeed;
		if (cloudParams[i].fallDown) yOffset = -yOffset;

		cloud->startPixel(Vector((float)cloud->startPixel().X + xOffset, cloud->startPixel().Y + yOffset, 0));

		// Cloud wieder von links anfangen lassen
		if (cloud->startPixel().X > ApplicationSettings::WIDTH) cloud->startPixel(Vector(-cloud->width() - 20, cloud->startPixel().Y, 0));
	}

	if (startButton->mouseInside())
	{
		PRESS_STATE state = startButton->listen();
		switch (state)
		{
		case RELEASE:
			active(false);
			ApplicationGUI::AppGUI->gameplayGUI->active(true);
			ApplicationGUI::AppGUI->escapeMenuGUI->active(false);
			ModelLoader::pPlayerPlane->startEngine();
			break;
		}
	}

	if (exitButton->mouseInside() && exitButton->listen() == RELEASE)
	{
		ApplicationSettings::writeSettings();
		ApplicationSettings::SHUT_DOWN = true;
		//exit(0);
	}



	for (auto component : Components)
	{
		component->update(delta);
	}
}

void StartScreenGUI::init()
{
	int buttonWidth = 300;
	int buttonHeight = 70;
	int gap = 10;


	GUITexture* skyBG = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT / 2, new Texture(ASSETS "img/bg_color.png"), true, true);
	Components.push_back(skyBG);

	GUITexture* cloudsBG = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT / 2, new Texture(ASSETS "img/bg_clouds.png"), true, true);
	Components.push_back(cloudsBG);

	GUITexture* logoBG = new GUITexture(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT / 2, new Texture(ASSETS "img/bg_logo.png"), true, true);
	Components.push_back(logoBG);

	int offsetHeight = gap + buttonHeight;
	GUITexture* gTex = new GUITexture(ApplicationSettings::WIDTH / 2, 150, new Texture(ASSETS "img/button_simple.png"), true);
	gTex->scale(Vector(0.3, 0.3, 0));
	gTex->startPixel(Vector(gTex->startPixel().X - gTex->width() / 2, gTex->startPixel().Y, 0));
	gTex->mouseoverHighlight(true);
	startButton = new GUIButton(Window, gTex, "Start", ARIAL_BLACK, true);
	Components.push_back(startButton);

	offsetHeight += gap + buttonHeight;
	GUITexture* exitButtonTex = new GUITexture(ApplicationSettings::WIDTH / 2, 20, new Texture(ASSETS "img/button_simple_gray.png"), true);
	exitButtonTex->scale(Vector(0.3, 0.3, 0));
	exitButtonTex->startPixel(Vector(exitButtonTex->startPixel().X - exitButtonTex->width() / 2, exitButtonTex->startPixel().Y, 0));
	exitButtonTex->mouseoverHighlight(true);
	exitButton = new GUIButton(Window, exitButtonTex, "Exit", ARIAL_BLACK, true);
	Components.push_back(exitButton);




	for (int i = 0; i <= 10; i++)
	{
		std::stringstream ss;
		ss << ASSETS << "clouds/" << i % 11 << ".png";
		std::string cloudPath = ss.str();

		float startX = MathUtil::randPercentage() * ApplicationSettings::HEIGHT;
		float startY = MathUtil::randPercentage() * ApplicationSettings::HEIGHT;

		startX = MathUtil::remapBounds(startX, 0, ApplicationSettings::WIDTH, -400, ApplicationSettings::WIDTH + 400);
		startY = MathUtil::remapBounds(startY, 0, ApplicationSettings::HEIGHT, -100, ApplicationSettings::HEIGHT - 100);

		GUITexture* cloud = new GUITexture(startX, startY, new Texture(cloudPath.c_str()), true, false);
		clouds.push_back(cloud);

		CloudParams cloudParam;
		cloudParam.speed = MathUtil::randPercentage(0.1, 0.9);
		cloudParam.fallSpeed = MathUtil::randPercentage(0.05, 0.1);
		cloudParam.fallDown = MathUtil::randBool();
		cloudParams.push_back(cloudParam);
	}
}

