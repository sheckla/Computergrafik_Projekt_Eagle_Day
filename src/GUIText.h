/*
 * GUITexture
 * - Ansammlung von GUIChar
 * - Erlaubt darstellung von Texten
 * - font.pnt & font.fnt muss vorhanden sein und reingeladen werden (FNTManager)
 *
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
	float startX, startY;
	float CharSpace = .95f;
	Vector Scale = Vector(1, 1, 0);
	float Width = 40;
	float Height = 40;
	float TotalWidth;
	bool Centred = false;

public:
	void applyCharParameter();
	GUIText(float startX, float startY, const char* string, FONT_NAMES font = ARIAL);
	~GUIText() override;

	void draw() override;
	void update(float delta);

	void text(const char* string);
	void scale(Vector scale);
	void centred(bool b);
	void charSpace(float f);
	void width(float w);
	void height(float h);

	float height();
	float totalWidth();
};

#endif