
#include "guiElement.h"

#include "GUIShader.h"
#include "MouseLogger.h"


GUIElement::~GUIElement()
{
    delete Shader;
}

void GUIElement::draw(BaseCamera& cam)
{
	this->Shader->activate(cam);
}

void GUIElement::shader(BaseShader* shader)
{
    this->Shader = shader;
}

void GUITexture::updateBuffers()
{



    Vector A = lb;
    Vector B = Vector(ub.X, lb.Y, 0);
    Vector C = Vector(lb.X, ub.Y, 0);
    Vector D = ub;

    if (StartIsCenter)
    {
        float diffW = (ub.X - lb.X)/2;
        float diffH = (ub.Y - lb.Y)/2;
        A = Vector(lb.X - diffW, lb.Y - diffH, 0);
        B = Vector(ub.X - diffW, lb.Y - diffH, 0);
        C = Vector(lb.X - diffW, ub.Y - diffH, 0);
        D = Vector(ub.X - diffW, ub.Y - diffH, 0);
    }

    /*
    *    C - D ub
    *    |   |
    * lb A - B
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

void GUITexture::updateBounds()
{
    if (!KeepImageSize)
    {
        Width = 100;
        Height = 100;
    }

    float normStartX = ((float)startX / ASPECT_WIDTH * 2) - 1;
    float normStartY = ((float)startY / ASPECT_HEIGHT * 2) - 1;

    float normEndX = ((startX + Width) * scaling.X / ASPECT_WIDTH * 2) - 1;
    float normEndY = ((startY + Height) * scaling.Y / ASPECT_HEIGHT * 2) - 1;

    lb = Vector(normStartX, normStartY, 0);
    ub = Vector(normEndX, normEndY, 0);
}

GUITexture::GUITexture(int startX, int startY, int width, int height) : startX(startX), startY(startY), Width(width), Height(height)
{
    updateBounds();
    updateBuffers();
}


GUITexture::GUITexture(int startX, int startY, Texture* tex, bool keepImageSize )
    : GUITexture(startX, startY, tex->width(), tex->height())
{
    this->KeepImageSize = keepImageSize;
    texture(tex);
}

GUITexture::~GUITexture()
{
    pTex->deactivate();
}

void GUITexture::texture(Texture* tex)
{
    this->pTex = tex;
    this->Width = pTex->width();
    this->Height = pTex->height();
    GUIShader* pGUI = dynamic_cast<GUIShader*>(this->Shader);
    pGUI->texture(this->pTex);
}

void GUITexture::draw(BaseCamera& cam)
{
    if (FollowMouse) {
        this->startX = MouseLogger::getX();
        this->startY = ASPECT_HEIGHT - MouseLogger::getY();
        updateBounds();
        updateBuffers();
    }

    float colX = MouseLogger::getX() / ASPECT_WIDTH;
    float colY = MouseLogger::getY() / ASPECT_HEIGHT;

    color(Color(colX, colY, 0));

    GUIElement::draw(cam);
    VB.activate();
    IB.activate();
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    glEnable(GL_DEPTH_TEST);
    IB.deactivate();
    VB.deactivate();
}

void GUITexture::scale(Vector scale)
{
    this->scaling = scale;
    updateBounds();
    updateBuffers();
}

void GUITexture::width(int width)
{
    this->Width = width;
}

void GUITexture::height(int height)
{
    this->Height = height;
}

void GUITexture::startIsCenter(bool b)
{
    this->StartIsCenter = b;
}

void GUITexture::followMouse(bool b)
{
    this->FollowMouse = b;
}

void GUITexture::color(Color c)
{
    this->Col = c;
    GUIShader* pGUI = dynamic_cast<GUIShader*>(this->Shader);
    pGUI->color(Col);
}
