#include "GUIButtonSwitch.h"

#include "ApplicationGUI.h"

GUIButtonSwitch::GUIButtonSwitch(float startX, float startY, float width, float height)
{
	GUIConstantQuad* quad = new GUIConstantQuad(startX, startY, width, height);
	quad->color(COL_DARK);
	quad->constantColorMode(true);
	button = new GUIButton(ApplicationGUI::AppGUI->Window, quad, "MouseSteer", ARIAL_BLACK);

	onOffText = new GUIText(startX + width + 10, startY, "OFF");
}

GUIButtonSwitch::~GUIButtonSwitch()
{
}

void GUIButtonSwitch::draw()
{
	button->draw();
	onOffText->draw();
}

void GUIButtonSwitch::update(float delta)
{
	onOffText->update(delta);
	button->update(delta);

	if (button->mouseInside() && button->state() == RELEASE)
	{
		if (On)
		{
			onOffText->text("Off");
			On = false;
		} else
		{
			onOffText->text("On");
			On = true;
		}
	}

}

bool GUIButtonSwitch::on()
{
	return On;
}
