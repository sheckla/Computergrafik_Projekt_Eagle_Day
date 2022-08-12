#include "MouseLogger.h"

MouseLogger* MouseLogger::pMouseLogger = nullptr;
double MouseLogger::x = 0;
double MouseLogger::y = 0;

MouseLogger& MouseLogger::instance()
{
	if (pMouseLogger) pMouseLogger = new MouseLogger();
	return *pMouseLogger;
}

void MouseLogger::update(double x, double y)
{
	MouseLogger::x = x;
	MouseLogger::y = y;
}

double MouseLogger::getX()
{
	return MouseLogger::x;
}

double MouseLogger::getY()
{
	return MouseLogger::y;
}