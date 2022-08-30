#ifndef GUISlider_h
#define GUISlider_h
#include "GUIBaseComponent.h"
#include "GUILoadingMeter.h"
#include "GUIText.h"
#include "PressListener.h"


class GUISlider : public GUILoadingMeter
{
	GUIConstantQuad* meterClickArea;
	PressListener meterMouseListener;
	GUIText* percentageText;
	GUIText* descriptionText;
public:
	GUISlider(float startX, float startY, float width, float height, float padding);
	~GUISlider() override;
	void draw() override;
	void update(float delta) override;
};

#endif