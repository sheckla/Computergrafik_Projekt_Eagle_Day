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

class GameplayGUI : public ApplicationGUIPrototype
{
	bool b;
public:
	GameplayGUI(GLFWwindow* window);
	virtual ~GameplayGUI();
	void draw() override;
	void update(float delta) override;
	void init() override;
};

#endif