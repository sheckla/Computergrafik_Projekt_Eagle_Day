#ifndef ParticleSprite_hpp
#define ParticleSprite_hpp

#include <stdio.h>

#include <stdio.h>
#include "basemodel.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class ParticleSprite : public BaseModel
{
public:
    ParticleSprite();
    virtual ~ParticleSprite() {}
    virtual void draw(const BaseCamera& Cam);

    void ParticleSprite::setStartScale(float startScale) { this->PS_scale_begin = startScale; }
    float ParticleSprite::getStartScale() { return this->PS_scale_begin; }

    void ParticleSprite::setScale(float scale) { this->PS_scale = scale; }
   
protected:
    VertexBuffer VB;
    IndexBuffer IB;
private:
    float PS_scale_begin=1;
    float PS_scale = 1;
};

#endif /* ParticleSprite_hpp */
