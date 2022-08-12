//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"
#include "CloudShader.h"

/*
Bottom    Top
D - C     H - G
|   |     |   |
A - B     E - F
*/
TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{

    /// TODO: Add your code (Exercise 2)
    //
    float x = Width / 2;
    float y = Height / 2;
    float z = Depth / 2;

    Vector A(-x, -y, z);
    Vector B(x, -y, z);
    Vector F(x, y, z);
    Vector E(-x, y, z);
    Vector D(-x, -y, -z);
    Vector C(x, -y, -z);
    Vector G(x, y, -z);
    Vector H(-x, y, -z);

    VB.begin();
    // bottom
    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(D); // 0

    VB.addTexcoord0(1, 0);
    VB.addVertex(C); // 1

    VB.addTexcoord0(0, 1);
    VB.addVertex(A); // 2

    VB.addTexcoord0(1, 1);
    VB.addVertex(B); // 3

    // top
    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(H); // 4

    VB.addTexcoord0(1, 0);
    VB.addVertex(G); // 5

    VB.addTexcoord0(0, 1);
    VB.addVertex(E); // 6

    VB.addTexcoord0(1, 1);
    VB.addVertex(F); // 7

    // front
    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(1, 1);
    VB.addVertex(A); // 8

    VB.addTexcoord0(0, 1);
    VB.addVertex(B); // 9

    VB.addTexcoord0(1, 0);
    VB.addVertex(E); // 10

    VB.addTexcoord0(0, 0);
    VB.addVertex(F); // 11

    // back
    VB.addNormal(0, 0, -1);
    VB.addTexcoord0(1, 1);
    VB.addVertex(D); // 12

    VB.addTexcoord0(0, 1);
    VB.addVertex(C); // 13

    VB.addTexcoord0(1, 0);
    VB.addVertex(H); // 14

    VB.addTexcoord0(0, 0);
    VB.addVertex(G); // 15

    // right
    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(F); // 16

    VB.addTexcoord0(1, 0);
    VB.addVertex(G); // 17

    VB.addTexcoord0(0, 1);
    VB.addVertex(B); // 18

    VB.addTexcoord0(1, 1);
    VB.addVertex(C); // 19

    // left
    VB.addNormal(-1, 0, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(E); // 20

    VB.addTexcoord0(1, 0);
    VB.addVertex(H); // 21

    VB.addTexcoord0(0, 1);
    VB.addVertex(A); // 22

    VB.addTexcoord0(1, 1);
    VB.addVertex(D); // 23

    VB.end();

    IB.begin();
    // bottom
    IB.addIndex(0);
    IB.addIndex(1);
    IB.addIndex(2);
    IB.addIndex(2);
    IB.addIndex(1);
    IB.addIndex(3);

    // top 
    IB.addIndex(7);
    IB.addIndex(5);
    IB.addIndex(4);
    IB.addIndex(4);
    IB.addIndex(6);
    IB.addIndex(7);

    //front
    IB.addIndex(10);
    IB.addIndex(8);
    IB.addIndex(9);
    IB.addIndex(9);
    IB.addIndex(11);
    IB.addIndex(10);

    //back
    IB.addIndex(13);
    IB.addIndex(14);
    IB.addIndex(15);
    IB.addIndex(13);
    IB.addIndex(12);
    IB.addIndex(14);

    // right
    IB.addIndex(16);
    IB.addIndex(18);
    IB.addIndex(19);
    IB.addIndex(19);
    IB.addIndex(17);
    IB.addIndex(16);

    // left
    IB.addIndex(23);
    IB.addIndex(22);
    IB.addIndex(21);
    IB.addIndex(22);
    IB.addIndex(20);
    IB.addIndex(21);

    IB.end();

}

void TriangleBoxModel::draw(const BaseCamera& Cam)
{
    //SHADERPARAMS
    //CloudShader* Shader = dynamic_cast<CloudShader*>(BaseModel::shader());
    //noise.load("G:/VisualStudioProjects/cgprakt5/assets/worley0.bmp");
    //Shader->SetWorleyTexture(&noise);


    BaseModel::draw(Cam);
    
    

    // TODO: Add your code (Exercise 2)
    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

    IB.deactivate();
    VB.deactivate();
}
