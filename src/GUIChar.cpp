#include "GUIChar.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Globals.h"
#include "MathUtil.h"
#include "MouseLogger.h"


GUIChar::GUIChar(int startX, int startY, int width, int height, FontData data) : GUITexture(startX, startY, Texture::defaultFontMap(), false, false),
data(data)
{
    GUIConstantQuad::width(width);
    GUIConstantQuad::height(height);
    Shader->isFont(true);
    updateBuffers();

}
GUIChar::~GUIChar()
{
}

void GUIChar::updateBuffers()
{
    Vector A = LowerBound;
    Vector B = Vector(UpperBound.X, LowerBound.Y, 0);
    Vector C = Vector(LowerBound.X, UpperBound.Y, 0);
    Vector D = UpperBound;
    GUIConstantQuad::updateBounds();

    /*
    *            C - D UpperBound
    *            |   |
    * LowerBound A - B
    */
    char c = 'S';
    float xOffset = MathUtil::remapBounds(data.x, 0, 512, 0, 1);
    float yOffset = MathUtil::remapBounds(data.y, 0, 512, 0, 1);
    float width = data.width;
    float height = data.height;
    float widthRemap = MathUtil::remapBounds(width, 0, 512, 0, 1);
    float heightRemap = MathUtil::remapBounds(height, 0, 512, 0, 1);

    if (VB->vertices().size() > 3)
    {     
        VB->vertices().at(0) = A;
        VB->vertices().at(1) = B;
        VB->vertices().at(2) = C;
        VB->vertices().at(3) = D;
          
        VB->texcoord0().at(0) = Vector(0 - xOffset, 0 - heightRemap - yOffset, 0);
        VB->texcoord0().at(1) = Vector(0 - widthRemap - xOffset, 0 - heightRemap - yOffset, 0);
        VB->texcoord0().at(2) = Vector(0 - xOffset, 0 - yOffset, 0);
        VB->texcoord0().at(3) = Vector(0 - widthRemap - xOffset, 0 - yOffset, 0);
        VB->update();
        return;
    }


    IB->deactivate();
    VB->deactivate();
      
    VB->begin();
    VB->addNormal(0, 0, 1);
      
    VB->addTexcoord0(0 - xOffset , 0- heightRemap - yOffset); // 1 0
    VB->addVertex(A); // 0 A
    VB->addTexcoord0(0- widthRemap - xOffset, 0 - heightRemap - yOffset); // 0 0 // x richtung 1 = richtung A
    VB->addVertex(B); // 1 B
    VB->addTexcoord0(0 - xOffset , 0 - yOffset); // 1 1
    VB->addVertex(C); // 2 C
    VB->addTexcoord0(0 - widthRemap - xOffset, 0 - yOffset); // 0 1 // x richtung 1 = richtung A
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

