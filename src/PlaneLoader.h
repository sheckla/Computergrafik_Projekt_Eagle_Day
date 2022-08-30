/*
 * Interface
 * Trennung: Application -> Plane
 */

#pragma once

#include "Plane.h"
#include "EnemyPlane.h"

class PlaneLoader {
public:
	virtual ~PlaneLoader() = default;
	virtual Plane* loadPlayerPlane(const char* path, Model**& planeParts) = 0;
	virtual Plane* loadPlayerPlaneOnline(const char* path ,Model**& planeParts, const char* srv_Adr, int port) = 0;
	virtual Plane* loadAIPlane(const char* path, Model**& planeParts) = 0;
};