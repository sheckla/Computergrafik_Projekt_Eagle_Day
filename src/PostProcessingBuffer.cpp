#include "PostProcessingBuffer.h"

PostProcessingBuffer::PostProcessingBuffer(int width = ASPECT_WIDTH, int height = ASPECT_HEIGHT)
{
    this->screenTex.create(width, height,
        GL_RGB, GL_RGB, GL_FLOAT, GL_LINEAR, GL_LINEAR,
        GL_CLAMP_TO_EDGE, false);
    this->buffer.create(true, width, height);
    this->buffer.attachColorTarget(screenTex);
    this->screenQuad = new ScreenQuadModel();
}

void PostProcessingBuffer::draw(Camera cam)
{
    screenQuad->draw(cam, &screenTex);
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
    screenQuad->shader()->gaussianBlur(b);
}

void PostProcessingBuffer::shake(bool b)
{
    screenQuad->shader()->shake(b);
    if (!b) shakeTime = 0;
}

void PostProcessingBuffer::hp(float hp)
{
    screenQuad->shader()->hp(hp);
}

void PostProcessingBuffer::update(float delta)
{
    if (!PostProcessingActive) delta = -delta;

    elapsedTime += delta;
    shakeTime += delta;
    if (elapsedTime < 0) elapsedTime = 0;
    if (elapsedTime > TIME_MAX_POST_PROCESSING_EFFECTS) elapsedTime = TIME_MAX_POST_PROCESSING_EFFECTS;
    screenQuad->shader()->elapsedTime(elapsedTime);
    screenQuad->shader()->shakeTime(shakeTime);
}

void PostProcessingBuffer::postProcessingActive(bool b)
{
    PostProcessingActive = b;
}