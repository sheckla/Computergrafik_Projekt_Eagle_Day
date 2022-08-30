#include "FNTManager.h"

#include <fstream>
#include <sstream>
#include <string>

#include "Globals.h"

FNTManager* FNTManager::pFNTManager = nullptr;
std::vector<FONT_CHAR_DATA> FNTManager::FontCharData;

FONT_CHAR_DATA FNTManager::matchFontName(FONT_NAMES font)
{
	for (FONT_CHAR_DATA fontCharData : FontCharData)
	{
		if (fontCharData.FontName == font) return fontCharData;
	}
	return {};
}

FNTManager& FNTManager::instance()
{
	if (!pFNTManager) pFNTManager = new FNTManager();
	return *pFNTManager;
}

void FNTManager::loadFont(const char* fontPath, const char* fontImagePath, FONT_NAMES font)
{
	FONT_CHAR_DATA fontCharData;
	fontCharData.FontName = font;
	fontCharData.FontTexture = new Texture(fontImagePath);

	int lineCount = 0;

	std::ifstream file;
	file.open(fontPath);

	std::string line;
	while (std::getline(file, line))
	{
		if (lineCount++ >= 4)
		{
			CHAR_DATA fontData;
			fontData.font = font;

			std::stringstream ss;
			ss << line;
			std::string word;
			ss >> word; // char
			ss >> word; // id;
			word = word.substr(word.find('=') + 1, word.size());
			fontData.id = std::stoi(word);

			ss >> word; // x;
			word = word.substr(word.find('=') + 1, word.size());
			fontData.x = std::stoi(word);

			ss >> word; // y;
			word = word.substr(word.find('=') + 1, word.size());
			fontData.y = std::stoi(word);

			ss >> word; // width;
			word = word.substr(word.find('=') + 1, word.size());
			fontData.width = std::stoi(word);

			ss >> word; // height;
			word = word.substr(word.find('=') + 1, word.size());
			fontData.height = std::stoi(word);

			ss >> word; // xOffset
			word = word.substr(word.find('=') + 1, word.size());
			fontData.xOffset = std::stoi(word);

			ss >> word; // yOffset
			word = word.substr(word.find('=') + 1, word.size());
			fontData.yOffset = std::stoi(word);

			ss >> word; // xAdvance
			word = word.substr(word.find('=') + 1, word.size());
			fontData.xAdvance = std::stoi(word);

			fontCharData.CharData.push_back(fontData);
		}
	}
	file.close();

	FontCharData.push_back(fontCharData);
}

CHAR_DATA FNTManager::charData(FONT_NAMES font, const char fontChar)
{
	FONT_CHAR_DATA matchedFontCharData = matchFontName(font);

	for (CHAR_DATA matchedCharData : matchedFontCharData.CharData)
	{
		if (matchedCharData.id == (int)fontChar)
		{
			return matchedCharData;
		}
	}
	print("FNTManager::charData()", "char not found: " + fontChar, true, 0);
}

Texture* FNTManager::fontTexture(FONT_NAMES font)
{
	FONT_CHAR_DATA matchedFontCharData = matchFontName(font);
	Texture* fontTexture = matchedFontCharData.FontTexture;

	if (!fontTexture) {
		print("FNTManager::fontTexture()", "not found for: " + font, true, 0);
		return nullptr;
	}

	return fontTexture;
}


float FNTManager::textWidth(const char* text, FONT_NAMES font)
{
	float width = 0;
	for (unsigned int i = 0; i < std::string(text).size(); i++)
	{
		width += charData(font, text[i]).xAdvance;
	}
	return width;
}