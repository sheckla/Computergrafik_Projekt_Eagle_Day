/*
 * GUITextu
 * - Ansammlung von GUIChar
 * - Erlaubt darstellung von Texten
 * - font.pnt & font.fnt muss vorhanden sein und reingeladen werden (FNTManager)
 * - Gap zwischen Zeichen anpassbar mit charSpace()
 */

#ifndef GUIText_h
#define GUIText_h
#include <vector>

#include "GUIBaseComponent.h"
#include "GUIChar.h"

class GUIText : public GUIBaseComponent
{
	FONT_NAMES font;
	const char* textString;
	std::vector<GUIChar*> textChars;

	Vector Scale = Vector(1, 1, 0);
	float startX, startY;
	float CharSpace = .95f;
	float Width = 40;
	float Height = 40;
	float TotalWidth;
	bool Centred = false;
public:
	GUIText(float startX, float startY, const char* string, FONT_NAMES font = ARIAL);
	~GUIText() override;

	void draw() override;
	void update(float delta);
	void applyCharParameter();

	// setters
	void text(const char* string);
	void scale(Vector scale);
	void centred(bool b);
	void charSpace(float f);
	void width(float w);
	void height(float h);
	void startPixel(Vector v);

	// getters
	Vector startPixel();
	float height();
	float totalWidth();
};

#endif