#include "MouseLogger.h"

#include "ApplicationSettings.h"

MouseLogger* MouseLogger::pMouseLogger = nullptr;
double MouseLogger::X = 0;
double MouseLogger::Y = 0;

MouseLogger& MouseLogger::instance()
{
	if (pMouseLogger) pMouseLogger = new MouseLogger();
	return *pMouseLogger;
}

void MouseLogger::update(double x, double y)
{
	MouseLogger::X = x;
	MouseLogger::Y = y;
}

double MouseLogger::x()
{
	return MouseLogger::X;
}

double MouseLogger::y()
{
	return MouseLogger::Y;
}

double MouseLogger::normY()
{
	return -(((float)MouseLogger::Y / ApplicationSettings::HEIGHT * 2) - 1);
}

double MouseLogger::normX()
{
	return ((float)MouseLogger::X / ApplicationSettings::WIDTH * 2) - 1;
}