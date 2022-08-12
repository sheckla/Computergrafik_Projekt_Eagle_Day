#ifndef GUILoader_h
#define GUILoader_h
#include <list>

#include "guiElement.h"

class GUILoader
{
	static GUILoader* pGUILoader;
public:
	static std::list<GUIElement*>* guis;
	static GUILoader& instance();
	static bool init(std::list<GUIElement*>* guis);

	static bool crossHair();
	static bool GUI();
};

#endif 
