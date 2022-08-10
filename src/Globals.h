#ifndef Globals_HPP
#define Globals_HPP

#include <iostream>
#include <stdio.h>
#pragma once

template<typename T>
void print(const char* error, T msg);

#ifdef WIN32
#define ASSETS "../../assets/"
#define WORLEY_AMOUNT 128
#define MAX_TEXTURE_ARR_SIZE 128
#else
#define ASSETS "../assets/"
#endif

#endif

template<typename T>
inline void print(const char* error, T msg)
{
	std::cout << "[" << error << "] - " << msg << "\n";
}
