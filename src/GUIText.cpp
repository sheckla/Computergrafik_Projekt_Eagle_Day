#include "GUIText.h"


#include <sstream>


void GUIText::applyCharParameter()
{
	for (auto gChar : textChars)
	{
		gChar->scale(Scale);
		gChar->width(Width);
		gChar->height(Height);
		gChar->centred(Centred);
	}
}

GUIText::GUIText(float startX, float startY, const char* string, FONT_NAMES font) : font(font), textString(string), startX(startX), startY(startY)
{
	text(string);
}

GUIText::~GUIText()
{
	for (GUIChar* gChar : textChars) delete gChar;
	//delete textString;
}

void GUIText::draw()
{
	for (int i = 0; i < textChars.size(); i++)
	{
		textChars[i]->draw();
	}
}

void GUIText::update(float delta)
{
}

void GUIText::text(const char* string)
{
	std::string textString{ string };
	float xOffset = 0;

	// Replace/Update existing chars
	if (!textChars.empty()) {
		// Iterate through textString
		for (unsigned int i = 0; i < textString.size(); i++)
		{
			CHAR_DATA charData = FNTManager::charData(font, textString[i]);
			if (i < textChars.size()) textChars.at(i)->updateFont(startX + xOffset, startY, charData);

			xOffset += charData.xAdvance * CharSpace;
			TotalWidth = xOffset;
		}

		for (unsigned int j = textString.size(); j < textChars.size(); j++)
		{
			CHAR_DATA charData = FNTManager::charData(font, ' ');
			textChars.at(j)->updateFont(0, 0, charData);
		}
		applyCharParameter();
		return;
	}


	// Load new chars
	for (int i = 0; i < textString.size(); i++)
	{
		CHAR_DATA charData = FNTManager::charData(font, textString[i]);
		GUIChar* gChar = new GUIChar(startX + xOffset, startY, charData);
		textChars.push_back(gChar);

		xOffset += charData.xAdvance * CharSpace;
		TotalWidth = xOffset;
	}
	applyCharParameter();
}

void GUIText::scale(Vector scale)
{
	Scale = scale;
	CharSpace = scale.X;
	for (auto gChar : textChars) gChar->scale(scale);
}

void GUIText::centred(bool b)
{
	Centred = b;
	//for (auto gChar : textChars) gChar->centred(b);
}

void GUIText::charSpace(float f)
{
	CharSpace = f;
	text(textString);
}

void GUIText::width(float w)
{
	Width = w;
	//for (auto gChar : textChars) gChar->width(w);
}

void GUIText::height(float h)
{
	Height = h;
	//for (auto gChar : textChars) gChar->height(h);
}

float GUIText::height()
{
	return Height;
}

float GUIText::totalWidth()
{
	return TotalWidth;
}

void GUIText::startPixel(Vector v)
{
	this->startX = v.X;
	this->startY = v.Y;
}

Vector GUIText::startPixel()
{
	return Vector(startX, startY, 0);
}
