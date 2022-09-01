/*
 * PostProcessingShader
 * - Shadereinheit fuer komplette Bildmanipulationen
 * - Gausscher Weichzeichner
 * - Farbfilter- & Anpassung
 */

#ifndef ProcessingShader_h
#define ProcessingShader_h

#include "camera.h"
#include "baseshader.h"
#include "texture.h"

class PostProcessingShader : public BaseShader
{
public:
    PostProcessingShader();
    virtual void activate(const BaseCamera& Cam) const;
    void screenTexture(Texture* tex);
    void gaussianBlur(bool b);
    void elapsedTime(float t);
    void shake(bool b);
    void shakeTime(float f);
    void hp(float hp);
private:
    GLint EyePosLoc;
    GLint ModelMatLoc;
    GLint ModelViewProjMatLoc;
    GLint ScreenTextureLoc;
    GLint GaussianBlurLoc;
    GLint ElapsedTimeLoc;
    GLint TimeMaxPostProcessingLoc;
    GLint ShakeLoc;
    GLint ShakeTimeLoc;
    GLint HPLoc;

    Texture* ScreenTexture;
    bool GaussianBlur;
    float ElapsedTime;
    bool Shake = false;
    float ShakeTime = 0;
    float HP = -1;
};

#endif
