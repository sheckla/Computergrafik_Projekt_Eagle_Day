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

	if (Active) {
		ApplicationGUIPrototype::draw();

		// Only display info in ONLINE_MODE
		if (ApplicationSettings::ONLINE_MODE)
		{
			localIP->draw();
			localPort->draw();
			enemyIP->draw();
			enemyPort->draw();
		}

		if (restartChangeRequired)
		{
			restartChangeRequiredText->draw();
		}
	}
}

void OptionsGUI::update(float delta)
{

	if (cogwheelTexture && cogwheelTexture->mouseInside() && cogwheelPressListener->listen() == RELEASE)
	{
		if (Active)
			// turn off
		{
			Application::guiBuffer->postProcessingActive(false);
			active(false);
		}
		else
			// turn on
		{
			Application::guiBuffer->gaussianBlur(true);
			Application::guiBuffer->postProcessingActive(true);
			active(true);
		}
	}
	(cogwheelPressListener->pressed()) ? cogwheelTexture->mousePress(true) : cogwheelTexture->mousePress(false);
	cogwheelTexture->update(delta);

	// Escape Button Pressed -> close Options
	if (escapeButtonPressListener->listen() == RELEASE) {
		Application::guiBuffer->postProcessingActive(false);
		active(false);
	}
	if (!Active) return;

	for (auto component : Components) component->update(delta);

	// Only display/update in ONLINE_MODE
	if (ApplicationSettings::ONLINE_MODE)
	{
		localIP->update(delta);
		localPort->update(delta);
		enemyIP->update(delta);
		enemyPort->update(delta);
	}
	audioSlider->update(delta);

	// RestartRequired Condition check
	((initial_FULL_SCREEN != ApplicationSettings::FULL_SCREEN) || (initial_ONLINE_MODE != ApplicationSettings::ONLINE_MODE)) ? restartChangeRequired = true :
		restartChangeRequired = false;

	// Save to ApplicationSettings
	ApplicationSettings::AUDIO_VALUE = audioSlider->percentage();
	ApplicationSettings::MOUSE_CONTROLS = mouseControlSwitch->on();
	ApplicationSettings::FULL_SCREEN = fullscreenControlSwitch->on();
	ApplicationSettings::ONLINE_MODE = onlineModeControlSwitch->on();
	ApplicationSettings::SEPIA_POST_PROCESSING = sepiaPostProcessingSwitch->on();

}

void OptionsGUI::init()
{
	// Cogwheel clickable - top left
	GUITexture* cogwheelTexture = new GUITexture(20, ApplicationSettings::HEIGHT - 70, new Texture(ASSETS "img/cogwheel.png"), true);
	cogwheelTexture->constantColorMode(true);
	cogwheelTexture->mouseoverHighlight(true);
	cogwheelTexture->mouseoverHighlightColor(COL_LIGHT);
	cogwheelTexture->mousePressColor(COL_DARK);
	cogwheelTexture->width(50);
	cogwheelTexture->height(50);
	this->cogwheelTexture = cogwheelTexture;

	// Button Width
	GUITexture* applyTexture = new GUITexture(50, 100, new Texture(ASSETS "img/button_simple.png"), true);
	applyTexture->scale(Vector(0.3, 0.3, 0));

	// --- Left Side --- 
	// PostProcessingSwitch
	sepiaPostProcessingSwitch = new GUIButtonSwitch(50, 500, applyTexture->width(), 75, "Color Filter");
	sepiaPostProcessingSwitch->on(ApplicationSettings::SEPIA_POST_PROCESSING);
	Components.push_back(sepiaPostProcessingSwitch);

	// Mouse Control Switch
	mouseControlSwitch = new GUIButtonSwitch(50, 400, applyTexture->width(), 75, "Mousesteer");
	mouseControlSwitch->on(ApplicationSettings::MOUSE_CONTROLS);
	Components.push_back(mouseControlSwitch);

	// Audio Slider
	GUISlider* audioSlider = new GUISlider(50, 300, applyTexture->width(), 75, 10, "Audio");
	this->audioSlider = audioSlider;
	audioSlider->percentage(ApplicationSettings::AUDIO_VALUE);
	Components.push_back(audioSlider);

	// --- Right Side --- 
	// Online Info Text
	std::string locIP{ "Local IP:" + ApplicationSettings::LOCAL_IP };
	std::string locPort{ "Local Port:" + ApplicationSettings::LOCAL_PORT };
	std::string enemIP{ "Enemy IP:" + ApplicationSettings::ENEMY_IP };
	std::string enemPort{ "Enemy Port:" + ApplicationSettings::ENEMY_PORT};
	localIP = new GUIText(ApplicationSettings::WIDTH - 590, ApplicationSettings::HEIGHT - 50, locIP.c_str());
	localPort = new GUIText(ApplicationSettings::WIDTH - 590, ApplicationSettings::HEIGHT - 80, locPort.c_str());
	enemyIP = new GUIText(ApplicationSettings::WIDTH - 590, ApplicationSettings::HEIGHT - 110, enemIP.c_str());
	enemyPort = new GUIText(ApplicationSettings::WIDTH - 590, ApplicationSettings::HEIGHT - 140, enemPort.c_str());

	// Online Mode Switch
	onlineModeControlSwitch = new GUIButtonSwitch(ApplicationSettings::WIDTH - 450, 500, applyTexture->width(), 75, "Online Mode");
	onlineModeControlSwitch->on(ApplicationSettings::ONLINE_MODE);
	Components.push_back(onlineModeControlSwitch);

	// Full Screen Switch
	fullscreenControlSwitch = new GUIButtonSwitch(ApplicationSettings::WIDTH - 450, 400, applyTexture->width(), 75, "Fullscreen");
	fullscreenControlSwitch->on(ApplicationSettings::FULL_SCREEN);
	Components.push_back(fullscreenControlSwitch);

	// Need to restart Text
	restartChangeRequiredText = new GUIText(ApplicationSettings::WIDTH - 450, 300, "(Restart required)");

	// clear
	delete applyTexture;
}
