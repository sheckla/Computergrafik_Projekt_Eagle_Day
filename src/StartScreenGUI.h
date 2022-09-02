/*
 * StartScreenGUI
 * - wird nach dem erfolgreichen Laden der Anwendung angezeigt:
 *			- Titelbild & Hintergrund
 *			- bewegende Wolken
 * - TODO erfmoeglicht Menuefuehrung bezueglich der Spieleinstellungen
 */


#ifndef StartScreenGUI_h
#define StartScreenGUI_h

#include "Globals.h"
#include "ApplicationGUIPrototype.h"
#include "GUIButton.h"
#include "GUISlider.h"
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

	GUIButton* exitButton;
	PressListener exitButtonListener;
public:
	StartScreenGUI(GLFWwindow* window);
	~StartScreenGUI() override;
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif