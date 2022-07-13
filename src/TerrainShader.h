#ifndef TerrainShader_hpp
#define TerrainShader_hpp

#include <stdio.h>
#include <assert.h>
#include "PhongShader.h"


class TerrainShader : public PhongShader
{
public:
    enum {
        DETAILTEX0=0,
        DETAILTEX1,
        DETAILTEX_COUNT
    };
    
    TerrainShader(const std::string& AssetDirectory);
    virtual ~TerrainShader() { std::cout << "DELETED!" << std::endl; }
    virtual void activate(const BaseCamera& Cam) const;
    virtual void deactivate() const;
    
    const Texture* detailTex(unsigned int idx) const { assert(idx<DETAILTEX_COUNT); return DetailTex[idx]; }
    const Texture* mixTex() const { return MixTex; }

    void detailTex(unsigned int idx, const Texture* pTex) { assert(idx<DETAILTEX_COUNT); DetailTex[idx] = pTex; }
    void mixTex(const Texture* pTex) { MixTex = pTex; }

    void setResolution(int res) { Resolution = res; }

    void scaling(const Vector& s) { Scaling = s; }
    const Vector& scaling() const { return Scaling; }

private:
    void activateTex(const Texture* pTex, GLint Loc, int slot) const;

    const Texture* MixTex;
    int Resolution;
    const Texture* DetailTex[DETAILTEX_COUNT];
    Vector Scaling;
    // shader locations
    int DetailTexK;

    GLint ResolutionLoc;
    GLint MixTexLoc;
    GLint DetailTexLoc[DETAILTEX_COUNT];
    GLint ScalingLoc;
    GLint DetailTexKLoc; // DetailText Multiplier
};

#endif /* TerrainShader_hpp */
