#include "GUIConstantTriangle.h"

#include "ApplicationSettings.h"
#include "MouseLogger.h"

void GUIConstantTriangle::updateBuffers()
{
	Vector A = Vector(LowerBound.X, LowerBound.Y + (UpperBound.Y - LowerBound.Y)/2, 0);
	Vector B = Vector(UpperBound.X, UpperBound.Y, 0);
	Vector C = Vector(UpperBound.X, LowerBound.Y , 0);

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
    if (VB->vertices().size() >= 3)
    {
        VB->vertices().at(0) = A;
        VB->vertices().at(1) = B;
        VB->vertices().at(2) = C;

        IB->indices().at(0) = 0;
        IB->indices().at(1) = 2;
        IB->indices().at(2) = 1;
        IB->end();
        //return;
    }
    // TODO FIX small Memory LEAK
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

void GUIConstantTriangle::updateBounds()
{
    float normStartX = ((float)startPixelX / ApplicationSettings::WIDTH * 2) - 1;
    float normStartY = ((float)startPixelY / ApplicationSettings::HEIGHT * 2) - 1;

    float normEndX = ((startPixelX + Width * Scale.X) / ApplicationSettings::WIDTH * 2) - 1;
    float normEndY = ((startPixelY + Height * Scale.Y) / ApplicationSettings::HEIGHT * 2) - 1;

    LowerBound = Vector(normStartX, normStartY, 0);
    UpperBound = Vector(normEndX, normEndY, 0);
}

GUIConstantTriangle::GUIConstantTriangle(int startPixelX, int startPixelY, int width, int height, bool flip) : GUIConstantQuad(startPixelX, startPixelY, width, height)
{
    Flip = flip;
    GUIConstantTriangle::updateBounds();
    GUIConstantTriangle::updateBuffers();
}

GUIConstantTriangle::~GUIConstantTriangle()
{
}

void GUIConstantTriangle::draw()
{
    // Element an Maus binden
    if (FollowMouse) {
        this->startPixelX = MouseLogger::x();
        this->startPixelY = ApplicationSettings::HEIGHT - MouseLogger::y();
        updateBounds();
        updateBuffers();
    }

    handleMouseEvents();

    Shader->activate();
    VB->activate();
    IB->activate();
    glDisable(GL_DEPTH_TEST);
    //glRotatef(90, 1, 1, 1);
    glDrawElements(GL_TRIANGLES, IB->indexCount(), IB->indexFormat(), 0);
    glEnable(GL_DEPTH_TEST);
    IB->deactivate();
    VB->deactivate();
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
