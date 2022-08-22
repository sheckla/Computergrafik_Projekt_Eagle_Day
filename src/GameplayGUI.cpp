#include "GameplayGUI.h"

#include "GUIButton.h"
#include "GUINumericPointerMeter.h"
#include "GUITexture.h"

GameplayGUI::GameplayGUI(GLFWwindow* window) : b(true),  ApplicationGUIPrototype(window)
{

}

GameplayGUI::~GameplayGUI()
{
	for (auto component : Components) delete component;
}

void GameplayGUI::draw()
{

	for (GUIBaseComponent* component : Components) {

		// attach plane to meter
		GUINumericPointerMeter* meter = dynamic_cast<GUINumericPointerMeter*>(component);
		if (meter && !meter->plane) {
			meter->plane = ModelLoader::pPlayerPlane;
			if (!meter->plane) print("GUINumericPointerMeter", "no plane!", true, 0);
		}
		component->draw();
	}
}

void GameplayGUI::update(float delta)
{
}

void GameplayGUI::init()
{
	GUITexture* mouseCircle = new GUITexture(0, 0, new Texture(ASSETS "circle.png"), false, false);
	mouseCircle->scale(Vector(1, 1, 0));
	mouseCircle->width(100.0f);
	mouseCircle->height(100.0f);
	mouseCircle->centred(true);
	mouseCircle->followMouse(true);
	mouseCircle->constantColorMode(true);
	mouseCircle->color(Color(1, 1, 1));
	Components.push_back(mouseCircle);

	GUINumericPointerMeter* speedMeterLeft = new GUINumericPointerMeter(50, 20, true, 5,
		Vector(110, 80, 60), Vector(13, 9, 4),
		Vector(2, 3, 0), Vector(0.05, 0.05, 0));
	Components.push_back(speedMeterLeft);

	GUINumericPointerMeter* altitudeMeterRight = new GUINumericPointerMeter(50, 20, false, 5,
		Vector(110, 80, 60), Vector(13, 9, 4),
		Vector(2, 3, 0), Vector(0.05, 0.05, 0));
	Components.push_back(altitudeMeterRight);
}

