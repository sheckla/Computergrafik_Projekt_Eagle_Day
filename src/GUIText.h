#ifndef GUIText_h
#define GUIText_h
#include <vector>

#include "GUIBaseComponent.h"
#include "GUIChar.h"

class GUIText : public GUIBaseComponent
{
	char* string;
	std::vector<GUIChar*> chars;
	std::vector<FontData> data;
public:
	GUIText(float startX, float startY, char* string);
	~GUIText() override;
	void draw() override;
	void text(char* string);
};

#endif