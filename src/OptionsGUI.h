#ifndef OptionsGUI_h
#define OptionsGUI_h
#include "ApplicationGUIPrototype.h"
#include "ApplicationSettings.h"
#include "GUIButtonSwitch.h"
#include "GUISlider.h"
#include "GUITexture.h"
#include "PressListener.h"

class OptionsGUI : public ApplicationGUIPrototype
{
private:
	GUITexture* cogwheelTexture = nullptr;
	PressListener* cogwheelPressListener;

	PressListener* escapeButtonPressListener;

	GUISlider* audioSlider;
	GUIButtonSwitch* mouseControlSwitch;
	GUIButtonSwitch* fullscreenControlSwitch;
	GUIButtonSwitch* onlineModeControlSwitch;
	GUIButtonSwitch* sepiaPostProcessingSwitch;

	GUIText* localIP;
	GUIText* localPort;
	GUIText* enemyIP;
	GUIText* enemyPort;

	GUIText* restartChangeRequiredText;
	bool restartChangeRequired = false;

	bool initial_FULL_SCREEN = ApplicationSettings::FULL_SCREEN;
	bool initial_ONLINE_MODE = ApplicationSettings::ONLINE_MODE;
public:
	OptionsGUI(GLFWwindow* window);
	~OptionsGUI() override;
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif