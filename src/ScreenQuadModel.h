#ifndef ScreenQuadModel_h
#define ScreenQuadModel_h

#include <stdio.h>

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "PostProcessingShader.h"
#include "Texture.h"

class ScreenQuadModel 
{
public:
    ScreenQuadModel();
    virtual ~ScreenQuadModel() {}
    virtual void draw(Camera& cam, Texture* tex);
    
protected:
    Texture noise;
    PostProcessingShader shader;
    unsigned int VAO, VBO;
    IndexBuffer IB;
    VertexBuffer VB;
};


#endif 
