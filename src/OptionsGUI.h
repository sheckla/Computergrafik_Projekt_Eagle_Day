/*
 * OptionsGUI
 * - bietet Menufuehrung zum Einstellen von Optionen, darunter:
 *		[Zur Laufzeit]
 *		- Post Processing Filter Switch
 *		- Mouse Steering Mode Switch
 *		- Audio percentage Slider
 *		[Nach Neustart -> Einstellungen werden in settings.txt gespeichert]
 *		- Fullscreen Mode
 *		- Online Mode
 */
#ifndef OptionsGUI_h
#define OptionsGUI_h
#include "ApplicationGUIPrototype.h"
#include "ApplicationSettings.h"
#include "GUIButtonSwitch.h"
#include "GUISlider.h"

class OptionsGUI : public ApplicationGUIPrototype
{
	// Top Left Icon
	GUITexture* cogwheelTexture = nullptr;
	PressListener* cogwheelPressListener;
	PressListener* escapeButtonPressListener;

	// Switchable Options
	GUISlider* audioSlider;
	GUIButtonSwitch* mouseControlSwitch;
	GUIButtonSwitch* fullscreenControlSwitch;
	GUIButtonSwitch* onlineModeControlSwitch;
	GUIButtonSwitch* sepiaPostProcessingSwitch;

	// ONLINE_MODE Info text
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