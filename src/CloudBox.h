/*
 * CloudBox
 * - Creates a container in which the CloudShader draws the volumetric clouds
 */

#ifndef CloudBox_h
#define CloudBox_h

#include <stdio.h>

#include <stdio.h>
#include "Globals.h"
#include "basemodel.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "Texture.h"
#include "CloudShader.h"
#include <sstream>

class CloudBox : public BaseModel
{
public:
    CloudBox(float Width = 1, float Height = 1, float Depth = 1,
        bool bottomOff = false, bool topOff = false, bool frontOff = false, bool backOff = false, bool rightOff = false, bool leftOff = false);
    virtual ~CloudBox() {}

    virtual void draw(const BaseCamera& Cam);
    void load(const char* worleyPath, const char* noisePath);
    AABB& boundingBox();
    void applyShaderParameter();
    AABB bounds;
protected:
    VertexBuffer VB;
    IndexBuffer IB;
    Texture DetailTex[WORLEY_AMOUNT];
    Texture noise;
    // Bei mehreren CloudBoxen DetailTex evtl aus der Klasse extrahieren
};


#endif