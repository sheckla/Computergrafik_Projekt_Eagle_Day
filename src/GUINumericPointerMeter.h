/*
 * GUINumericPointerMeter
 * - DisplayKomponente fuer das Gameplay
 * - funktioniert entweder fuer die Hoehe oder die Geschwindigkeit des Flugzeuges
 *
 *
 */

#ifndef GUIMeter_h
#define GUIMeter_h

#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"
#include "GUIConstantTriangle.h"
#include "GUIText.h"
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

static std::vector<const char*> speedTexts = { "0  ", "100", "200", "300", "400", "500", "600" };
static std::vector<const char*> altitudeTexts = { "0  ", "50", "100", "150", "200", "250", "300" };

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
	GUIText* number(int startX, int startY, int i, Vector scale);
	void initMeterQuads(int startX, int startY, Vector& barWidths, Vector& barHeights,
		int& bigXOffset, int& mediumXOffset, int& smallXOffset);
	void initMeterText(int startY);
public:
	Plane* plane = ModelLoader::pPlayerPlane;

	std::vector<GUIBaseComponent*> Components; // Bars
	std::vector<GUIText*> Numbers;

	// Pointer
	GUIConstantQuad* MeterQuad;
	GUIConstantTriangle* MeterTriangle;

	// Text
	GUIText* meterText;

	int MeterMaxHeight = 0; // Max Height for Pointer
	int ComponentAmount = 0;
	bool speedMeterMode;

	GUINumericPointerMeter(int startX, int startY, bool speedMeterMode, int gap, Vector barWidths, Vector barHeights, 
		Vector cornerSize, Vector numberScale);
	~GUINumericPointerMeter() override;
	void draw() override;
	void update(float delta);
};

#endif