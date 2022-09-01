/*
 * Erweiterung zu GUIConstantQuad
 * - Texturen/Bilder koennen uebergeben werden
 * - KeepImageSize=true uebernimmt die echte groesse des Bildes
 * - FillScreen=true spreizt das Bild ueber das ganze Fenster
 */

#ifndef GUITexture_h
#define GUITexture_h

#include "GUIConstantQuad.h"
#include "Texture.h"

constexpr int STATIC_WIDTH = 100;
constexpr int STATIC_HEIGHT = 100;

class GUITexture : public GUIConstantQuad
{
protected:
	Texture* pTex;
	bool KeepImageSize = false;
	bool FillScreen = false;
	bool Active = true;
	// bei KeepImageSize=false
	void updateBounds() override;
	void deactivateTex();
public:
	GUITexture(int startX, int startY, Texture* tex, bool keepImageSize, bool fillScreen = false);
	~GUITexture() override;

	void draw() override;
	void texture(Texture* tex);
	void handleMouseEvents() override;
	void active(bool b);
};

#endif
