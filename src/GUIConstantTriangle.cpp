#include "GUIConstantTriangle.h"
#include "ApplicationSettings.h"
#include "MouseLogger.h"

void GUIConstantTriangle::updateBuffers()
{

    if (Flip)
    {
        rotate180();
        return;
    }

    /*
    *     / B
    *   A   |
    *     \ C
    */
	Vector A = Vector(LowerBound.X, LowerBound.Y + (UpperBound.Y - LowerBound.Y)/2, 0);
	Vector B = Vector(UpperBound.X, UpperBound.Y, 0);
	Vector C = Vector(UpperBound.X, LowerBound.Y , 0);
    if (VB->vertices().size() >= 3)
    {
        VB->vertices().at(0) = A;
        VB->vertices().at(1) = B;
        VB->vertices().at(2) = C;

        VB->update();
        return;
    }
    VB->unload();
    VB->deactivate();
    IB->deactivate();
      
    VB->begin();
    VB->addTexcoord0(1.0, 0.0f);
    VB->addVertex(A); // 0 A
    VB->addTexcoord0(0.0f, 0.0f);
    VB->addVertex(B); // 1 B
    VB->addTexcoord0(1.0f, 1.0f);
    VB->addVertex(C); // 2 C
    VB->end();
      
    IB->begin();
    IB->addIndex(0);
    IB->addIndex(2);
    IB->addIndex(1);
    IB->end();
}

GUIConstantTriangle::GUIConstantTriangle(int startPixelX, int startPixelY, int width, int height, bool flip) : GUIConstantQuad(startPixelX, startPixelY, width, height)
{
    Flip = flip;

    // new VB/IB because of Triangle indices
    delete IB;
    delete VB;
    IB = new IndexBuffer();
    VB = new VertexBuffer();

    GUIConstantTriangle::updateBounds();
    GUIConstantTriangle::updateBuffers();
}

GUIConstantTriangle::~GUIConstantTriangle()
{
}

void GUIConstantTriangle::rotate180()
{
    Vector A = Vector(LowerBound.X + (UpperBound.X - LowerBound.X) * 2, LowerBound.Y + (UpperBound.Y - LowerBound.Y) / 2, 0);
    Vector B = Vector(UpperBound.X, UpperBound.Y, 0);
    Vector C = Vector(UpperBound.X, LowerBound.Y, 0);

    /*
    *     / B
    *   A   |
    *     \ C
    */

    if (VB->vertices().size() >= 3)
    {
        VB->vertices().at(0) = A;
        VB->vertices().at(1) = B;
        VB->vertices().at(2) = C;

        VB->update();
        return;
    }
    VB->unload();
    VB->deactivate();
    IB->deactivate();
      
    VB->begin();
    VB->addTexcoord0(1.0, 0.0f);
    VB->addVertex(A); // 0 A
    VB->addTexcoord0(0.0f, 0.0f);
    VB->addVertex(B); // 1 B
    VB->addTexcoord0(1.0f, 1.0f);
    VB->addVertex(C); // 2 C
    VB->end();
      
    IB->begin();
    IB->addIndex(2);
    IB->addIndex(0);
    IB->addIndex(1);
    IB->end();
}
