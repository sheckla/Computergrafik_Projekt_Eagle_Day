#include "GUILoader.h"

GUILoader* GUILoader::pGUILoader = nullptr;
std::list<GUIElement*>* GUILoader::guis;

GUILoader& GUILoader::instance()
{
	if (!pGUILoader) pGUILoader = new GUILoader();
	return *pGUILoader;
}

bool GUILoader::init(std::list<GUIElement*>* guis)
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
	gTex->startIsCenter(true);
	gTex->followMouse(true);
	guis->push_back(gTex);
}

bool GUILoader::GUI()
{
	GUITexture* ui = new GUITexture(0, 0, new Texture(ASSETS "ui.png"), true);
	ui->color(Color(0.5, 0, 0));
	guis->push_back(ui);
	return true;
}