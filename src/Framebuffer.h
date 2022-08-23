/*
 * Framebuffereinheit
 * Nutzen: - ShadowMapper
 *         - PostProcessingBuffer
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef FrameBuffer_h
#define FrameBuffer_h
#include "Texture.h"

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();
    
    bool create(bool CreateDepthTarget=true, unsigned int Width=0, unsigned int Height=0);

    bool attachColorTarget( const Texture& Tex);
    bool detachColorTarget();
    const Texture* getAttachedTexture() { return m_pTexture; }
    
    void activate();
    void deactivate();
    
    unsigned int width() const { return m_Width; }
    unsigned int height() const { return m_Height; }
protected:
    GLuint m_FramebufferID;
    GLuint m_DepthTargetID;
    const Texture* m_pTexture;
    unsigned int m_Width;
    unsigned int m_Height;
};

#endif /* defined(__RealtimeRending__Framebuffer__) */
