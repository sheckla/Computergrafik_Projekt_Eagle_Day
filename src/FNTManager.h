/*
* FNTManager
* - Ansammlung von Fonts innerhalb von ASSETS/"typography"
* - Fonts muessen vor dem Nutzen erstmalig reingeladen werden
* 
* - genutzt von GUIText & GUIChar
*/
#ifndef FNTManager_h
#define FNTManager_h
#include <vector>
#include "texture.h"

// Application Supported Fonts
enum FONT_NAMES
{
	ARIAL,
	WW2,
	TYPEWRITER,
	ARIAL_BLACK
};

// .fnt params per Char
struct CHAR_DATA
{
	CHAR_DATA(int id, int x, int y, int width, int height, int xOffset, int yOffset, int xAdvance) :
		font(ARIAL), id(id), x(x), y(y), width(width), height(height), xOffset(xOffset), yOffset(yOffset), xAdvance(xAdvance) {}
	CHAR_DATA(): font(ARIAL), id(0), x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0) {}
	FONT_NAMES font;
	int id;
	int x;
	int y;
	int width;
	int height;
	int xOffset;
	int yOffset;
	int xAdvance;
};

/*
* Application Font info
* - FontName -> Application Fontname
* - FontTexture -> Map with all chars
* - CharData -> vector of supported chars for Font
*/
struct FONT_CHAR_DATA
{
	std::vector<CHAR_DATA> CharData;
	Texture* FontTexture;
	FONT_NAMES FontName;
};

class FNTManager
{
	static FNTManager* pFNTManager;
	static std::vector<FONT_CHAR_DATA> FontCharData;
	static FONT_CHAR_DATA matchFontName(FONT_NAMES font); // find FONT_CHAR_DATA for given font
public:
	static FNTManager& instance();

	static void loadFont(const char* fontPath, const char* fontImagePath, FONT_NAMES font);
	static CHAR_DATA charData(FONT_NAMES font, const char fontChar);
	static Texture* fontTexture(FONT_NAMES font);

	// Wie groess ein gegebener Text fuer eine bestimmte Font sein wuerde
	static float textWidth(const char* text, FONT_NAMES font);
};

#endif