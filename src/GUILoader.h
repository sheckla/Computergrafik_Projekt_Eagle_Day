#ifndef GUILoader_h
#define GUILoader_h
#include <list>


class GUIBaseComponent;

class GUILoader
{
	static GUILoader* pGUILoader;
public:
	static std::list<GUIBaseComponent*>* guis;
	static GUILoader& instance();
	static bool init(std::list<GUIBaseComponent*>* guis);

	static bool crossHair();
	static bool GUI();
};

#endif 
