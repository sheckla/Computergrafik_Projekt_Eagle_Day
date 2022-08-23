
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
    *            C - D UpperBound
    *            |   |
    * LowerBound A - B
    */

    if (VB->vertexCount() >= 4)
    {
        VB->vertices().at(0) = A;
        VB->vertices().at(1) = B;
        VB->vertices().at(2) = C;
        VB->vertices().at(3) = D;

        VB->texcoord0().at(0) = Vector(1.0, 0.0, 0);
        VB->texcoord0().at(1) = Vector(0.0, 0.0, 0);
        VB->texcoord0().at(2) = Vector(1.0, 1.0, 0);
        VB->texcoord0().at(3) = Vector(0.0, 1.0, 0);
        VB->update();
        return;
    }
    /*delete VB;
    delete IB;
    VB = new VertexBuffer();
    IB = new IndexBuffer();*/
      
    VB->deactivate();
    IB->deactivate();
      
    VB->begin();
    VB->addNormal(0, 0, 1);
      
    VB->addTexcoord0(1.0, 0.0f);
    VB->addVertex(A); // 0 A
    VB->addTexcoord0(0.0f, 0.0f);
    VB->addVertex(B); // 1 B
    VB->addTexcoord0(1.0f, 1.0f);
    VB->addVertex(C); // 2 C
    VB->addTexcoord0(0.0f, 1.0f);
    VB->addVertex(D); // 3 D
    VB->end();
      
    IB->begin();
    IB->addIndex(2);
    IB->addIndex(0);
    IB->addIndex(1);
    IB->addIndex(1);
    IB->addIndex(3);
    IB->addIndex(2);
    IB->end();
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

void GUIConstantQuad::handleMouseEvents()
{
    // Element an Maus binden
    if (FollowMouse) {
        this->startPixelX = MouseLogger::x();
        this->startPixelY = ASPECT_HEIGHT - MouseLogger::y();
        updateBounds();
        updateBuffers();
    }
    (ForceMouseoverHighlight) ? Shader->color(MouseoverHighlightColor) : (mouseInside() && MouseoverHighlight && !MousePress) ? Shader->color(MouseoverHighlightColor) : (mouseInside() && MousePress) ? Shader->color(MousePressColor) : Shader->color(Col);
}

GUIConstantQuad::GUIConstantQuad(float startX, float startY, float width, float height) :
startPixelX(startX), startPixelY(startY), Width(width), Height(height)
{
    updateBounds();
    updateBuffers();
}

GUIConstantQuad::~GUIConstantQuad()
{
    delete Shader;
    delete IB;
    delete VB;
}

void GUIConstantQuad::draw()
{
    handleMouseEvents();

	Shader->activate();
    VB->activate();
    IB->activate();
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, IB->indexCount(), IB->indexFormat(), 0);
    glEnable(GL_DEPTH_TEST);
    IB->deactivate();
    VB->deactivate();
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
    float diffW = 0;
    float diffH = 0;
    if (Centred)
    {
        diffW = (UpperBound.X - LowerBound.X) / 2;
        diffH = (UpperBound.Y - LowerBound.Y) / 2;
    }
    if ((MouseLogger::normX() >= LowerBound.X - diffW && MouseLogger::normX() <= UpperBound.X - diffW) &&
        (MouseLogger::normY() >= LowerBound.Y - diffH && MouseLogger::normY() <= UpperBound.Y - diffH)) return true;
    return false;
}

int GUIConstantQuad::width()
{
    return Width * Scale.X;
}

int GUIConstantQuad::height()
{
    return Height * Scale.Y;
}

Vector GUIConstantQuad::startPixel()
{
    return Vector(startPixelX, startPixelY, 0);
}

bool GUIConstantQuad::mousePress()
{
    return MousePress;
}

void GUIConstantQuad::color(Color c)
{
    this->Col = c;
}

void GUIConstantQuad::mouseoverHighlightColor(Color c)
{
    MouseoverHighlightColor = c;
}

void GUIConstantQuad::mousePressColor(Color c)
{
    MousePressColor = c;
}

void GUIConstantQuad::mouseoverHighlight(bool b)
{
    MouseoverHighlight = b;
    Shader->mouseoverHighlight(b);
}

void GUIConstantQuad::forceMouseoverHighlight(bool b)
{
    ForceMouseoverHighlight = b;
}

void GUIConstantQuad::mousePress(bool b)
{
    MousePress = b;
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
    updateBounds();
    updateBuffers();
}

void GUIConstantQuad::width(float width)
{
    this->Width = width;
    updateBounds();
    updateBuffers();
}

void GUIConstantQuad::height(float height)
{
    this->Height = height;
    updateBounds();
    updateBuffers();
}

void GUIConstantQuad::constantColorMode(bool b)
{
    Shader->constantColorMode(b);
}

