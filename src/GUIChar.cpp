#include "GUIChar.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "ApplicationSettings.h"
#include "Globals.h"
#include "MathUtil.h"
#include "MouseLogger.h"


GUIChar::GUIChar(float startX, float startY, CHAR_DATA data) : GUITexture(startX, startY, FNTManager::fontTexture(data.font), true, false),
data(data)
{
    updateFont(startX, startY, data);

}
GUIChar::~GUIChar()
{
    /*delete Shader;
    delete VB;
    delete IB;*/
}

void GUIChar::updateFont(float startX, float startY, CHAR_DATA d)
{
    Shader->isFont(true);
    this->data = d;
    texture(FNTManager::fontTexture(d.font));
    this->startPixelX = startX + (float)data.xOffset;
    //this->startPixelY = startY - data.yOffset;
    startPixel(Vector(startPixelX, startPixelY, 0));
    char c = 'M';
    char p = 'a';
    updateBounds();
    updateBuffers();
}

void GUIChar::updateBuffers()
{
    Vector A = LowerBound;
    Vector B = Vector(UpperBound.X, LowerBound.Y, 0);
    Vector C = Vector(LowerBound.X, UpperBound.Y, 0);
    Vector D = UpperBound;

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
    float xOffsetRemap = MathUtil::remapBounds(data.x, 0, 512, 0, 1);
    float yOffsetRemap = MathUtil::remapBounds(data.y, 0, 512, 0, 1);
    float widthRemap = MathUtil::remapBounds(data.width, 0, 512, 0, 1);
    float heightRemap = MathUtil::remapBounds(data.height, 0, 512, 0, 1);

    float xRemap = MathUtil::remapBounds(data.xOffset, 0, 512, 0, 1);
    float yRemap = MathUtil::remapBounds(data.yOffset, 0, 512, 0, 1);

    float xStart = -xOffsetRemap ;
    float xEnd = -widthRemap - xOffsetRemap ;
    float yEnd = -heightRemap - yOffsetRemap ;
    float yStart = -yOffsetRemap ;

    char a = 'b';
    char b = 'B';

    // VB schon initialisiert
    if (VB->vertices().size() >= 4)
    {     
        VB->vertices().at(0) = A;
        VB->vertices().at(1) = B;
        VB->vertices().at(2) = C;
        VB->vertices().at(3) = D;
          
        VB->texcoord0().at(0) = Vector(xStart, yEnd, 0);
        VB->texcoord0().at(1) = Vector(xEnd, yEnd, 0);
        VB->texcoord0().at(2) = Vector(xStart, yStart, 0);
        VB->texcoord0().at(3) = Vector(xEnd, yStart, 0);
        VB->update();
        return;
    }

    // VB initialisieren
    IB->deactivate();
    VB->deactivate();
      
    VB->begin();
    VB->addNormal(0, 0, 1);
      
    VB->addTexcoord0(xStart, yEnd); // 1 0
    VB->addVertex(A); // 0 A
    VB->addTexcoord0(xEnd, yEnd); // 0 0 // x richtung 1 = richtung A
    VB->addVertex(B); // 1 B
    VB->addTexcoord0(xStart, yStart); // 1 1
    VB->addVertex(C); // 2 C
    VB->addTexcoord0(xEnd, yStart); // 0 1 // x richtung 1 = richtung A
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

void GUIChar::updateBounds()
{
    float normStartX = ((float)startPixelX / ApplicationSettings::WIDTH * 2) - 1;
    float normStartY = ((float)startPixelY / ApplicationSettings::HEIGHT * 2) - 1;

    float normEndX = ((startPixelX + data.width * Scale.X) / ApplicationSettings::WIDTH * 2) - 1;
    float normEndY = ((startPixelY + data.height * Scale.Y) / ApplicationSettings::HEIGHT * 2) - 1;

    LowerBound = Vector(normStartX, normStartY, 0);
    UpperBound = Vector(normEndX, normEndY, 0);
}

