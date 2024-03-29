#include "ScreenQuadModel.h"
#include "GUIConstantQuad.h"
#include "Vector.h"

ScreenQuadModel::ScreenQuadModel()
{
    Shader = new PostProcessingShader();
    VB.begin();
    /*
     *   C - D
     *   |   |
     *   A - B
     */
    VB.addTexcoord0(1.0f, 1);
    VB.addVertex(Vector(-1.0f, -1.0f , 0)); // 0 A

    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(Vector(1.0f , -1.0f , 0)); // 1 B

    VB.addTexcoord0(0, 1.0f);
	VB.addVertex(Vector(-1.0f , 1.0f , 0)); // 2 C

    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(Vector(1.0f , 1.0f  , 0)); // 3 D
    VB.end();

    IB.begin();
    IB.addIndex(2);
    IB.addIndex(1);
    IB.addIndex(3);
    IB.addIndex(1);
    IB.addIndex(2);
    IB.addIndex(0);
    IB.end();

}

ScreenQuadModel::~ScreenQuadModel()
{
    delete Shader;
}

void ScreenQuadModel::draw(Camera& cam, Texture* tex)
{
    Shader->screenTexture(tex);
    Shader->activate(cam);
    VB.activate();
    IB.activate();
	glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    glEnable(GL_DEPTH_TEST);
    IB.deactivate();
    VB.deactivate();
}

PostProcessingShader* ScreenQuadModel::shader()
{
    return Shader;
}
