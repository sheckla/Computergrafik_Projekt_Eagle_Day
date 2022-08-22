#ifndef EscapeMenu_h
#define EscapeMenu_h

#include "ApplicationGUIPrototype.h"
#include "GUIButton.h"
#include "PostProcessingBuffer.h"

constexpr float TIME_MAX_POST_PROCESSING_EFFECTS = 0.3f;

class EscapeMenuGUI : public ApplicationGUIPrototype
{
	PostProcessingBuffer* ppBuffer;
	GUIButton* returnStartButton;
	GUIButton* leaveEscapeMenuButton;

	float ElapsedTime = 0;
	bool PostProcessingInit = false;
public:
	EscapeMenuGUI(GLFWwindow* window);
	virtual ~EscapeMenuGUI();
	void attachPostProcessingBuffer(PostProcessingBuffer* ppBuffer);
	void postProcessingEffects(bool b);
	void elapseTime(float delta);
	bool postProcessingInit();
	void update(float delta) override;
	void init() override;
};

#endif