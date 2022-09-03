/*
 * GUIChar
 * - einzelnes Zeichen
 * - CHAR_DATA muss mittels einer entsprechenden .fnt Datei uebergeben werden 
 *	 (Wird mittels FNTManager geladen)
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
	CHAR_DATA data;
	void updateBuffers() override;
	void updateBounds() override;
public:
	GUIChar(float startX, float startY, CHAR_DATA data);
	virtual ~GUIChar() override;
	void updateFont(float startX, float startY, CHAR_DATA d);
};
#endif