#ifndef guiElement_h
#define guiElement_h
#include "camera.h"
#include "constantshader.h"
#include "GUIShader.h"
#include "indexbuffer.h"
#include "PostProcessingShader.h"
#include "Texture.h"
#include "vector.h"
#include "vertexbuffer.h"

class GUIElement
{
protected:
	BaseShader* Shader = new GUIShader();
public:
	virtual ~GUIElement();
	virtual void draw(BaseCamera& cam);
	virtual void shader(BaseShader* shader);
};

class GUITexture : public GUIElement
{
	Vector lb = Vector(-0.5f, -0.5f, 0); // lower bound
	Vector ub = Vector(0.5f, 0.5f, 0); // upper bound
	Vector scaling = Vector(1, 1, 0);
	int startX, startY, Width, Height;
	bool KeepImageSize = true;
	bool StartIsCenter = false;
	bool FollowMouse = false;
	Color Col = Color(-1,-1,-1);
private:
	Texture* pTex;
	IndexBuffer IB;
	VertexBuffer VB;

	void updateBuffers();
	void updateBounds();
public:
	GUITexture(int startX = 0, int startY = 0, int width = 0, int height = 0);
	GUITexture(int startX, int startY, Texture* tex, bool keepImageSize);
	~GUITexture() override;
	void texture(Texture* tex);
	void draw(BaseCamera& cam) override;
	void scale(Vector scale);
	void width(int width);
	void height(int height);
	void startIsCenter(bool b);
	void followMouse(bool b);
	void color(Color c);
};

#endif 