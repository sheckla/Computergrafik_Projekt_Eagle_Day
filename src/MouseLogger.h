#ifndef MouseLogger_h
#define MouseLogger_h
#include  "Globals.h"
class MouseLogger
{
	static MouseLogger* pMouseLogger;
	static double x, y;
public:
	static MouseLogger& instance();
	static void update(double x, double y);
	static double getX();
	static double getY();
};
#endif
