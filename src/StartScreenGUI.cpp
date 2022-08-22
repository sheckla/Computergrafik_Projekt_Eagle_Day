#include "StartScreenGUI.h"

#include "GUIButton.h"
#include "GUITexture.h"
#include <sstream>
#include "ApplicationGUI.h"

#include "MathUtil.h"

StartScreenGUI::StartScreenGUI(GLFWwindow* window) : startButtonListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE), ApplicationGUIPrototype(window)
{

	return;
}

StartScreenGUI::~StartScreenGUI()
{
}

void StartScreenGUI::draw()
{
	if (startButton->mouseInside())
	{
		PRESS_STATE state = startButton->listen();
		switch (state)
		{
		case RELEASE:
			active(false);
			ApplicationGUI::AppGUI->gameplayGUI->active(true);
			break;
		}
	}

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
		if (cloud->startPixel().X > ASPECT_WIDTH) cloud->startPixel(Vector(-cloud->width() - 20, cloud->startPixel().Y, 0));
	}
}

void StartScreenGUI::init()
{
	int buttonWidth = 300;
	int buttonHeight = 70;
	int gap = 10;

	GUITexture* skyBG = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2, new Texture(ASSETS "bg_color_4k.png"), true, true);
	Components.push_back(skyBG);

	GUITexture* cloudsBG = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2, new Texture(ASSETS "bg_clouds_4k.png"), true, true);
	Components.push_back(cloudsBG);

	GUITexture* logoBG = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2, new Texture(ASSETS "bg_logo_4k.png"), true, true);
	Components.push_back(logoBG);

	int offsetHeight = gap + buttonHeight;
	GUITexture* gTex = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT / 2 - offsetHeight * 4, new Texture(ASSETS "button.png"), true);
	gTex->centred(true);
	gTex->scale(Vector(0.3, 0.3, 0));
	gTex->mouseoverHighlight(true);

	startButton = new GUIButton(Window, gTex);
	Components.push_back(startButton);

	for (int i = 0; i <= 10; i++)
	{
		std::stringstream ss;
		ss << ASSETS << "clouds/" << i % 11 << ".png";
		std::string cloudPath = ss.str();

		float startX = MathUtil::randPercentage() * ASPECT_WIDTH;
		float startY = MathUtil::randPercentage() * ASPECT_HEIGHT;

		startX = MathUtil::remapBounds(startX, 0, ASPECT_WIDTH, -400, ASPECT_WIDTH + 400);
		startY = MathUtil::remapBounds(startY, 0, ASPECT_HEIGHT, -100, ASPECT_HEIGHT - 100);

		GUITexture* cloud = new GUITexture(startX, startY, new Texture(cloudPath.c_str()), true, false);
		clouds.push_back(cloud);

		CloudParams cloudParam;
		cloudParam.speed = MathUtil::randPercentage(0.1, 0.9);
		cloudParam.fallSpeed = MathUtil::randPercentage(0.05, 0.1);
		cloudParam.fallDown = MathUtil::randBool();
		cloudParams.push_back(cloudParam);
	}
}

