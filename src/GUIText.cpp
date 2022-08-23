#include "GUIText.h"

#include <sstream>

GUIText::GUIText(float startX, float startY, char* string) : string(string)
{
	int lineCount = 0;

	std::ifstream file;
	file.open(ASSETS "typgraphy/arial.fnt");

	std::string line;
	while (std::getline(file, line))
	{
		if (lineCount++ >= 4)
		{
			FontData fontData;

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
			data.push_back(fontData);
		}
	}
	file.close();
	text(string);
}

GUIText::~GUIText()
{
}

void GUIText::draw()
{
	for (GUIChar* gChar : chars) gChar->draw();
}

void GUIText::text(char* string)
{
	if (chars.size() > 0) {
		for (auto ch : chars) delete ch;
		chars.clear();
	}
	for (auto c : data)
	{
		//print("x", c.x);
	}

	std::string charText{ string };

	for (int i = 0; i < charText.size(); i++)
	{
		//print("i", (int)charText.at(i));

		for (auto d : data)
		{
			if (d.id == (int)charText.at(i))
			{
				GUIChar* gChar = new GUIChar(66 * i, 100, 22, 66, d);
				chars.push_back(gChar);
				break;
			}
		}

	}

}
