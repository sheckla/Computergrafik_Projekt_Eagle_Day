#include "PlaneLoaderImpl.h"

PlaneLoaderImpl::PlaneLoaderImpl()
{
}

PlaneLoaderImpl::~PlaneLoaderImpl()
{

}

Plane* PlaneLoaderImpl::loadPlayerPlaneOnline(const char* path, Model**& planeParts, const char* srv_Adr, int port)
{
    Plane* plane = new Plane(path, srv_Adr, port);
	planeParts = plane->getParts();
    return plane;
}

Plane* PlaneLoaderImpl::loadPlayerPlane(const char* path, Model**& planeParts)
{
	Plane* plane = new Plane(path);
	planeParts = plane->getParts();
	return plane;
}

