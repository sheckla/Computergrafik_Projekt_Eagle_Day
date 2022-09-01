#include "ParticleSprite.h"
#include "Matrix.h"


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
    Matrix lookAtCam,invView, M_Scale;
    invView = Cam.getViewMatrix();
    invView.invert();
    lookAtCam.lookAt(Vector(invView.m03, invView.m13, invView.m23), this->transform().up(), Vector(this->transform().m03, this->transform().m13, this->transform().m23)); 
    lookAtCam.invert();

    M_Scale.scale(PS_scale);
    //std::cout <<"Scale: " << PS_scale << " Start: "<<PS_scale_begin << std::endl;

    this->transform( lookAtCam * M_Scale);
    


    BaseModel::draw(Cam);
    
    VB.activate();
    IB.activate();
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    IB.deactivate();
    VB.deactivate();
    
}

