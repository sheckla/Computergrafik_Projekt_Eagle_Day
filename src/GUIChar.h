/*
 * GUIChar
 * - einzelnes Zeichen
 * - CharData muss mittels einer entsprechenden .fnt Datei uebergeben werden
 */
#ifndef GUIChar_h
#define GUIChar_h
#include <vector>

#include "FNTManager.h"
#include "GUIBaseComponent.h"
#include "GUIConstantQuad.h"
#include "GUITexture.h"


class GUIChar : public GUITexture
{
	CharData data;
	void updateBuffers() override;
public:
	GUIChar(float startX, float startY, CharData data);
	virtual ~GUIChar() override;
	void updateFont(float startX, float startY, CharData d);
};

#endif