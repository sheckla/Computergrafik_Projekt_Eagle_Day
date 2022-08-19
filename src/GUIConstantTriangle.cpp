#include "GUIConstantTriangle.h"

#include "MouseLogger.h"

void GUIConstantTriangle::updateBuffers()
{
    Vector A = Vector(LowerBound.X, LowerBound.Y + (UpperBound.Y - LowerBound.Y)/2, 0);
    Vector B = Vector(UpperBound.X, UpperBound.Y, 0);
    Vector C = Vector(UpperBound.X, LowerBound.Y , 0);

    /*
    *     / B
    *   A   |
    *     \ C
    */
    VB.deactivate();
    IB.deactivate();

    VB.begin();

    VB.addTexcoord0(1.0, 0.0f);
    VB.addVertex(A); // 0 A
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(B); // 1 B
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(C); // 2 C
    VB.end();

    IB.begin();
    IB.addIndex(0);
    IB.addIndex(2);
    IB.addIndex(1);
    IB.end();
}

void GUIConstantTriangle::updateBounds()
{
    float normStartX = ((float)startPixelX / ASPECT_WIDTH * 2) - 1;
    float normStartY = ((float)startPixelY / ASPECT_HEIGHT * 2) - 1;

    float normEndX = ((startPixelX + Width * Scale.X) / ASPECT_WIDTH * 2) - 1;
    float normEndY = ((startPixelY + Height * Scale.Y) / ASPECT_HEIGHT * 2) - 1;

    LowerBound = Vector(normStartX, normStartY, 0);
    UpperBound = Vector(normEndX, normEndY, 0);
}

GUIConstantTriangle::GUIConstantTriangle(int startPixelX, int startPixelY, int width, int height) : GUIConstantQuad(startPixelX, startPixelY, width, height)
{
    updateBounds();
    updateBuffers();
}

GUIConstantTriangle::~GUIConstantTriangle()
{
}

void GUIConstantTriangle::draw()
{
    // Element an Maus binden
    if (FollowMouse) {
        this->startPixelX = MouseLogger::x();
        this->startPixelY = ASPECT_HEIGHT - MouseLogger::y();
        updateBounds();
        updateBuffers();
    }

    (MouseoverHighlight && mouseInside()) ? Shader->color(MouseHitColor) : Shader->color(Col);

    Shader->activate();
    VB.activate();
    IB.activate();
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    glEnable(GL_DEPTH_TEST);
    IB.deactivate();
    VB.deactivate();
}
