#ifndef AIPlaneController_h
#define AIPlaneController_h
#include "Plane.h"

class AIPlaneController
{
public:
	Plane* aiPlane;
	Plane* playerPlane;

	bool stop = false;
	float prevXZCross = 0;

	float min = 0;
	float max = 0;

	AIPlaneController();

	void update(float delta);
};

#endif