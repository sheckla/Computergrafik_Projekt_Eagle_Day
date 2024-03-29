/*
 * MouseLogger
 * - statische Instanz
 * - erlaubt globale Einsicht auf die aktuelle Mausposition pro Frame
 *	 in normalisierten oder standartmaessigen Bildschirm- bzw Pixelkoordinaten
 *
 */

#ifndef MouseLogger_h
#define MouseLogger_h
#include  "Globals.h"
class MouseLogger
{
	static MouseLogger* pMouseLogger;
	static double X, Y;
public:
	static MouseLogger& instance();
	static void update(double x, double y);

	// Bildschirmkoordinaten in PX
	static double x();
	static double y();

	// Bildschirmkoordinaten normalisiert [-1,-1] bis [1,1]
	static double normY();
	static double normX();
};
#endif
