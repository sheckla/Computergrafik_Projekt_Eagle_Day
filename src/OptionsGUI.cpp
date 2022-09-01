#include "OptionsGUI.h"

#include "Application.h"
#include "ApplicationSettings.h"
#include "GUITexture.h"

OptionsGUI::OptionsGUI(GLFWwindow* window) : ApplicationGUIPrototype(window)
{
	cogwheelPressListener = new PressListener(window, GLFW_MOUSE_BUTTON_LEFT, MOUSE);
	escapeButtonPressListener = new PressListener(window, GLFW_KEY_ESCAPE, KEYBOARD);
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

	if (cogwheelTexture && cogwheelTexture->mouseInside() && cogwheelPressListener->listen() == RELEASE)
	{
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


	if (escapeButtonPressListener->listen() == RELEASE) {
		Application::testBuffer->postProcessingActive(false);
		active(false);
	}
	if (!Active) return;



	for (auto component : Components) component->update(delta);
	audioSlider->update(delta);


	ApplicationSettings::AUDIO_VALUE = audioSlider->percentage();
	ApplicationSettings::MOUSE_CONTROLS = mouseControlSwitch->on();
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
	//Components.push_back(applyButton);

	// Audio Slider
	GUISlider* audioSlider = new GUISlider(50, 300, applyTexture->width(), 75, 10, "Audio");
	this->audioSlider = audioSlider;
	audioSlider->percentage(ApplicationSettings::AUDIO_VALUE);
	Components.push_back(audioSlider);

	mouseControlSwitch = new GUIButtonSwitch(50, 400, applyTexture->width(), 75);
	Components.push_back(mouseControlSwitch);
}
