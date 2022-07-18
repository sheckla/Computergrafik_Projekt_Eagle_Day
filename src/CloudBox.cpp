#include "CloudBox.h"

CloudBox::CloudBox(float Width, float Height, float Depth, bool bottomOff, bool topOff, bool frontOff, bool backOff, bool rightOff, bool leftOff)
{
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
    if (!bottomOff) {
        VB.addNormal(0, 0, -1);
        VB.addTexcoord0(0, 1);
        VB.addTexcoord0(1, 1);
        VB.addTexcoord0(1, 0);
        VB.addTexcoord0(0, 0);
        VB.addVertex(D);
        VB.addVertex(C);
        VB.addVertex(B);
        VB.addVertex(A);
    }

    // top
    if (!topOff) {
        VB.addNormal(0, 0, 1);
        VB.addTexcoord0(0, 1);
        VB.addTexcoord0(1, 1);
        VB.addTexcoord0(1, 0);
        VB.addTexcoord0(0, 0);
        VB.addVertex(E);
        VB.addVertex(F);
        VB.addVertex(G);
        VB.addVertex(H);
    }

    // front
    if (!frontOff) {
        VB.addNormal(0, 0, -1);
        VB.addTexcoord0(0, 1);
        VB.addTexcoord0(1, 1);
        VB.addTexcoord0(1, 0);
        VB.addTexcoord0(0, 0);
        VB.addVertex(A);
        VB.addVertex(B);
        VB.addVertex(F);
        VB.addVertex(E);
    }

    // back
    if (!backOff) {
        VB.addNormal(0, 0, -1);
        VB.addTexcoord0(0, 1);
        VB.addTexcoord0(1, 1);
        VB.addTexcoord0(1, 0);
        VB.addTexcoord0(0, 0);
        VB.addVertex(C);
        VB.addVertex(D);
        VB.addVertex(H);
        VB.addVertex(G);
    }

    // right
    if (!rightOff) {
        VB.addNormal(1, 0, 0);
        VB.addTexcoord0(0, 1);
        VB.addTexcoord0(1, 1);
        VB.addTexcoord0(1, 0);
        VB.addTexcoord0(0, 0);
        VB.addVertex(B);
        VB.addVertex(C);
        VB.addVertex(G);
        VB.addVertex(F);
    }

    // left
    if (!leftOff) {
        VB.addNormal(-1, 0, 0);
        VB.addTexcoord0(0, 1);
        VB.addTexcoord0(1, 1);
        VB.addTexcoord0(1, 0);
        VB.addTexcoord0(0, 0);
        VB.addVertex(D);
        VB.addVertex(A);
        VB.addVertex(E);
        VB.addVertex(H);
    }

    VB.end();

    IB.begin();
    // Indexbuffer
    int sum = bottomOff + topOff + frontOff + backOff + rightOff + leftOff;
    for (size_t i = 0; i <= 20 - (4 * sum); i += 4)
    {
        IB.addIndex(i + 0);
        IB.addIndex(i + 1);
        IB.addIndex(i + 2);

        IB.addIndex(i + 0);
        IB.addIndex(i + 2);
        IB.addIndex(i + 3);

        /*IB.addIndex(i + 2);
        IB.addIndex(i + 1);
        IB.addIndex(i + 0);

        IB.addIndex(i + 3);
        IB.addIndex(i + 2);
        IB.addIndex(i + 0);*/
    }
    IB.end();
    this->bounds = AABB(Vector(-x, -y, -z), Vector(x, y, z));
}

void CloudBox::draw(const BaseCamera& Cam)
{   
    applyShaderParameter();
    BaseModel::draw(Cam);

    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

    IB.deactivate();
    VB.deactivate();
}

void CloudBox::applyShaderParameter()
{
    CloudShader* Shader = dynamic_cast<CloudShader*>(BaseModel::shader());
    if (!Shader) std::cout << "fail";
        return;

    for (int i = 0; i < WORLEY_AMOUNT; i++) {
        Shader->detailTex(i, &DetailTex[i]);
    }
    Shader->noise(&this->noise);
}

void CloudBox::load(const char* worleyPath, const char* noisePath)
{
    // Worley DetailTex
    for (int i = 0; i < WORLEY_AMOUNT; i++) {
        std::stringstream ss;
        ss << "../../assets/worley/" << "worley" << i << ".bmp";
        std::string s = ss.str();
        if (!DetailTex[i].load(s.c_str())) {
            std::cout << "load error!" << std::endl;
        }
        std::cout << s << std::endl;
        ss.str("");
    }

    // Noise
    std::string nPath = worleyPath;
    nPath += "worley0.bmp";
    noise.load(nPath.c_str());
}

AABB& CloudBox::boundingBox()
{
    return this->bounds;
}
