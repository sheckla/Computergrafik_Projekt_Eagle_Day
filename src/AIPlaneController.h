#ifndef AIPlaneController_h
#define AIPlaneController_h
#include "Plane.h"

class AIPlaneController
{
public:
	Plane* aiPlane;
	Plane* playerPlane;

	AIPlaneController();

	void update(float delta);
};

#endif