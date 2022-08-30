#include "GUISlider.h"

#include <sstream>

#include "Application.h"
#include "MathUtil.h"
#include "MouseLogger.h"

GUISlider::GUISlider(float startX, float startY, float width, float height, float padding) :
	GUILoadingMeter(startX, startY, width, height, padding),
	meterMouseListener(Application::pWindow, GLFW_MOUSE_BUTTON_LEFT, PRESS_TYPE::MOUSE)
{
	meterClickArea = new GUIConstantQuad(startX + padding, startY + padding, width - padding*2, height - padding*2);

	meter->mousePressColor(COL_VERY_LIGHT);

	// Percentage Text
	percentageText = new GUIText(startX + padding*2, startY + padding, "00.000%");

	// Description Text
	descriptionText = new GUIText(startX + outlineArea->width() + padding * 2, startY + padding, "Audio");

	
}

GUISlider::~GUISlider()
{
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
	float remapMax = 0;
	if (meterClickArea->mouseInside() && meterMouseListener.pressed())
	{
		meter->mousePress(true);
		remapMax = MathUtil::remapBounds(MouseLogger::x(), meterMin, meterMin + meterMax, 0, 1);
		percentage(remapMax);
	} else
	{
		meter->mousePress(false);
	}

	remapMax = percentage() * 100;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << remapMax << "%";
	std::string percentageString{ ss.str() };
	percentageText->text(percentageString.c_str());

	meterClickArea->update(delta);
	percentageText->update(delta);
	descriptionText->update(delta);
	GUILoadingMeter::update(delta);
}