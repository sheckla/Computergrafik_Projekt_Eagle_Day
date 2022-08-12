#ifndef SkyBox_h
#define SkyBox_h

#include "Model.h"
#include "Texture.h"

class SkyBox : public Model
{
private:
	Texture SkyBox2D;
	Texture SkyBoxCubeMap;
public:
	SkyBox();
	virtual ~SkyBox();
	void draw(const BaseCamera& cam) override;
};

#endif /
