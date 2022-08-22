#include "EscapeMenuGUI.h"

#include "ApplicationGUI.h"
#include "GUIButton.h"

EscapeMenuGUI::EscapeMenuGUI(GLFWwindow* window) : ApplicationGUIPrototype(window)
{


}

EscapeMenuGUI::~EscapeMenuGUI()
{
}

void EscapeMenuGUI::attachPostProcessingBuffer(PostProcessingBuffer* ppBuffer)
{
	this->ppBuffer = ppBuffer;
}

void EscapeMenuGUI::postProcessingEffects(bool b)
{
	PostProcessingInit = b;
	ppBuffer->gaussianBlur(true);
	//if (!b) ElapsedTime = 0;
}

void EscapeMenuGUI::elapseTime(float delta)
{
	ElapsedTime += delta;
	if (ElapsedTime > TIME_MAX_POST_PROCESSING_EFFECTS) ElapsedTime = TIME_MAX_POST_PROCESSING_EFFECTS;
	if (ElapsedTime < 0) ElapsedTime = 0;
	ppBuffer->elapseTime(ElapsedTime);
}

bool EscapeMenuGUI::postProcessingInit()
{
	return PostProcessingInit;
}

void EscapeMenuGUI::update(float delta)
{

	if (returnStartButton->listen() == RELEASE)
	{
		active(false);
		ApplicationGUI::AppGUI->setGUIStatus(STARTSCREEN_GUI, true);
	}
}

void EscapeMenuGUI::init()
{
	int buttonWidth = 300;
	int buttonHeight = 70;
	int gap = 10;

	int offsetHeight = gap + buttonHeight + 50;
	GUITexture* tab3 = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT - offsetHeight * 4, new Texture(ASSETS "button.png"), true);
	tab3->centred(true);
	tab3->scale(Vector(0.3, 0.3, 0));
	tab3->mouseoverHighlight(true);
	returnStartButton = new GUIButton(Window, tab3);
	Components.push_back(returnStartButton);

	offsetHeight += gap + buttonHeight + 50;
	GUITexture* leaveEscapeMenuTex = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT - offsetHeight * 1, new Texture(ASSETS "button_return.png"), true);
	leaveEscapeMenuTex->centred(true);
	leaveEscapeMenuTex->scale(Vector(0.3, 0.3, 0));
	leaveEscapeMenuTex->mouseoverHighlight(true);
	leaveEscapeMenuButton = new GUIButton(Window, leaveEscapeMenuTex);
	//Components.push_back(leaveEscapeMenuButton);

	GUITexture* logo = new GUITexture(ASPECT_WIDTH / 2, ASPECT_HEIGHT- 200, new Texture(ASSETS "logo.png"), true, false);
	logo->centred(true);
	logo->scale(Vector(0.3, 0.3, 0));
	Components.push_back(logo);
}

