#ifndef PlaneLoaderImpl_h
#define PlaneLoaderImpl_h

#include "Application.h"
#include "Plane.h"
#include "PlaneLoader.h"
#include "ConstantShader.h"
#include "Model.h"


class PlaneLoaderImpl : public PlaneLoader
{
public:
	PlaneLoaderImpl();
	virtual ~PlaneLoaderImpl();
	Plane* loadPlayerPlane(const char* path, Model**& planeParts) override;
};

#endif
