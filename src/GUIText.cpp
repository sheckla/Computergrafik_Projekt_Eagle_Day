#include "GUIText.h"

#include <sstream>

GUIText::GUIText(float startX, float startY, const char* string) : startX(startX), startY(startY), string(string)
{
	int lineCount = 0;

	std::ifstream file;
	file.open(ASSETS "typography/arial/arial.fnt");

	std::string line;
	while (std::getline(file, line))
	{
		if (lineCount++ >= 4)
		{
			CharData fontData;

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

			data.push_back(fontData);
		}
	}
	file.close();
	text(string);
}

GUIText::~GUIText()
{
	for (GUIChar* gChar : chars) delete gChar;
	//delete string;
}

void GUIText::draw()
{
	for (int i = 0; i < chars.size(); i++)
	{
		chars[i]->draw();
	}
}

void GUIText::text(const char* string)
{
	if (!chars.empty()) {
		float xOffset = 0;
		// Iterate through string
		for (unsigned int i = 0; i < std::string(string).size(); i++)
		{
			// Check for matching CharData
			for (const CharData fData : data)
			{
				// Update Char
				if (fData.id == (int)std::string(string).at(i))
				{
					if (i <= chars.size() - 1)
					{
						chars.at(i)->updateFont(startX + xOffset, startY, fData);
					}
					xOffset += fData.xAdvance;
					break;
				}
			}
		}
		return;
	}

	std::string charText{ string };

	int xOffset = 0;
	for (int i = 0; i < charText.size(); i++)
	{
		for (const auto d : data)
		{
			if (d.id == (int)charText.at(i))
			{
				GUIChar* gChar = new GUIChar(startX+ (float)xOffset, startY, d);
				chars.push_back(gChar);

				xOffset += d.xAdvance;
				break;
			}
		}

	}

}
