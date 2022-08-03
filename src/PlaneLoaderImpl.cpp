#include "PlaneLoaderImpl.h"

PlaneLoaderImpl::PlaneLoaderImpl()
{
}

PlaneLoaderImpl::~PlaneLoaderImpl()
{

}

Plane* PlaneLoaderImpl::loadPlayerPlane(const char* path, Model**& planeParts)
{
	Plane* plane = new Plane(path);
	planeParts = plane->getParts();
	return plane;
}
