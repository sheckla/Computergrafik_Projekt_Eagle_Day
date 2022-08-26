
#include "GUILoadingMeter.h"

#include "MathUtil.h"
#include "MouseLogger.h"

GUILoadingMeter::GUILoadingMeter(float startX, float startY, float width, float height, float padding)
{
	outlineArea = new GUIConstantQuad(startX, startY, width, height);
	outlineArea->color(Color(0.38, 0.18, 0.12));

	meterMin = startX + padding;
	meterMax = width - padding * 2;
	meterMaxWidth = meterMax;

	meter = new GUIConstantQuad(meterMin, startY + padding, 0, height - padding*2);
	meter->color(Color(0.6, 0.5, 0.3));

}

GUILoadingMeter::~GUILoadingMeter()
{
}

void GUILoadingMeter::draw()
{
	outlineArea->draw();
	meter->draw();
}

void GUILoadingMeter::percentage(float percentage)
{
	Percentage = percentage;
	float meterXPos = MathUtil::remapBounds(percentage, 0, 1, 0, meterMaxWidth);
	meter->width(meterXPos);
}