#include "GUIButtonSwitch.h"

#include "ApplicationGUI.h"

GUIButtonSwitch::GUIButtonSwitch(float startX, float startY, float width, float height, const char* text)
{

	offTexture = new GUITexture(startX, startY, new Texture(ASSETS "img/button_simple_gray.png"), true);
	offTexture->width(width);
	offTexture->height(height);
	offTexture->mouseoverHighlight(true);

	onTexture = new GUITexture(startX, startY, new Texture(ASSETS "img/button_simple.png"), true);
	onTexture->width(width);
	onTexture->height(height);
	onTexture->mouseoverHighlight(true);

	button = new GUIButton(ApplicationGUI::AppGUI->Window, offTexture, text, ARIAL_BLACK, false);
	button->pText()->scale(Vector(0.8, 0.8, 0));
	button->pText()->charSpace(0.8);

	onOffText = new GUIText(startX + width + 10, startY, "OFF");
}

GUIButtonSwitch::~GUIButtonSwitch()
{
	delete offTexture;
	delete onTexture;
	delete button;
	delete onOffText;
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
	on(On);
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

		button->area(onTexture);
	}
	else
	{
		button->area(offTexture);
		onOffText->text("Off");
	}
}
