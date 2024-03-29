/*
* GUIButtonSwitch
* - Klickbarer Button mit binaerer On/Off Abfrage
* - Texturen werden je nach Zustand veraendert
* 
* - Eingesetzt im OptionsGUI
*/

#ifndef GUIButtonSwitch_h
#define GUIButtonSwitch_h

#include "GUIBaseComponent.h"
#include "GUIButton.h"

class GUIButtonSwitch : public GUIBaseComponent
{
	GUIButton* button;
	GUITexture* onTexture;
	GUITexture* offTexture;
	GUIText* onOffText;
	bool On = false;
public:
	GUIButtonSwitch(float startX, float startY, float width, float height, const char* text);
	~GUIButtonSwitch() override;
	void draw() override;
	void update(float delta) override;
	bool on();
	void on(bool b);
};

#endif