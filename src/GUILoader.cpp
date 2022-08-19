#include "GUILoader.h"

#include "GUITexture.h"

GUILoader* GUILoader::pGUILoader = nullptr;
std::list<GUIBaseComponent*>* GUILoader::guis;

GUILoader& GUILoader::instance()
{
	if (!pGUILoader) pGUILoader = new GUILoader();
	return *pGUILoader;
}

bool GUILoader::init(std::list<GUIBaseComponent*>* guis)
{
	instance().guis = guis;
	return true;
}

bool GUILoader::crossHair()
{
	GUITexture* gTex = new GUITexture(0, 0, new Texture(ASSETS "circle.png"), false);
	gTex->scale(Vector(1, 1, 0));
	gTex->width(100);
	gTex->height(100);
	gTex->centred(true);
	gTex->followMouse(true);
	gTex->constantColorMode(true);
	gTex->color(Color(1, 1, 1));
	guis->push_back(gTex);

	return true;
}

bool GUILoader::GUI()
{
	GUITexture* ui = new GUITexture(0, 0, new Texture(ASSETS "ui.png"), true);
	ui->color(Color(0.5, 0, 0));
	guis->push_back(ui);
	return true;
}