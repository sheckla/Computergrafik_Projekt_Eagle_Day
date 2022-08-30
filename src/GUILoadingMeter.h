/*
 * GUILoadingMeter
 * - Simpler Ladebalken mit Parametrisierung
 */

#ifndef GUILoadingMeter_h
#define GUILoadingMeter_h
#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"

class GUILoadingMeter : public GUIBaseComponent
{
protected:
	GUIConstantQuad* meter;
	GUIConstantQuad* outlineArea;

	float meterMin, meterMax;
	float meterMaxWidth;
	float Percentage = 0;
public:
	GUILoadingMeter(float startX, float startY, float width, float height, float padding);
	~GUILoadingMeter() override;
	void draw() override;
	void update(float delta) override;
	void percentage(float percentage);
	float percentage();
};

#endif