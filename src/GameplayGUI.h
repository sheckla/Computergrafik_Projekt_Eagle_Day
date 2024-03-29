/*
 * GameplayGUI
 * - Zeigt waehrend des Spieles Informationen fuer den Spieler an
 * - Geschwindigkeitsmeter
 * - Hoehenmeter
 * - Kompass fuer x-Rotation
 * - Lebensanzeige
 * - GunHP-Anzeige
 */

#ifndef GameplayGUI_h
#define GameplayGUI_h

#include "ApplicationGUIPrototype.h"
#include "GUILoadingMeter.h"
#include "GUISlider.h"
#include "GUIText.h"
#include "GUITexture.h"

class GameplayGUI : public ApplicationGUIPrototype
{
	GUISlider* lifeMeter;
	GUISlider* gunMeter;
	GUIText* speedMeterText;
	GUIText* altitudeMeterText;
	GUITexture* mouseCircle;
	GUIConstantQuad* quad;
	GUIConstantQuad* compass;
	GUIText* compassText;
public:
	GameplayGUI(GLFWwindow* window);
	virtual ~GameplayGUI();
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif