/*
 * GUISlider
 * - UI Komponente mit Text und Slider-Balken
 * - Slider kann mit der Maus verschoben werden
 * - Prozentualler Anteil wird innerhalb des Sliders angezeigt
 */

#ifndef GUISlider_h
#define GUISlider_h
#include "GUIBaseComponent.h"
#include "GUILoadingMeter.h"
#include "GUIText.h"
#include "PressListener.h"


class GUISlider : public GUILoadingMeter
{
	GUIConstantQuad* meterClickArea; // Mouse Click Area
	PressListener meterMouseListener;
	GUIText* percentageText;
	GUIText* descriptionText;
	bool EnableSliding = true;
public:
	GUISlider(float startX, float startY, float width, float height, float padding, const char* text);
	~GUISlider() override;
	void draw() override;
	void update(float delta) override;
	void enableSliding(bool b);
};

#endif