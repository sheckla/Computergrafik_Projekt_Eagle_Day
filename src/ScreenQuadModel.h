/*
 * ScreenQuadModel
 * - einfaches Quad, welches sich ueber die normalisierten Bildschirmkoordinaten erstreckt
 * - Der ganze Bildschirm wird mit diesem Quad gefuellt
 * - der hier vorhandene Shader ist fuer die PostProcessing Bildberarbeitung zustaendig
 */
#ifndef ScreenQuadModel_h
#define ScreenQuadModel_h
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "PostProcessingShader.h"
#include "Texture.h"

class ScreenQuadModel 
{
public:
    ScreenQuadModel();
    virtual ~ScreenQuadModel();
    virtual void draw(Camera& cam, Texture* tex);
    void gaussianBlur(bool b);
    PostProcessingShader* shader();
protected:
    Texture noise;
    PostProcessingShader* Shader;
    unsigned int VAO, VBO;
    IndexBuffer IB;
    VertexBuffer VB;
};


#endif 
