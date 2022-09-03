#include "GUISlider.h"
#include <sstream>
#include "Application.h"
#include "MathUtil.h"
#include "MouseLogger.h"

GUISlider::GUISlider(float startX, float startY, float width, float height, float padding, const char* text) :
	GUILoadingMeter(startX, startY, width, height, padding),
	meterMouseListener(Application::pWindow, GLFW_MOUSE_BUTTON_LEFT, PRESS_TYPE::MOUSE)
{
	meterClickArea = new GUIConstantQuad(startX + padding, startY + padding, width - padding*2, height - padding*2);

	delete outlineArea; // delete from GUILoadingMeter
	outlineArea = new GUITexture(startX, startY, new Texture(ASSETS "img/button_simple.png"), true); // replace
	outlineArea->width(width);
	outlineArea->height(height);
	meter->mousePressColor(COL_VERY_LIGHT);

	// Percentage Text
	percentageText = new GUIText(startX + padding*2, startY + padding, "00.000%");

	// Description Text
	descriptionText = new GUIText(startX + outlineArea->width() + padding * 2, startY + padding, text);
}

GUISlider::~GUISlider()
{
	delete meterClickArea;
	delete percentageText;
	delete descriptionText;
}

void GUISlider::draw()
{
	outlineArea->draw();
	meter->draw();
	percentageText->draw();
	descriptionText->draw();
}

void GUISlider::update(float delta)
{
	meterMouseListener.listen();
	float slidePercentage = 0;
	if (EnableSliding && meterClickArea->mouseInside() && meterMouseListener.pressed())
	{
		// Slide
		meter->mousePress(true);
		slidePercentage = MathUtil::remapBounds(MouseLogger::x(), meterMin, meterMin + meterMax, 0, 1);
		percentage(slidePercentage);
	} else
	{
		meter->mousePress(false);
	}

	// Slide Percentage Text
	slidePercentage = percentage() * 100;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << slidePercentage << "%";
	std::string percentageString{ ss.str() };
	percentageText->text(percentageString.c_str());

	percentageText->update(delta);
	descriptionText->update(delta);

	GUILoadingMeter::update(delta);
}

void GUISlider::enableSliding(bool b)
{
	EnableSliding = b;
}
