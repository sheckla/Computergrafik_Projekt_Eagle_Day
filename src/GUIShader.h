
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
    ~GUIShader() override;
    void activate() const override;
    void texture(Texture* tex);
    void color(Color c);
    void constantColorMode(bool b);
    void isGUITex(bool b);
private:
    GLint TexLoc;
    GLint ColorLoc;
    GLint ConstantColorModeLoc;
    GLint IsGUITexLoc;

    Texture* pTex;
    Color Col;
    bool ConstantColorMode;
    bool IsGUITex;
};

#endif
