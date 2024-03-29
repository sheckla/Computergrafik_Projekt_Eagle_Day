#include "VolumetricCloudsLoaderImpl.h"

VolumetricCloudsLoaderImpl::VolumetricCloudsLoaderImpl(const char* worleyPath, const char* noisePath) : 
	worleyPath(worleyPath), noisePath(noisePath)
{
}

VolumetricCloudsLoaderImpl::~VolumetricCloudsLoaderImpl()
{
}

std::vector<CloudBox*> VolumetricCloudsLoaderImpl::createClouds(float width, float height, float depth)
{ 
	std::vector<CloudBox*> clouds;
	int size = 1;
	int y = 0;

	CloudShader* cloudShader = new CloudShader();
	cloudShader->diffuseTexture(Texture::LoadShared(ASSETS "/img/noise/worley/worley0.bmp"));

	for (int i = 0; i <= size; i++) {
		CloudBox* cloudBox = new CloudBox(width, height, depth);
		cloudBox->load(worleyPath, noisePath);
		cloudShader->aabb(&cloudBox->boundingBox());
		cloudBox->shader(cloudShader, true);
		cloudBox->shadowCaster(false); // wichtig fuer Schlagschatten
		//cloudBox->shadowCaster(false);

		Matrix translation;
		translation.translation(Vector(0, y, depth * i));
		cloudBox->transform(translation);

		clouds.push_back(cloudBox);
	}

	return clouds;
}
