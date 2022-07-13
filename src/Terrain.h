#ifndef Terrain_hpp
#define Terrain_hpp

#include <stdio.h>
#include "basemodel.h"
#include "texture.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "HeightMapStorage.h"
#include "TerrainShader.h"

class Terrain : public BaseModel
{
public:
    Terrain(HeightMapStorage* hms,int resolution,const char* HeightMap=NULL, const char* DetailMap1=NULL, const char* DetailMap2=NULL);
    virtual ~Terrain();
    bool load();

    virtual void shader( BaseShader* shader, bool deleteOnDestruction=false );
    virtual void draw(const BaseCamera& Cam);
    
    void update();
    float width() const { return Size.X; }
    float height() const { return Size.Y; }
    float depth() const { return Size.Z; }
    Vector& convert_heightmap_color_to_vec(const RGBImage* img, int x, int y) const;

    void width(float v) { Size.X = v; }
    void height(float v) { Size.Y = v; }
    void depth(float v) { Size.Z = v; }
    
    const Vector& size() const { return Size; }
    void size(const Vector& s) { Size = s; }

    Matrix getTransform() { return this->Transform; }

    void finishVB() { this->VB->end(); }
    void finishIB() { this->IB->end(); }

protected:
    void applyShaderParameter();
    
    Texture MixTex;

    VertexBuffer* VB;
    IndexBuffer* IB;
    Texture DetailTex[2];
    
    Texture HeightTex;
    Vector Size;
    int Resolution;

private:
    //Texture Water_Frames[100];
    HeightMapStorage* HeightMaps;
};



#endif /* Terrain_hpp */
