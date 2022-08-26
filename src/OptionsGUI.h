#ifndef OptionsGUI_h
#define OptionsGUI_h
#include "ApplicationGUIPrototype.h"

class OptionsGUI : public ApplicationGUIPrototype
{
private:

public:
	OptionsGUI(GLFWwindow* window);
	~OptionsGUI() override;
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif