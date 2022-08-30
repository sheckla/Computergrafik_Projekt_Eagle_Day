#include "OptionsGUI.h"

#include "Application.h"
#include "ApplicationSettings.h"
#include "GUITexture.h"

OptionsGUI::OptionsGUI(GLFWwindow* window) : ApplicationGUIPrototype(window)
{
	cogwheelPressListener = new PressListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE);
}

OptionsGUI::~OptionsGUI()
{
}

void OptionsGUI::draw()
{
	cogwheelTexture->draw();
	if (Active) ApplicationGUIPrototype::draw();
}

void OptionsGUI::update(float delta)
{
	//if (cogwheelTexture && cogwheelTexture->mouseInside() && cogwheelPressListener->listen() == PRESS)
	//{
	//	//cogwheelTexture->mousePress(true);
	//} 

	if (cogwheelTexture && cogwheelTexture->mouseInside() && cogwheelPressListener->listen() == RELEASE)
	{
		//cogwheelTexture->mousePress(false);
		if (Active)
			// turn off
		{
			Application::testBuffer->postProcessingActive(false);
			active(false);
		}
		else
			// turn on
		{
			Application::testBuffer->gaussianBlur(true);
			Application::testBuffer->postProcessingActive(true);
			active(true);
		}
	}

	(cogwheelPressListener->pressed()) ? cogwheelTexture->mousePress(true) : cogwheelTexture->mousePress(false);

	cogwheelTexture->update(delta);
	for (auto component : Components) component->update(delta);
	audioSlider->update(delta);


	ApplicationSettings::AUDIO_VALUE = audioSlider->percentage();
}

void OptionsGUI::init()
{
	// Cogwheel clickable
	GUITexture* cogwheelTexture = new GUITexture(20, ASPECT_HEIGHT - 70, new Texture(ASSETS "img/cogwheel.png"), true);
	cogwheelTexture->constantColorMode(true);
	cogwheelTexture->mouseoverHighlight(true);
	cogwheelTexture->mouseoverHighlightColor(COL_LIGHT);
	cogwheelTexture->mousePressColor(COL_DARK);
	cogwheelTexture->width(50);
	cogwheelTexture->height(50);
	this->cogwheelTexture = cogwheelTexture;

	// Apply button
	GUITexture* applyTexture = new GUITexture(50, 100, new Texture(ASSETS "img/button_simple.png"), true);
	applyTexture->mouseoverHighlight(true);
	applyTexture->scale(Vector(0.3, 0.3, 0));
	GUIButton* applyButton = new GUIButton(ApplicationGUI::AppGUI->Window, applyTexture, "Apply");
	Components.push_back(applyButton);

	// Audio Slider
	GUISlider* audioSlider = new GUISlider(50, 300, applyTexture->width(), 75, 10);
	this->audioSlider = audioSlider;
	audioSlider->percentage(ApplicationSettings::AUDIO_VALUE);
	Components.push_back(audioSlider);
}
