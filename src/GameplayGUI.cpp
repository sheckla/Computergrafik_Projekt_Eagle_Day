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
	// Update Everything else
	for (auto component : Components) component->update(delta);

	// Update LifeMeter/GunMeter percentage
	lifeMeter->percentage(ModelLoader::pPlayerPlane->hp / 100);
	gunMeter->percentage(ModelLoader::pPlayerPlane->gunHP / 100);
	ModelLoader::pPlayerPlane->hp = 100 * lifeMeter->percentage();

	// Enable/Disable Mousesteer circle
	(!ApplicationSettings::MOUSE_CONTROLS) ? mouseCircle->active(false) : mouseCircle->active(true);

	// Calculate Player/Enemy angle for Compass & Distance
	Vector enemyPos;
	if (ModelLoader::pAIPlane) enemyPos = ModelLoader::pAIPlane->transform().translation();
	else { enemyPos = ModelLoader::pEnemyPlane->transform().translation(); }
	Vector playerPos = ModelLoader::pPlayerPlane->getParts()[0]->transform().translation();
	float dist = (enemyPos - playerPos).length() * 30;

	Vector xzPlayer = Vector(playerPos.X, playerPos.Z, 0).normalize();
	Vector xzEnemy = Vector(enemyPos.X, playerPos.Z,0).normalize();

	float angle = acos(xzPlayer.dot(xzEnemy));
	Vector rotAxis = xzEnemy.cross(xzPlayer);
	rotAxis.normalize();

	Vector diff(0, 0, (enemyPos - playerPos).length());
	Vector rotDiff = Camera::rotateAxisAngle(diff, rotAxis, angle);

	Vector cdir = enemyPos - playerPos;
	cdir.normalize();
	Vector cup = ModelLoader::pPlayerPlane->getParts()[0]->transform().up().normalize();
	Vector cright = cdir.cross(cup);
	cright.normalize();
	cup = cright.cross(cdir);

	Vector rotDiffW;
	rotDiffW.X = cright.X * rotDiff.X + cup.X * rotDiff.Y + -cdir.X * rotDiff.Z;
	rotDiffW.Y = cright.Y * rotDiff.X + cup.Y * rotDiff.Y + -cdir.Y * rotDiff.Z;
	rotDiffW.Z = cright.Z * rotDiff.X + cup.Z * rotDiff.Y + -cdir.Z * rotDiff.Z;
	rotDiffW.normalize();

	float xCosRemap;
	xCosRemap = MathUtil::remapBounds(rotDiffW.X, -1, 1, -500, 500);

	float startX = ApplicationSettings::WIDTH / 2 + xCosRemap;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(0) << dist * 0.1 << "m";
	std::string xzCrossText{ ss.str() };

	// Translate compass & compassText
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

	// Altitude- & Speedmeter Parametrisierung
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

	// GunHP Anzeige
	GUIConstantQuad* gunQuad = new GUIConstantQuad(ApplicationSettings::WIDTH - 355, ApplicationSettings::HEIGHT - 175, 400, 75);
	gunQuad->constantColorMode(true);
	gunQuad->color(COL_DARK);
	Components.push_back(gunQuad);
	GUISlider* gunSlider = new GUISlider(ApplicationSettings::WIDTH - 355, ApplicationSettings::HEIGHT - 175, 250, 75, 10, "GUN");
	Components.push_back(gunSlider);
	gunSlider->percentage(1);
	gunSlider->enableSliding(false);
	gunMeter = gunSlider;

	// Compass Text
	compassText = new GUIText(ApplicationSettings::WIDTH / 2, ApplicationSettings::HEIGHT - 75, "                              ");
	compassText->scale(Vector(0.5, 0.5, 0));
	Components.push_back(compassText);

	// Compass icon
	compass = new GUITexture(ApplicationSettings::WIDTH / 2 - 30, ApplicationSettings::HEIGHT - 100, new Texture(ASSETS "img/enemy_icon.png"), true);;
	compass->width(30);
	compass->height(30);
	Components.push_back(compass);
}

