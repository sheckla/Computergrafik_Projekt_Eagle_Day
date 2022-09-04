#include "ParticleSprite.h"
#include "Matrix.h"

//Creates a simple plane
ParticleSprite::ParticleSprite()
{
    VB.begin();
            VB.addNormal(0, 0, 1); 
            VB.addTexcoord0( 0, 0);
            VB.addTexcoord1( 0, 0);
            VB.addVertex(-.5, -.5, 0);

            VB.addNormal(0, 0, 1);
            VB.addTexcoord0(1, 0);
            VB.addTexcoord1(1, 0);
            VB.addVertex(-.5, .5, 0);

            VB.addNormal(0, 0, 1);
            VB.addTexcoord0(0, 1);
            VB.addTexcoord1(0, 1);
            VB.addVertex(.5, -.5, 0);

            VB.addNormal(0, 0, 1);
            VB.addTexcoord0(1, 1);
            VB.addTexcoord1(1, 1);
            VB.addVertex(.5, .5, 0);  
    VB.end();
   


    IB.begin();
        IB.addIndex(0);
        IB.addIndex(1);
        IB.addIndex(2);

        IB.addIndex(2);
        IB.addIndex(1);
        IB.addIndex(3);
    IB.end();
}

void ParticleSprite::draw(const BaseCamera& Cam)
{
    //Before draw, plane has to always face the camera
    Matrix lookAtCam,invView, M_Scale;
    invView = Cam.getViewMatrix();
    invView.invert();
    lookAtCam.lookAt(Vector(invView.m03, invView.m13, invView.m23), this->transform().up(), Vector(this->transform().m03, this->transform().m13, this->transform().m23)); 
    lookAtCam.invert();

    M_Scale.scale(PS_scale);

    this->transform( lookAtCam * M_Scale);
    

    //Normal OpenGL-Draw
    BaseModel::draw(Cam);
    
    VB.activate();
    IB.activate();
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    IB.deactivate();
    VB.deactivate();
    
}

