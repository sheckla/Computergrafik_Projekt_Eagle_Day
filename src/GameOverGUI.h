#ifndef GameOverGUI_h
#define GameOverGUI_h
#include "ApplicationGUIPrototype.h"
#include "GUIButton.h"

class GameOverGUI : public ApplicationGUIPrototype
{
	GUIButton* returnButton;
public:
	GameOverGUI(GLFWwindow* window);
	~GameOverGUI() override;
	void init() override;
	void update(float delta) override;
};

#endif