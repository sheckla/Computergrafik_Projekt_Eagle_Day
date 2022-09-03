#include "GUITexture.h"
#include "ApplicationSettings.h"
#include "GUINumericPointerMeter.h"
#include "MathUtil.h"

void GUITexture::updateBounds()
{
    if (!KeepImageSize)
    {
        Width = 100;
        Height = 100;
    }

    if (FillScreen)
    {
        Width = MathUtil::remapBounds(Width, 0, Width, 0, ApplicationSettings::WIDTH);
        Height = MathUtil::remapBounds(Height, 0, Height, 0, ApplicationSettings::HEIGHT);
    }

    GUIConstantQuad::updateBounds();
}

void GUITexture::deactivateTex()
{
    if (pTex)
    {
        pTex->deactivate();
        delete pTex;
    }
}

GUITexture::GUITexture(int startX, int startY, Texture* tex, bool keepImageSize, bool fillScreen) :
    GUIConstantQuad(startX, startY, tex->width(), tex->height())
{
	this->KeepImageSize = keepImageSize;
    this->FillScreen = fillScreen;
    if (FillScreen) centred(true);
    Shader->isGUITex(true);
    constantColorMode(false);
	texture(tex);
}

GUITexture::~GUITexture()
{
    //deactivateTex();
}

void GUITexture::draw()
{
    if (!Active) return;
    GUIConstantQuad::draw();
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

void GUITexture::handleMouseEvents()
{
    (mouseInside()) ? Shader->mouseInside(true) : Shader->mouseInside(false);
    GUIConstantQuad::handleMouseEvents();
}

void GUITexture::active(bool b)
{
    Active = b;
}

