#include "texture.h"
#include <stdio.h>

#pragma once
class HeightMapStorage
{
public:
	Texture Water_Frames[100];
	HeightMapStorage(char* fname);
	float time=0;
};

