
#include "GUIConstantQuad.h"

#include "GUIShader.h"
#include "MouseLogger.h"

void GUIConstantQuad::updateBuffers()
{
    Vector A = LowerBound;
    Vector B = Vector(UpperBound.X, LowerBound.Y, 0);
    Vector C = Vector(LowerBound.X, UpperBound.Y, 0);
    Vector D = UpperBound;

    // Element zentrieren
    if (Centred)
    {
        float diffW = (UpperBound.X - LowerBound.X) / 2;
        float diffH = (UpperBound.Y - LowerBound.Y) / 2;
        A = Vector(LowerBound.X - diffW, LowerBound.Y - diffH, 0);
        B = Vector(UpperBound.X - diffW, LowerBound.Y - diffH, 0);
        C = Vector(LowerBound.X - diffW, UpperBound.Y - diffH, 0);
        D = Vector(UpperBound.X - diffW, UpperBound.Y - diffH, 0);
    }

    /*
    *    C - D UpperBound
    *    |   |
    * LowerBound A - B
    */
    VB.deactivate();
    IB.deactivate();

    VB.begin();
    VB.addNormal(0, 0, 1);

    VB.addTexcoord0(1.0, 0.0f);
    VB.addVertex(A); // 0 A
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(B); // 1 B
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(C); // 2 C
    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(D); // 3 D
    VB.end();

    IB.begin();
    IB.addIndex(2);
    IB.addIndex(0);
    IB.addIndex(1);
    IB.addIndex(1);
    IB.addIndex(3);
    IB.addIndex(2);
    IB.end();
}

void GUIConstantQuad::updateBounds()
{
    float normStartX = ((float)startPixelX / ASPECT_WIDTH * 2) - 1;
    float normStartY = ((float)startPixelY / ASPECT_HEIGHT * 2) - 1;

    float normEndX = ((startPixelX + Width * Scale.X) / ASPECT_WIDTH * 2) - 1;
    float normEndY = ((startPixelY + Height * Scale.Y) / ASPECT_HEIGHT * 2) - 1;

    LowerBound = Vector(normStartX, normStartY, 0);
    UpperBound = Vector(normEndX, normEndY, 0);
}

GUIConstantQuad::GUIConstantQuad(int startX, int startY, int width, int height) : startPixelX(startX), startPixelY(startY), Width(width), Height(height)
{
    updateBounds();
    updateBuffers();
}

GUIConstantQuad::~GUIConstantQuad()
{
    delete Shader;
}

void GUIConstantQuad::draw()
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

void GUIConstantQuad::shader(GUIShader* shader)
{
    this->Shader = shader;
}

void GUIConstantQuad::scale(Vector scale)
{
    this->Scale = scale;
    updateBounds();
    updateBuffers();
}

bool GUIConstantQuad::mouseInside()
{
    if ((MouseLogger::normX() >= LowerBound.X && MouseLogger::normX() <= UpperBound.X) &&
        (MouseLogger::normY() >= LowerBound.Y && MouseLogger::normY() <= UpperBound.Y)) return true;
    return false;
}

Vector GUIConstantQuad::startPixel()
{
    return Vector(startPixelX, startPixelY, 0);
}

void GUIConstantQuad::color(Color c)
{
    this->Col = c;
}

void GUIConstantQuad::mouseHitColor(Color c)
{
    MouseHitColor = c;
}

void GUIConstantQuad::mouseoverHighlight(bool b)
{
    MouseoverHighlight = b;
}

int GUIConstantQuad::width()
{
    return Width * Scale.X;
}

int GUIConstantQuad::height()
{
    return Height * Scale.Y;
}

void GUIConstantQuad::startPixel(Vector v)
{
    startPixelX = v.X;
    startPixelY = v.Y;
    updateBounds();
    updateBuffers();
}

void GUIConstantQuad::followMouse(bool b)
{
    this->FollowMouse = b;
}

void GUIConstantQuad::centred(bool b)
{
    this->Centred = b;
}

void GUIConstantQuad::width(int width)
{
    this->Width = width;
    updateBounds();
    updateBuffers();
}

void GUIConstantQuad::height(int height)
{
    this->Height = height;
    updateBounds();
    updateBuffers();
}

void GUIConstantQuad::constantColorMode(bool b)
{
    Shader->constantColorMode(b);
}

