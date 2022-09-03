/*
 * GUIShader
 * - Shadereinheit fuer GUI-Elemente
 * - verwaltet die verschiedenen Parametrisierungen bezueglich der GUI-Elemente
 */
#ifndef GUIShader_h
#define GUIShader_h

#include "color.h"
#include "camera.h"
#include "baseshader.h"
#include "texture.h"

class GUIShader : public BaseShader
{
public:

    GUIShader();
    ~GUIShader();
    void activate() const override;

    //setter
    void texture(Texture* tex);
    void color(Color c);
    void constantColorMode(bool b);
    void isGUITex(bool b);
    void mouseoverHighlight(bool b);
    void mouseInside(bool b);
    void isFont(bool b);
private:
    GLint TexLoc;
    GLint ColorLoc;
    GLint ConstantColorModeLoc;
    GLint IsGUITexLoc;
    GLint MouseoverHighlightLoc;
    GLint MouseInsideLoc;
    GLint IsFontLoc;

    Texture* pTex;
    Color Col;
    bool ConstantColorMode;
    bool IsGUITex;
    bool MouseoverHighlight;
    bool MouseInside;
    bool IsFont;
};

#endif
