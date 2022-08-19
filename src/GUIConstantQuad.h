#ifndef GUIConstantQuad_h
#define GUIConstantQuad_h

#include "Camera.h"
#include "ConstantShader.h"
#include "GUIBaseComponent.h"
#include "GUIShader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"


/*
 * GUI-Element - quader mit fester Farbe
 * - startpixel X/Y
 * - breite/hoehe ausgehend vom startpixel X/Y
 * - Quader zentrieren oder ab X/Y aufspannen -> Centred
 */
class GUIConstantQuad : public GUIBaseComponent
{
protected:
	GUIShader* Shader = new GUIShader();
	Color Col = Color(1, 1, 1);
	Color MouseHitColor = Color(0.6, 0.6, 0.6);

	IndexBuffer IB;
	VertexBuffer VB;

	Vector LowerBound = Vector(-0.5f, -0.5f, 0); 
	Vector UpperBound = Vector(0.5f, 0.5f, 0); 
	Vector Scale = Vector(1, 1, 0); 

	// normal pixel-width/height
	int startPixelX, startPixelY, Width, Height;
	bool Centred = false;
	bool FollowMouse = false;
	bool MouseoverHighlight = false;

	// IB & VB aktualisieren mit dem aktuellen lower & upper bounds
	virtual void updateBuffers();

	// lower & upper Bound aktualisieren mit den aktuellen Scaling & StartpixelXY
	virtual void updateBounds();

public:
	GUIConstantQuad(int startX, int startY, int width, int height);
	~GUIConstantQuad() override;
	void draw() override;
	virtual void shader(GUIShader* shader);

	// Maus innerhalb des Elementes
	bool mouseInside();
	Vector startPixel();

	// Setter
	void startPixel(Vector v);
	void followMouse(bool b);
	void centred(bool b);
	void scale(Vector scale);
	void width(int width);
	void height(int height);
	void constantColorMode(bool b); // statische Farbe
	void color(Color c);
	void mouseHitColor(Color c);
	void mouseoverHighlight(bool b);

	// Getter
	int width();
	int height();
};



#endif 