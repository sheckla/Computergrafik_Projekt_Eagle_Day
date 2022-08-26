#include "FNTManager.h"

#include <fstream>
#include <sstream>
#include <string>

#include "Globals.h"

FNTManager* FNTManager::pFNTManager = nullptr;

FNTManager& FNTManager::instance()
{
	if (!pFNTManager) pFNTManager = new FNTManager();
	return *pFNTManager;
}

void FNTManager::loadFont(const char* path)
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

			//data.push_back(fontData);
		}
	}
	file.close();
}
