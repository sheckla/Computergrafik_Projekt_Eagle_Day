#ifndef OptionsGUI_h
#define OptionsGUI_h
#include "ApplicationGUIPrototype.h"
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
public:
	OptionsGUI(GLFWwindow* window);
	~OptionsGUI() override;
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif