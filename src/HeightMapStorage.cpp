#include "HeightMapStorage.h"
#include <stdio.h>
#include <string>

HeightMapStorage::HeightMapStorage(char* fname) {
	for (int i = 0; i < 100; i++) {
		std::string temp = std::to_string(i);
		char charBuf[100] = "";
		//char* fname = "G:/VisualStudioProjects/cgprakt5/assets/waves3/";
		const char* num = temp.c_str();
		char* dataType = ".bmp";
		strcat(charBuf, fname);
		strcat(charBuf, num);
		strcat(charBuf, dataType);
		//std::cout << charBuf << std::endl;
		Water_Frames[i].load(charBuf);
	}
}