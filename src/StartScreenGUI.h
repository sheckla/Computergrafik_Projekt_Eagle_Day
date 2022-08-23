#ifndef StartScreenGUI_h
#define StartScreenGUI_h

#include "Globals.h"
#include "ApplicationGUIPrototype.h"
#include "GUIButton.h"
#include "PressListener.h"

struct CloudParams
{
	float speed;
	bool fallDown;
	float fallSpeed;
};

class StartScreenGUI : public ApplicationGUIPrototype
{
	std::vector<GUITexture*> clouds;
	std::vector<CloudParams> cloudParams;
	GUIButton* startButton;
	PressListener startButtonListener;
public:
	StartScreenGUI(GLFWwindow* window);
	~StartScreenGUI() override;
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif