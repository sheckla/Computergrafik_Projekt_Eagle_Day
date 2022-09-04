/*
 * PlaneLoaderImplementation:
 * - Zustaendig fuer das korrekte Laden der Flugzeuge
 */
#ifndef PlaneLoaderImpl_h
#define PlaneLoaderImpl_h
#include "Plane.h"
#include "PlaneLoader.h"
#include "Model.h"

class PlaneLoaderImpl : public PlaneLoader
{
public:
	PlaneLoaderImpl();
	virtual ~PlaneLoaderImpl();
	Plane* loadPlayerPlaneOnline(const char* path, Model**& planeParts, const char* srv_Adr, int port) override;
	Plane* loadPlayerPlane(const char* path, Model**& planeParts) override;
	Plane* loadAIPlane(const char* path, Model**& planeParts) override;
};

#endif
