/*
 * IndexBuffer
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef IndexBuffer_h
#define IndexBuffer_h

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <vector>

class IndexBuffer
{
private:

    std::vector<unsigned int> Indices;

    GLuint IBO;
    bool BufferInitialized;
    bool WithinBeginAndEnd;
    
    GLenum IndexFormat;
    unsigned int IndexCount;
    
public:
    IndexBuffer();
    ~IndexBuffer();
    
    void begin();
    void addIndex( unsigned int Index);
    void end();
    
    void activate();
    void deactivate();
    
    GLenum indexFormat() const { return IndexFormat; }
    unsigned int indexCount() const { return IndexCount; }
    std::vector<unsigned int>& indices() { return Indices; }
    
};

#endif /* IndexBuffer_hpp */
