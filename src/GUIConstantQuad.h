/*
 * GUI-Element - quader mit fester Farbe
 * - bietet viele und grundlegende Funktionalitaeten fuer UI/Displayelemente
 * - startpixel X/Y
 * - breite/hoehe ausgehend vom startpixel X/Y
 * - Quader zentrieren oder ab X/Y aufspannen -> Centred
 */

#ifndef GUIConstantQuad_h
#define GUIConstantQuad_h

#include "Camera.h"
#include "ConstantShader.h"
#include "GUIBaseComponent.h"
#include "GUIShader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class GUIConstantQuad : public GUIBaseComponent
{
protected:
	GUIShader* Shader = new GUIShader();
	IndexBuffer* IB = new IndexBuffer();
	VertexBuffer* VB = new VertexBuffer();
	Vector LowerBound = Vector(-0.5f, -0.5f, 0); 
	Vector UpperBound = Vector(0.5f, 0.5f, 0); 
	Color Col = Color(1, 1, 1);
	Vector Scale = Vector(1, 1, 0); 

	// normal pixel-width/height
	float startPixelX, startPixelY, Width, Height;

	// Display
	bool Centred = false;
	bool FollowMouse = false;

	// Mouseover
	bool MouseoverHighlight = false; bool ForceMouseoverHighlight = false;
	Color MouseoverHighlightColor = Color(0.6f, 0.6f, 0.6f);

	// Mousepress
	bool MousePress = false;
	Color MousePressColor = Color(1, 0, 0);

	// IB & VB aktualisieren mit dem aktuellen lower & upper bounds
	virtual void updateBuffers();
	// lower & upper Bound aktualisieren mit den aktuellen Scaling & StartpixelXY
	virtual void updateBounds();

	virtual void handleMouseEvents();

public:
	GUIConstantQuad(float startX, float startY, float width, float height);
	~GUIConstantQuad() override;
	void draw() override;
	virtual void shader(GUIShader* shader);
	void update(float delta) override;

	// Maus innerhalb des Elementes
	bool mouseInside();

	// --- Setter --- 
	// General
	void startPixel(Vector v);
	void width(float width);
	void height(float height);
	void scale(Vector scale);

	// Display
	void centred(bool b);
	void followMouse(bool b);

	// Color mode
	void color(Color c);
	void constantColorMode(bool b); // statische Farbe

	// Mouseover
	void mouseoverHighlight(bool b);
	void mouseoverHighlightColor(Color c);
	void forceMouseoverHighlight(bool b);

	// Mousepress
	void mousePress(bool b);
	void mousePressColor(Color c);

	// Getter
	int width();
	int height();
	Vector startPixel();
	bool mousePress();
};

#endif 