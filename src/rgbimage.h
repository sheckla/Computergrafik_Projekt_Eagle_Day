#ifndef __SimpleRayTracer__rgbimage__
#define __SimpleRayTracer__rgbimage__

#include <iostream>
#include <stdio.h>
#include <ostream>
#include <string>
#include <stdio.h>
#include <cstdint>
#include <fstream>

typedef unsigned char byte;

class Color;

class RGBImage
{
private:
    unsigned int m_ImageIndexOffset(unsigned int x, unsigned int y) const;
    bool coordsInBounds(unsigned int x, unsigned int y) const;

public:
    RGBImage(unsigned int Width, unsigned Height);
    ~RGBImage();
    void setPixelColor(unsigned int x, unsigned int y, const Color& c);
    const Color& getPixelColor(unsigned int x, unsigned int y) const;
    bool saveToDisk(const char* Filename);
    unsigned int width() const;
    unsigned int height() const;

    static unsigned char convertColorChannel(float f);
    static RGBImage& SobelFilter(RGBImage& dst, const RGBImage& src, float factor = 1.0f);
    static Color grayscaleColor(const Color& c);
protected:
    Color* m_Image;
    unsigned int m_Height;
    unsigned int m_Width;
};

#endif /* defined(__SimpleRayTracer__rgbimage__) */