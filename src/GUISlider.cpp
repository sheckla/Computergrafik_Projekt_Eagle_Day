#include "GUISlider.h"

#include "Application.h"
#include "MathUtil.h"
#include "MouseLogger.h"

GUISlider::GUISlider(float startX, float startY, float width, float height, float padding) :
	GUILoadingMeter(startX, startY, width, height, padding),
	meterMouseListener(Application::pWindow, GLFW_MOUSE_BUTTON_LEFT, PRESS_TYPE::MOUSE)
{
	meterClickArea = new GUIConstantQuad(startX + padding, startY + padding, width - padding*2, height - padding*2);
	meterClickArea->color(Color(1, 0, 0));
	meterClickArea->constantColorMode(true);

}

GUISlider::~GUISlider()
{
}

void GUISlider::draw()
{
	if (meterClickArea->mouseInside() && meterMouseListener.listen() == PRESS_STATE::HOLD)
	{
		float remapMax = MathUtil::remapBounds(MouseLogger::x(), meterMin, meterMin + meterMax, 0, 1);
		percentage(remapMax);
	} 


	outlineArea->draw();
	meter->draw();
}

float GUISlider::sliderPercentage()
{
	return 0;
}
