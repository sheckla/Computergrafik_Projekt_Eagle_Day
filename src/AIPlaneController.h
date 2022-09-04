/*
 * AIPlaneControls
 * - Steuereinheit fuer den gegnerischen KI Spieler
 * - AIPlane versucht seine Position an den des Spielers anzunaehern
 */
#ifndef AIPlaneController_h
#define AIPlaneController_h
#include "EnemyPlane.h"
#include "Plane.h"

class AIPlaneController
{
	int deadCounter = 0;
public:
	EnemyPlane* aiPlane;
	Plane* playerPlane;
	AIPlaneController();
	void update(float delta);
};

#endif