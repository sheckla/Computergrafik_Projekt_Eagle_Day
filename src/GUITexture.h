#ifndef GUITexture_h
#define GUITexture_h

#include "GUIConstantQuad.h"
#include "Texture.h"

constexpr int STATIC_WIDTH = 100;
constexpr int STATIC_HEIGHT = 100;

/*
 * Erweiterung zu GUIConstantQuad
 * - Texturen/Bilder koennen uebergeben werden
 * - KeepImageSize=true uebernimmt die echte groesse des Bildes
 */
class GUITexture : public GUIConstantQuad
{
	Texture* pTex;
	bool KeepImageSize = false;

	// bei KeepImageSize=false
	void updateBounds() override;
public:
	GUITexture(int startX, int startY, Texture* tex, bool keepImageSize);
	~GUITexture() override;
	void texture(Texture* tex);
};

#endif
