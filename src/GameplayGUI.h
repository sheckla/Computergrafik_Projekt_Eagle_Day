/*
 * GameplayGUI
 * - Zeigt waehrend des Spieles Informationen fuer den Spieler an
 * - Geschwindigkeitsmeter
 * - Hoehenmeter
 * - TODO Kompass 
 * - TODO Andere Gegner 
 */

#ifndef GameplayGUI_h
#define GameplayGUI_h

#include "ApplicationGUIPrototype.h"
#include "GUILoadingMeter.h"
#include "GUIText.h"

class GameplayGUI : public ApplicationGUIPrototype
{
	GUILoadingMeter* lifeMeter;
	GUIText* speedMeterText;
	GUIText* altitudeMeterText;
public:
	GameplayGUI(GLFWwindow* window);
	virtual ~GameplayGUI();
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif