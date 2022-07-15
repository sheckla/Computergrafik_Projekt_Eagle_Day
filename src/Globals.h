#ifndef Globals_HPP
#define Globals_HPP

#include <iostream>
#include <stdio.h>
#pragma once
template<typename T>
void printerr(const char* error, T msg);

void test();

#ifdef WIN32
#define ASSETS "../../assets/"
#else
#define ASSETS "../assets/"
#endif

#endif

template<typename T>
inline void printerr(const char* error, T msg)
{
	std::cout << "[" << error << "] - " << msg << "\n";
}
