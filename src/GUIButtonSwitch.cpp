#include "GUIButtonSwitch.h"

#include "ApplicationGUI.h"

GUIButtonSwitch::GUIButtonSwitch(float startX, float startY, float width, float height, const char* text)
{
	GUIConstantQuad* quad = new GUIConstantQuad(startX, startY, width, height);
	quad->color(COL_DARK);
	quad->constantColorMode(true);
	quad->mouseoverHighlight(true);
	quad->mouseoverHighlightColor(COL_LIGHT);
	button = new GUIButton(ApplicationGUI::AppGUI->Window, quad, text, ARIAL_BLACK, false);
	button->pText()->scale(Vector(0.8, 0.8, 0));
	button->pText()->charSpace(0.8);

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

void GUIButtonSwitch::on(bool b)
{
	On = b;
	if (On)
	{
		onOffText->text("On");
	}
	else
	{
		onOffText->text("Off");
	}
}
