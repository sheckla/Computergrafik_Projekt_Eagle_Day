#include "HeightMapStorage.h"
#include <stdio.h>
#include <string>

HeightMapStorage::HeightMapStorage(char* fname) {
	//loads all displacement textures
	for (int i = 0; i < 100; i++) {
		std::string temp = std::to_string(i);
		char charBuf[100] = "";
		const char* num = temp.c_str();
		char* dataType = ".bmp";
		strcat(charBuf, fname);
		strcat(charBuf, num);
		strcat(charBuf, dataType);
		Water_Frames[i].load(charBuf);
	}
}