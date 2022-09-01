#ifndef GUIButtonSwitch_h
#define GUIButtonSwitch_h

#include "GUIBaseComponent.h"
#include "GUIButton.h"

class GUIButtonSwitch : public GUIBaseComponent
{
	GUIButton* button;
	GUIText* onOffText;
	bool On = false;
public:
	GUIButtonSwitch(float startX, float startY, float width, float height);
	~GUIButtonSwitch() override;
	void draw() override;
	void update(float delta) override;
	bool on();
};

#endif