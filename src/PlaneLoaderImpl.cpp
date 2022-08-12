#include "PlaneLoaderImpl.h"

PlaneLoaderImpl::PlaneLoaderImpl()
{
}

PlaneLoaderImpl::~PlaneLoaderImpl()
{

}

Plane* PlaneLoaderImpl::createPlane(const char* path, const char* srv_Adr, int port)
{
    Plane* plane = new Plane(srv_Adr, port);
    plane->loadModels(path);
    return plane;
}

Plane* PlaneLoaderImpl::loadPlayerPlane(const char* path, Model**& planeParts)
{
	Plane* plane = new Plane(path);
	planeParts = plane->getParts();
	return plane;
}

