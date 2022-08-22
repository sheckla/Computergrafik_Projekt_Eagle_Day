#include "PostProcessingBuffer.h"

PostProcessingBuffer::PostProcessingBuffer()
{
}

PostProcessingBuffer::PostProcessingBuffer(int width = ASPECT_WIDTH, int height = ASPECT_HEIGHT)
{
    this->screenTex.create(width, height,
        GL_RGB, GL_RGB, GL_FLOAT, GL_LINEAR, GL_LINEAR,
        GL_CLAMP_TO_EDGE, false);
    this->buffer.create(true, width, height);
    this->buffer.attachColorTarget(screenTex);
    this->screenQuad = ScreenQuadModel();
}

void PostProcessingBuffer::draw(Camera cam)
{
    screenQuad.draw(cam, &screenTex);
}

void PostProcessingBuffer::postDraw()
{
    buffer.deactivate();
    buffer.detachColorTarget();

}

void PostProcessingBuffer::preDraw()
{
    buffer.attachColorTarget(screenTex);
    buffer.activate();
}

void PostProcessingBuffer::gaussianBlur(bool b)
{
    screenQuad.shader()->gaussianBlur(b);
}

void PostProcessingBuffer::elapseTime(float delta)
{
    screenQuad.shader()->elapsedTime(delta);
}
