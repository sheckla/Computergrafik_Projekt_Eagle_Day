#ifndef GUIMeter_h
#define GUIMeter_h

#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"
#include "GUIConstantTriangle.h"
#include "GUIText.h"
#include "GUITexture.h"
#include "ModelLoader.h"

constexpr char const* numberPaths[10] = { ASSETS "typography/0.png",
ASSETS "typography/1.png",
	ASSETS "typography/2.png",
	ASSETS "typography/3.png",
	ASSETS "typography/4.png",
	ASSETS "typography/5.png",
	ASSETS "typography/6.png",
	ASSETS "typography/7.png",
	ASSETS "typography/8.png",
	ASSETS "typography/9.png" };

/*
 *
 * Loop:
 *   [--- Big ---]    (Number1)
 *     [- Sml -]
 * 	   [- Sml -]
 * 	  [-- Med --]
 *     [- Sml -]
 * 	   [- Sml -]
 *   [--- Big ---]    (Number0)
 */
class GUINumericPointerMeter : public GUIBaseComponent
{
private:
	GUIBaseComponent* cornerBar(int startX, int startY, int width, int height, int cornerWidth, int cornerHeight);
	GUIBaseComponent* bar(int startX, int startY, int width, int height);
	GUITexture* number(int startX, int startY, const char* numberPath, Vector scale);
public:
	Plane* plane = ModelLoader::pPlayerPlane;

	std::vector<GUIBaseComponent*> Components; // Bars
	std::vector<GUITexture*> Numbers;
	GUIText* numberDisplay;

	// Pointer
	GUIConstantQuad* MeterQuad;
	GUIConstantTriangle* MeterTriangle;
	int MeterMaxHeight = 0; // Max Height for Pointer
	int ComponentAmount = 0;
	bool leftBound;

	GUINumericPointerMeter(int startX, int startY, bool leftBound, int gap, Vector barWidths, Vector barHeights, 
		Vector cornerSize, Vector numberScale);
	void draw() override;
	~GUINumericPointerMeter() override;
};

#endif