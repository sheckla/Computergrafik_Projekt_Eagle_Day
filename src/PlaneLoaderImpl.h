#ifndef PlaneLoaderImpl_h
#define PlaneLoaderImpl_h

#include "Plane.h"
#include "PlaneLoader.h"

class PlaneLoaderImpl : public PlaneLoader
{
public:
	PlaneLoaderImpl();
	~PlaneLoaderImpl();
	Plane createPlane(const char* path);
};

#endif
