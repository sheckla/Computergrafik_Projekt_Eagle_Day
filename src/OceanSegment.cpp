#include "OceanSegment.h"
#include "rgbimage.h"
#include "OceanShader.h"


OceanSegment::OceanSegment(HeightMapStorage* hms,int resolution, const char* HeightMap, const char* DetailMap1, const char* DetailMap2) : Size(20, 1, 20),HeightMaps(hms),Resolution(resolution)
{
	shadowCaster(true);
	this->IB = new IndexBuffer;
	this->VB = new VertexBuffer;
}

OceanSegment::~OceanSegment()
{
	//delete(Water_Frames);
}

Vector& OceanSegment::convert_heightmap_color_to_vec(const RGBImage* img, int x, int y) const {
	float vec_x, vec_y, vec_z;

	//      (      ratio[0,1]  *  scale           ) - (centralised offset)
	vec_x = (float)x / img->width() * this->width() - this->width() / 2;
	vec_z = (float)y / img->height() * this->depth() - this->depth() / 2;
	vec_y = img->getPixelColor(x, y).R;
	return Vector(vec_x, 0, vec_z);
}

bool OceanSegment::load()
{
	int width = Resolution;
	int height = Resolution;

	int vertices = 0;
	this->VB->begin();
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {

			/* triangle
			* C - B     
			* | /	  E
			* A     / |
			*	  F - D
			*/

			Vector n(0, 1, 0);
			n.normalize();
			this->VB->addNormal(n);

			this->VB->addTexcoord1((float)x / (width - 1), (float)y / (height - 1));
			this->VB->addTexcoord0((float)x / (width - 1), (float)y / (height-1));

		
			float vec_x = (float)x / (width-1) * this->width();
			float vec_z = (float)y / (height-1) * this->depth();
			Vector x(vec_x, 0, vec_z);
			this->VB->addVertex(x);
			vertices++;
		}
	}

	this->IB->begin();
	for (unsigned int x = 0; x < width - 1; x++) {
		for (unsigned int y = 0; y < height - 1; y++) {
			unsigned int offset = y * height + x;

			/* first triangle
			* C - B
			* | /
			* A
			*/
			this->IB->addIndex(offset); // C
			this->IB->addIndex(offset + 1);// B
			this->IB->addIndex(offset + height); // A


			/* second triangle
			*     E
			*   / |
			* F - D
			*/
			this->IB->addIndex(offset + width); // F
			this->IB->addIndex(offset + 1); // E
			this->IB->addIndex((offset + 1) + height); // D
		}
	}
	return true;
}

void OceanSegment::shader(BaseShader* shader, bool deleteOnDestruction)
{
	BaseModel::shader(shader, deleteOnDestruction);
}

void OceanSegment::draw(const BaseCamera& Cam)
{
	applyShaderParameter();
	BaseModel::draw(Cam);
	this->VB->activate();
	this->IB->activate();
	glDrawElements(GL_TRIANGLES, this->IB->indexCount(), this->IB->indexFormat(), 0);
	this->IB->deactivate();
	this->VB->deactivate();
}

void OceanSegment::applyShaderParameter()
{
	OceanShader* Shader = dynamic_cast<OceanShader*>(BaseModel::shader());
	if (!Shader)
		return;

	//Set the active displacement texture from texture storage according to elapsed time
	HeightTex = HeightMaps->Water_Frames[(int)HeightMaps->time];

	Shader->mixTex(&HeightTex);
	

	for (int i = 0; i < 2; i++)
		Shader->detailTex(i, &DetailTex[i]);
	Shader->scaling(Size);

}

void OceanSegment::update(double deltaTime)
{
	
}
