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
private:
    GLint EyePosLoc;
    GLint ModelMatLoc;
    GLint ModelViewProjMatLoc;
    GLint ScreenTextureLoc;
    GLint GaussianBlurLoc;
    GLint ElapsedTimeLoc;
    GLint TimeMaxPostProcessingLoc;

    Texture* ScreenTexture;
    bool GaussianBlur;
    float ElapsedTime;
};

#endif
