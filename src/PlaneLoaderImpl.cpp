#include "PlaneLoaderImpl.h"

PlaneLoaderImpl::PlaneLoaderImpl()
{
}

PlaneLoaderImpl::~PlaneLoaderImpl()
{

}

Plane* PlaneLoaderImpl::loadPlayerPlane(const char* path)
{
    Plane* plane = new Plane(path);
    return plane;
}
