#include "GameplayGUI.h"

#include <sstream>

#include "ApplicationSettings.h"
#include "GUIButton.h"
#include "GUILoadingMeter.h"
#include "GUINumericPointerMeter.h"
#include "GUISlider.h"
#include "GUITexture.h"
#include "MathUtil.h"

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
		component->draw();
	}
}

void GameplayGUI::update(float delta)
{
	for (auto component : Components) component->update(delta);
	lifeMeter->percentage(ModelLoader::pPlayerPlane->hp / 100);
	ModelLoader::pPlayerPlane->hp = 100 * lifeMeter->percentage();
	(!ApplicationSettings::MOUSE_CONTROLS) ? mouseCircle->active(false) : mouseCircle->active(true);

	Vector enemyPos;
	if (ModelLoader::pAIPlane) enemyPos = ModelLoader::pAIPlane->getPosition();
	else { enemyPos = ModelLoader::pEnemyPlane->transform().translation(); }
	print("enemy", enemyPos);

	Vector playerPos = ModelLoader::pPlayerPlane->getParts()[0]->transform().translation();
	float dist =(Vector(playerPos.X, 0, playerPos.Z).length() * Vector(enemyPos.X, 0, enemyPos.Z).length());

	float planeCos = ModelLoader::pPlayerPlane->totalRudderRotation;
	float xyDot = playerPos.X * enemyPos.X + playerPos.Z * enemyPos.Z;
	float xzDet = playerPos.X * enemyPos.Z + playerPos.Z * enemyPos.X;
	float signedXYAngle = atan2(xzDet, xyDot);
	float xzCos = planeCos - signedXYAngle;

	while (planeCos < PI) planeCos += PI;
	while (planeCos > PI) planeCos -= PI;
	while (xzCos > PI*2) xzCos -= PI*2;
	while (xzCos < PI*2) xzCos += PI*2;

	/*if (xzCos > 2*PI && xzCos < 3*PI) ss << "LEFT";
	if (xzCos <= 4*PI && xzCos >= 3*PI) ss << "RIGHT";*/

	float cosRemap;
	if (xzCos <= 3*PI)
	{
		cosRemap = MathUtil::remapBounds(xzCos, 2*PI, 3*PI, 0, 10);
		cosRemap = MathUtil::remapBounds(cosRemap, 0, 10, 0, 500);
		
	} else
	{
		cosRemap = MathUtil::remapBounds(3*PI + (xzCos-3*PI), 3*PI , 4*PI, 10, 00);
		cosRemap = -MathUtil::remapBounds(cosRemap, 0, 10, 0, 500);
		
	}
	float startX = ApplicationSettings::WIDTH / 2 + cosRemap ;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(0) << dist * 0.1 << "m";
	std::string xzCrossText{ ss.str() };
	compassText->startPixel(Vector(startX - 15, compass->startPixel().Y + 55, 0));
	compassText->text(xzCrossText.c_str());
	compass->startPixel(Vector(startX , compass->startPixel().Y, 0));
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
	this->mouseCircle = mouseCircle;
	Components.push_back(mouseCircle);


	Vector barWidths = Vector(110, 80, 60);
	Vector barHeights = Vector(13, 9, 4);
	Vector cornerSize = Vector(2, 3, 0);
	Vector numberScale(0.45, 0.45, 0);

	// Geschwindigkeitsmeter
	GUINumericPointerMeter* speedMeterLeft = new GUINumericPointerMeter(50, 20, true, 5,
		barWidths, barHeights,
		cornerSize, numberScale);
	Components.push_back(speedMeterLeft);

	// Hoehenemeter
	GUINumericPointerMeter* altitudeMeterRight = new GUINumericPointerMeter(50, 20, false, 5,
		barWidths, barHeights,
		cornerSize, numberScale);
	Components.push_back(altitudeMeterRight);

	// Lebensanzeige
	GUIConstantQuad* lifeQuad = new GUIConstantQuad(ApplicationSettings::WIDTH - 355, ApplicationSettings::HEIGHT - 100, 400, 75);
	lifeQuad->constantColorMode(true);
	lifeQuad->color(COL_DARK);
	Components.push_back(lifeQuad);
	lifeMeter = new GUISlider(ApplicationSettings::WIDTH - 355, ApplicationSettings::HEIGHT - 100, 250, 75, 10, "HP");
	Components.push_back(lifeMeter);
	lifeMeter->percentage(1);
	lifeMeter->enableSliding(false);

	compassText = new GUIText(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 75, "                              ");
	compassText->scale(Vector(0.5, 0.5, 0));
	Components.push_back(compassText);

	compass = new GUITexture(ApplicationSettings::WIDTH / 2 - 30, ApplicationSettings::HEIGHT - 100, new Texture(ASSETS "img/enemy_icon.png"), true);;
	compass->width(30);
	compass->height(30);
	Components.push_back(compass);
}

