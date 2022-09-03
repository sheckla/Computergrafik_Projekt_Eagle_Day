/*
 * EscapeMenuGUI
 * - wird im Spielbetrieb der Anwendung angezeigt und bietet Menuefuehrung
 */

#ifndef EscapeMenu_h
#define EscapeMenu_h

#include "ApplicationGUIPrototype.h"
#include "GUIButton.h"
#include "PostProcessingBuffer.h"

class EscapeMenuGUI : public ApplicationGUIPrototype
{
	PostProcessingBuffer* ppBuffer;
	GUIButton* returnStartButton;
	GUIButton* leaveEscapeMenuButton;
	PressListener* escapeButtonPressListener;
	GUIButton* restartButton;
	bool PostProcessingInit = false;
public:
	EscapeMenuGUI(GLFWwindow* window);
	virtual ~EscapeMenuGUI();
	void attachPostProcessingBuffer(PostProcessingBuffer* ppBuffer);
	void postProcessingEffects(bool b);
	bool postProcessingInit();
	void update(float delta) override;
	void init() override;
	void listen();
};

#endif