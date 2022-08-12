/*
 * Interface
 * Trennung: Application -> Plane
 *
 */

#pragma once

#include "Plane.h"

class PlaneLoader {
public:
	virtual ~PlaneLoader() = default;
	virtual Plane* loadPlayerPlane(const char*, Model**& planeParts) = 0;
	virtual Plane* createPlane(const char*, const char* srv_Adr, int port) = 0;
};