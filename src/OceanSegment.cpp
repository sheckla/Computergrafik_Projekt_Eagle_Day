#include "OceanSegment.h"
#include "rgbimage.h"
#include "OceanShader.h"


OceanSegment::OceanSegment(HeightMapStorage* hms,int resolution, const char* HeightMap, const char* DetailMap1, const char* DetailMap2) : Size(20, 1, 20),HeightMaps(hms),Resolution(resolution)
{
	if (HeightMap && DetailMap1 && DetailMap2)
	{
		//bool loaded = load(HeightMap, DetailMap1, DetailMap2);
		//if (!loaded)
		//	throw std::exception();
	}
	this->IB = new IndexBuffer;
	this->VB = new VertexBuffer;
}

OceanSegment::~OceanSegment()
{
	//delete(Water_Frames);
	std::cout << "DELETED!" << std::endl;
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

	//std::cout << "start" << std::endl;
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

			//std::cout <<"X: " << x << std::endl;
		
			float vec_x = (float)x / (width-1) * this->width();
			float vec_z = (float)y / (height-1) * this->depth();
			Vector x(vec_x, 0, vec_z);
			this->VB->addVertex(x);
			vertices++;
			//std::cout << heightmap_vec.X << "  ,  "<< heightmap_vec.Z<< std::endl;
			//std::cout << (float)x / img->width() <<"  ,  "<< (float)y / img->height() << std::endl;
		}
	}
	//std::cout << "------------------------------------------------------------------------------"<< std::endl;
	//this->VB->end();
	//std::cout<< vertices<<std::endl;

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
	//this->IB->end();
	//std::cout << "end" << std::endl;
	//std::cout << "created" << std::endl;
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
	// TODO: add code for drawing index and vertex buffers
	//std::cout << "draw" << std::endl;
	//std::cout << "VertexCount " << this->VB->vertexCount() << std::endl;
	this->VB->activate();
	this->IB->activate();
	glDrawElements(GL_TRIANGLES, this->IB->indexCount(), this->IB->indexFormat(), 0);
	//std::cout << "draw ELEMENTS" << std::endl;
	this->IB->deactivate();
	this->VB->deactivate();



}

void OceanSegment::applyShaderParameter()
{
	OceanShader* Shader = dynamic_cast<OceanShader*>(BaseModel::shader());
	if (!Shader)
		return;

	HeightTex = HeightMaps->Water_Frames[(int)HeightMaps->time];
	//HeightTex = Water_Frames[(int) time];

	Shader->mixTex(&HeightTex);
	//Shader->setResolution(Resolution);
	

	for (int i = 0; i < 2; i++)
		Shader->detailTex(i, &DetailTex[i]);
	Shader->scaling(Size);

	// TODO: add additional parameters if needed..
}

void OceanSegment::update(double deltaTime)
{
	
}
