#include "GUITexture.h"

void GUITexture::updateBounds()
{
    if (!KeepImageSize)
    {
        Width = 100;
        Height = 100;
    }

    GUIConstantQuad::updateBounds();
}

GUITexture::GUITexture(int startX, int startY, Texture* tex, bool keepImageSize) :
    GUIConstantQuad(startX, startY, tex->width(), tex->height())
{
	this->KeepImageSize = keepImageSize;
    Shader->isGUITex(true);
    constantColorMode(false);
	texture(tex);
}

GUITexture::~GUITexture()
{
    if (pTex)
    {
	    pTex->deactivate();
	    delete pTex;
    }
}

void GUITexture::texture(Texture* tex)
{
    this->pTex = tex;

    if (!KeepImageSize)
    {
        Width = STATIC_HEIGHT;
        Height = STATIC_WIDTH;
    } else
    {
    this->Width = pTex->width();
    this->Height = pTex->height();
    }

    GUIShader* pGUI = dynamic_cast<GUIShader*>(this->Shader);
    pGUI->texture(this->pTex);
}

