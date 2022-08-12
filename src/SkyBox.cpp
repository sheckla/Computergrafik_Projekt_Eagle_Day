#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::draw(const BaseCamera& cam)
{
	BaseModel::draw(cam);
}
