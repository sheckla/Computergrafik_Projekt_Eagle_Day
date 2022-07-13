#include "VolumetricCloudsLoaderImpl.h"

VolumetricCloudsLoaderImpl::VolumetricCloudsLoaderImpl()
{
}

VolumetricCloudsLoaderImpl::~VolumetricCloudsLoaderImpl()
{
}

TriangleBoxModel* VolumetricCloudsLoaderImpl::createClouds(const char* path)
{ // path = "worley0.bpm"
    CloudShader* cloudShader = new CloudShader(ASSETS);
    Texture* tex = new Texture;
    tex->load(path);
    cloudShader->SetWorleyTexture(tex);
    TriangleBoxModel* clouds = new TriangleBoxModel(10.0f, 1.0f, 10.0f);
    clouds->shader(cloudShader, true);
    Matrix mvClouds;
    mvClouds.translation(Vector(0, .6f, 0));
    clouds->transform(mvClouds);

	return clouds;
}
