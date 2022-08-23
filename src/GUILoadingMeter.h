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
	GUIConstantQuad* meter;
	GUIConstantQuad* outlineArea;

	float meterMin, meterMax;
	float meterMaxWidth;
public:
	GUILoadingMeter(float startX, float startY, float height, float width);
	~GUILoadingMeter() override;
	void draw() override;
	void percentage(float percentage);
};

#endif