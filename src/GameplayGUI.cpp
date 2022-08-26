#include "GameplayGUI.h"

#include <sstream>

#include "GUIButton.h"
#include "GUILoadingMeter.h"
#include "GUINumericPointerMeter.h"
#include "GUITexture.h"

GameplayGUI::GameplayGUI(GLFWwindow* window) :   ApplicationGUIPrototype(window)
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
		std::stringstream ss;
		ss << "Speed: ";

		if (ModelLoader::pPlayerPlane)
		{
			ss << ModelLoader::pPlayerPlane->getSpeed();
		}

		std::string speedString{ ss.str() };
		speedMeterText->text(speedString.c_str());

		//delete speedMeterText;
		//speedMeterText->text(speedString.c_str());
		component->draw();
	}
}

void GameplayGUI::update(float delta)
{

}

void GameplayGUI::init()
{
	// Maus-Kreis
	GUITexture* mouseCircle = new GUITexture(0, 0, new Texture(ASSETS "img/circle.png"), false, false);
	mouseCircle->scale(Vector(1, 1, 0));
	mouseCircle->width(100.0f);
	mouseCircle->height(100.0f);
	mouseCircle->centred(true);
	mouseCircle->followMouse(true);
	mouseCircle->constantColorMode(true);
	mouseCircle->color(Color(1, 1, 1));
	Components.push_back(mouseCircle);


	Vector barWidths = Vector(110, 80, 60);
	Vector barHeights = Vector(13, 9, 4);
	Vector cornerSize = Vector(2, 3, 0);
	Vector numberScale(0.05, 0.05, 0);

	// Geschwindigkeitsmeter
	GUINumericPointerMeter* speedMeterLeft = new GUINumericPointerMeter(50, 20, true, 5,
		barWidths, barHeights,
		cornerSize, numberScale);

	speedMeterText = new GUIText(0, 600, "dsadasd");
	Components.push_back(speedMeterLeft);
	Components.push_back(speedMeterText);

	// Hoehenemeter
	GUINumericPointerMeter* altitudeMeterRight = new GUINumericPointerMeter(50, 20, false, 5,
		barWidths, barHeights,
		cornerSize, numberScale);
	Components.push_back(altitudeMeterRight);

	// Lebensanzeige
	lifeMeter = new GUILoadingMeter(ASPECT_WIDTH / 2 - 100, ASPECT_HEIGHT - 50, 200, 50, 10);
	Components.push_back(lifeMeter);
	lifeMeter->percentage(0.5);
}

