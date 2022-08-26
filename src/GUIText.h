/*
 * GUIText
 * - Ansammlung von GUIChar
 * - Erlaubt darstellung von Texten
 * - font.pnt & font.fnt muss vorhanden sein und reingeladen werden
 *
 */

#ifndef GUIText_h
#define GUIText_h
#include <vector>

#include "GUIBaseComponent.h"
#include "GUIChar.h"

class GUIText : public GUIBaseComponent
{
	const char* string;
	std::vector<GUIChar*> chars;
	std::vector<CharData> data;

	float startX, startY;
public:
	GUIText(float startX, float startY, const char* string);
	~GUIText() override;
	void draw() override;
	void text(const char* string);
};

#endif