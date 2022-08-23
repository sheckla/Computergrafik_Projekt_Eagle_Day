#include "LineBoxModel.h"

LineBoxModel::LineBoxModel(float Width, float Height, float Depth)
{
    // TODO: Add your code (Exercise 1)  
    int x = Width / 2;
    int y = Height / 2;
    int z = Depth / 2;

    Vector A(-x, -y, z);
    Vector B(x, -y, z);
    Vector F(x, y, z);
    Vector E(-x, y, z);
    Vector D(-x, -y, -z);
    Vector C(x, -y, -z);
    Vector G(x, y, -z);
    Vector H(-x, y, -z);

    VB.begin();

    VB.addVertex(A);
    VB.addVertex(B);
    VB.addVertex(B);
    VB.addVertex(C);
    VB.addVertex(C);
    VB.addVertex(D);
    VB.addVertex(D);
    VB.addVertex(A);

    VB.addVertex(E);
    VB.addVertex(F);
    VB.addVertex(F);
    VB.addVertex(G);
    VB.addVertex(G);
    VB.addVertex(H);
    VB.addVertex(H);
    VB.addVertex(E);

    VB.addVertex(A);
    VB.addVertex(E);
    VB.addVertex(B);
    VB.addVertex(F);
    VB.addVertex(C);
    VB.addVertex(G);
    VB.addVertex(D);
    VB.addVertex(H);
    /*
    VB.addVertex(-(Width / 2), -(Height / 2), -(Depth / 2));
    VB.addVertex((Width / 2), -(Height / 2), -(Depth / 2));

    VB.addVertex(-(Width / 2), -(Height / 2), -(Depth / 2));
    VB.addVertex(-(Width / 2), (Height / 2), -(Depth / 2));

    VB.addVertex(-(Width / 2), -(Height / 2), -(Depth / 2));
    VB.addVertex(-(Width / 2), -(Height / 2), (Depth / 2));

    VB.addVertex(-(Width / 2), -(Height / 2), (Depth / 2));
    VB.addVertex(-(Width / 2), (Height / 2), (Depth / 2));

    VB.addVertex(-(Width / 2), -(Height / 2), (Depth / 2));
    VB.addVertex((Width / 2), -(Height / 2), (Depth / 2));

    VB.addVertex(-(Width / 2), (Height / 2), (Depth / 2));
    VB.addVertex((Width / 2), (Height / 2), (Depth / 2));

    VB.addVertex(-(Width / 2), (Height / 2), (Depth / 2));
    VB.addVertex(-(Width / 2), (Height / 2), -(Depth / 2));

    VB.addVertex((Width / 2), (Height / 2), (Depth / 2));
    VB.addVertex((Width / 2), (Height / 2), -(Depth / 2));

    VB.addVertex((Width / 2), -(Height / 2), (Depth / 2));
    VB.addVertex((Width / 2), (Height / 2), (Depth / 2));

    VB.addVertex((Width / 2), (Height / 2), -(Depth / 2));
    VB.addVertex((Width / 2), -(Height / 2), -(Depth / 2));

    VB.addVertex((Width / 2), -(Height / 2), -(Depth / 2));
    VB.addVertex((Width / 2), -(Height / 2), (Depth / 2));

    VB.addVertex((Width / 2), (Height / 2), -(Depth / 2));
    VB.addVertex(-(Width / 2), (Height / 2), -(Depth / 2));
    */

    VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    // TODO: Add your code (Exercise 1)
    BaseModel::draw(Cam);

    VB.activate();

    glDrawArrays(GL_LINES, 0, VB.vertexCount());

    VB.deactivate();
}
