#include "PlaneLoaderImpl.h"

PlaneLoaderImpl::PlaneLoaderImpl()
{
}

PlaneLoaderImpl::~PlaneLoaderImpl()
{

}

Plane* PlaneLoaderImpl::createPlane(const char* path)
{
    Plane* plane = new Plane();
    plane->loadModels(path);
    return plane;
}
