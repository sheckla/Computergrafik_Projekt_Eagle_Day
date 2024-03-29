/*
 * ApplicationGUIPrototype
 * - Basisklasse fuer eine GUI-Application Komponente (bsp GameplayGUI, EscapeMenuGUI etc.)
 */

#ifndef ApplicationGUIPrototype_h
#define ApplicationGUIPrototype_h

#include "Globals.h"
#include <list>
#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"

class ApplicationGUIPrototype
{
protected:
	GLFWwindow* Window;
	std::list<GUIBaseComponent*> Components = {};
	bool Active;
public:
	ApplicationGUIPrototype(GLFWwindow* window) : Window(window), Active(false)
	{
	}

	virtual ~ApplicationGUIPrototype()
	{
	};

	virtual void draw()
	{
		for (auto component : Components) component->draw();
	}

	void active(bool b)
	{
		Active = b;
	}

	virtual void update(float delta) = 0;

	bool active() { return Active; }

	virtual void init() = 0;

	std::list<GUIBaseComponent*> components();
};

#endif