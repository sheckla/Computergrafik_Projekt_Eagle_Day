//
//  Texture.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 24.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Texture.h"
#include "rgbimage.h"
#include "color.h"
#include <assert.h>
#include <stdint.h>
#include <exception>
#include <algorithm>

#include "FreeImage.h"
#include "StringUtils.h"

Texture * Texture::pDefaultTex = NULL;
Texture* Texture::pDefaultNormalTex = NULL;
Texture* Texture::pDefaultCubeMapTex = NULL;
Texture* Texture::pDefaultFontMap = NULL;
Texture::SharedTexMap Texture::SharedTextures;

Texture* Texture::defaultTex()
{
    if (pDefaultTex)
        return pDefaultTex;

    unsigned char* data = new unsigned char[4 * 4 * 4];
    assert(data);
    std::memset(data, 255, 4 * 4 * 4);
    pDefaultTex = new Texture(4, 4, data);
    delete[] data;

    return pDefaultTex;
}

Texture* Texture::defaultNormalTex()
{
    if (pDefaultNormalTex)
        return pDefaultNormalTex;

    unsigned char data[4 * 4 * 4] = {
        127,127,255,255, 127,127,255,255, 127,127,255,255, 127,127,255,255,
        127,127,255,255, 127,127,255,255, 127,127,255,255, 127,127,255,255,
        127,127,255,255, 127,127,255,255, 127,127,255,255, 127,127,255,255,
        127,127,255,255, 127,127,255,255, 127,127,255,255, 127,127,255,255,
    };

    pDefaultNormalTex = new Texture(4, 4, data);

    return pDefaultNormalTex;
}

Texture* Texture::defaultCubeMap()
{
    if (pDefaultCubeMapTex) {
        print("loading Texture::Cubemap", "found");
        return pDefaultCubeMapTex;
    }
    print("loading Texture::Cubemap", "not found, initializing...");
    Texture* cube = new Texture();
    cube->loadCubeMap(ASSETS "models/skybox/bright/cubemap.png");
    pDefaultCubeMapTex = cube;
    return pDefaultCubeMapTex;
}

Texture* Texture::defaultFontMap()
{
    if (pDefaultFontMap)
    {
        return pDefaultFontMap;
    }
    Texture* fontMap = new Texture(ASSETS "typography/arial/arial.png");
    pDefaultFontMap = fontMap;
    return pDefaultFontMap;
}

const Texture* Texture::LoadShared(const char* Filename)
{
    std::string path = Filename;

    std::transform(path.begin(), path.end(), path.begin(), ::tolower);

    SharedTexMap::iterator it = SharedTextures.find(path);

    if (it != SharedTextures.end())
    {
        it->second.RefCount++;
        return it->second.pTex;
    }

    Texture* pTex = new Texture();
    if (!pTex->load(Filename))
    {
        delete pTex;
        std::cout << "WARNING: Texture " << Filename << " not loaded (not found).\n";
        return NULL;
    }

    TexEntry TE;
    TE.pTex = pTex;
    TE.RefCount = 1;
    SharedTextures[path] = TE;

    return pTex;
}

void Texture::ReleaseShared(const Texture* pTex)
{
    for (SharedTexMap::iterator it = SharedTextures.begin(); it != SharedTextures.end(); ++it)
    {
        if (it->second.pTex == pTex)
        {
            it->second.RefCount--;
            if (it->second.RefCount <= 0)
            {
                delete it->second.pTex;
                SharedTextures.erase(it);
            }
            return;
        }
    }
}



Texture::Texture() : m_TextureID(0), m_pImage(NULL), CurrentTextureUnit(0), Width(0), Height(0)
{

}



Texture::Texture(unsigned int width, unsigned int height, unsigned char* data) : m_TextureID(0), m_pImage(NULL), CurrentTextureUnit(0), Width(0), Height(0)
{
    bool Result = create(width, height, data);
    if (!Result)
        throw std::exception();
    Width = width;
    Height = height;
}

Texture::Texture(unsigned int width, unsigned int height, GLint InternalFormat, GLint Format, GLint ComponentSize, GLint MinFilter, GLint MagFilter, GLint AddressMode, bool GenMipMaps)
    : m_TextureID(0), m_pImage(NULL), CurrentTextureUnit(0), Width(0), Height(0)
{
    bool Result = create(width, height, InternalFormat, Format, ComponentSize, MinFilter, MagFilter, AddressMode, GenMipMaps);
    if (!Result)
        throw std::exception();
    Width = width;
    Height = height;
}

Texture::Texture(const char* Filename) : m_TextureID(0), m_pImage(NULL), CurrentTextureUnit(0), Width(0), Height(0)
{
    bool Result = load(Filename);
    if (!Result)
        throw std::exception();

    Width = getRGBImage()->width();
    Height = getRGBImage()->height();
}

Texture::Texture(const RGBImage& img) : m_TextureID(0), m_pImage(NULL), CurrentTextureUnit(0), Width(0), Height(0)
{
    bool Result = create(img);
    if (!Result)
        throw std::exception();

    Width = getRGBImage()->width();
    Height = getRGBImage()->height();
}

Texture::~Texture()
{
    release();
}

void Texture::release()
{
    if (isValid())
    {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = -1;
    }
    if (m_pImage)
        delete m_pImage;
    m_pImage = NULL;
}

bool Texture::isValid() const
{
    return m_TextureID > 0;
}

unsigned int Texture::width() const
{
    return Width;
}
unsigned int Texture::height() const
{
    return Height;
}

GLuint Texture::ID() const
{
    return m_TextureID;
}

bool Texture::load(const char* Filename)
{
    release();
    checkFileFormat(Filename);
    unsigned char* data = nullptr;
    if (!(data = loadDataPtr(Filename)))
    {
        print("Texture::load | couldn't be loaded", Filename, true);
        return false;
    }

    if (m_pImage) delete m_pImage;

    m_pImage = createImage(data, Width, Height);

    // Generate
    glGenTextures(1, &m_TextureID);

    // Bind
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    Width = m_pImage->width();
    Height = m_pImage->height();

    delete[] data;
    return true;
}

bool Texture::loadCubeMap(const char* Filename)
{
    release();
    checkFileFormat(Filename);
    unsigned char* data = new unsigned char[Width * Height * 4];
    if (!(data = loadDataPtr(Filename)))
    {
        print("Texture::Cubemap, load fail", Filename, true);
        return false;
    }

    if (m_pImage) delete m_pImage;

    CubeMapData cData = splitCubeMap(Filename, data, Width, Height);

    glGenTextures(1, &m_TextureID);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    for (int i = 0; i < 6; i++)
    {
        //glGenerateMipmap(GL_TEXTURE_2D);
        print("loading cubemap Face", cData.facePaths[i].c_str(), 0, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
            cData.faceWidth, cData.faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, LoadDataPtr(cData.facePaths.at(i).c_str())); // todo free
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    //Width = m_pImage->width();
    //Height = m_pImage->height();

    delete[] data;
    return true;
}


bool Texture::create(unsigned int width, unsigned int height, unsigned char* data)
{
    release();

    if (data)
        m_pImage = createImage(data, width, height);
    else
        m_pImage = NULL;

    glGenTextures(1, &m_TextureID);

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    Width = width;
    Height = height;

    return true;
}

bool Texture::create(unsigned int width, unsigned int height, GLint InternalFormat, GLint Format, GLint ComponentSize, GLint MinFilter, GLint MagFilter, GLint AddressMode, bool GenMipMaps)
{
    release();

    glGenTextures(1, &m_TextureID);

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, ComponentSize, NULL);
    if (GenMipMaps)
        glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, AddressMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, AddressMode);
    glBindTexture(GL_TEXTURE_2D, 0);

    Width = width;
    Height = height;

    if (glGetError() != 0)
        return false;

    return true;
}

bool Texture::create(const RGBImage& img)
{
    if (img.width() <= 0 || img.height() <= 0)
        return false;

    release();

    const unsigned int w = img.width();
    const unsigned int h = img.height();
    unsigned char* data = new unsigned char[w * h * 4];

    unsigned int k = 0;
    for (unsigned int i = 0; i < h; i++)
        for (unsigned int j = 0; j < w; j++)
        {
            Color c = img.getPixelColor(j, i);
            data[k++] = RGBImage::convertColorChannel(c.R);
            data[k++] = RGBImage::convertColorChannel(c.G);
            data[k++] = RGBImage::convertColorChannel(c.B);
            data[k++] = RGBImage::convertColorChannel(1.0f);
        }

    bool success = create(w, h, data);
    delete[] data;

    Width = w;
    Height = h;

    return success;
}


RGBImage* Texture::createImage(unsigned char* Data, unsigned int width, unsigned int height)
{
    // create CPU accessible image
    RGBImage* pImage = new RGBImage(width, height);
    assert(pImage);
    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++)
        {
            Color c((float)*(Data) / 255.0f, (float)*(Data + 1) / 255.0f, (float)*(Data + 2) / 255.0f);
            pImage->setPixelColor(j, i, c);
            Data += 4;
        }
    return pImage;
}



/*
 * Texture target 	                Orientation
 * GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
 * GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
 * GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
 * GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Back
 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Front
 */
Texture::CubeMapData Texture::splitCubeMap(const char* path, unsigned char* Data, unsigned int width, unsigned int height)
{
    typedef unsigned int uint;
    CubeMapData cubeMapData;
    // left-bound!
    // (0,0) -> (4,3)
    uint wSlice = width / 4;
    uint hSlice = height / 3;
    cubeMapData.faceWidth = wSlice;
    cubeMapData.faceHeight = hSlice;

    std::string dir = path;
    dir = dir.substr(0, dir.find_last_of('/') + 1);
    std::vector <std::string> entries = STRING_UTILS::dirEntries(dir);

    // relative filename : xxx.bmp
    std::string* faceFilename = new std::string[6];
    faceFilename[0] = "faceTop.bmp";
    faceFilename[1] = "faceLeft.bmp";
    faceFilename[2] = "faceRight.bmp";
    faceFilename[3] = "faceFront.bmp";
    faceFilename[4] = "faceBack.bmp";
    faceFilename[5] = "faceBottom.bmp";

    // absolute filename: /xxx/xxx/xxx.bmp
    std::vector<std::string> facePaths;
    for (int i = 0; i < 6; i++)
    {
        facePaths.push_back(dir + faceFilename[i]);
    }

    // front->left->top->bottom->right->back
    std::vector<std::string> cubeMapOrderedPaths;
    cubeMapOrderedPaths.push_back(facePaths[3]); // front
    cubeMapOrderedPaths.push_back(facePaths[1]); // left
    cubeMapOrderedPaths.push_back(facePaths[0]); // top
    cubeMapOrderedPaths.push_back(facePaths[5]); // bottom
    cubeMapOrderedPaths.push_back(facePaths[2]); // right
    cubeMapOrderedPaths.push_back(facePaths[4]); // back
    cubeMapData.facePaths = cubeMapOrderedPaths;

    int hits = 0;
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < entries.size(); i++) {
            if (faceFilename[face] == entries[i])
            {
                hits++;
                break;
            }
        }
    }

    if (hits == 6) {
        print("loading Cubemap, faces found", dir);
        return cubeMapData;
    }

    // Generate Faces
    print("loading Cubemap, faces not found, generating in", dir);

    std::vector<RGBImage*> faces;
    for (int i = 0; i < 6; i++) faces.push_back(new RGBImage(wSlice, hSlice));

    // first row, y = [0, 1/3*height)
    for (uint i = 0; i < hSlice; i++)
    {
        for (uint j = 0; j < width; j++)
        {
            Color c((float)*Data / 255.0f, (float)*(Data + 1) / 255.0f, (float)*(Data + 2) / 255.0f);
            faces.at(0)->setPixelColor(j - wSlice, i, c);
            Data += 4;
        }
    }

    // second row, y = [1/3*height, 2/3*height)
    for (uint i = 0; i < hSlice; i++)
    {
        for (uint j = 0; j < width; j++)
        {
            Color c((float)*Data / 255.0f, (float)*(Data + 1) / 255.0f, (float)*(Data + 2) / 255.0f);

            // left, x = [0,1/4*width]
            if (j < wSlice) faces.at(1)->setPixelColor(j, i, c);

            // front, x =[1/4*width, 2/4*width]
            if (j >= wSlice && j < wSlice * 2) faces.at(2)->setPixelColor(j - wSlice, i, c);

            // right, x = [2/4*width, 3/4*width]
            if (j >= wSlice * 2 && j < wSlice * 3) faces.at(3)->setPixelColor(j - wSlice * 2, i, c);

            // back, x = [3/4*width, width]
            if (j >= wSlice * 3) faces.at(4)->setPixelColor(j - wSlice * 3, i, c);

            Data += 4;
        }
    }

    // third row, y = [2/3*height, height)
    RGBImage bottom(wSlice, hSlice);
    for (uint i = 0; i < hSlice; i++)
    {
        for (uint j = 0; j < width; j++)
        {
            Color c((float)*Data / 255.0f, (float)*(Data + 1) / 255.0f, (float)*(Data + 2) / 255.0f);
            faces.at(5)->setPixelColor(j - wSlice, i, c);
            Data += 4;
        }
    }
    for (int i = 0; i < 6; i++) faces[i]->saveToDisk(facePaths[i].c_str());
    return cubeMapData;
}

unsigned char* Texture::loadDataPtr(const char* filename)
{
    FIBITMAP* pBitmap = checkFileFormat(filename);

    unsigned int Width = FreeImage_GetWidth(pBitmap);
    unsigned int Height = FreeImage_GetHeight(pBitmap);
    this->Width = Width;
    this->Height = Height;
    unsigned int bpp = FreeImage_GetBPP(pBitmap);
    assert(bpp == 32 || bpp == 16 || bpp == 24);

    unsigned char* data = new unsigned char[Width * Height * 4];
    unsigned char* dataPtr = data - 1;

    if (data == NULL)
    {
        FreeImage_Unload(pBitmap);
        return nullptr;
    }


    RGBQUAD c;
    for (unsigned int i = 0; i < Height; ++i)
        for (unsigned int j = 0; j < Width; ++j)
        {
            FreeImage_GetPixelColor(pBitmap, j, Height - i - 1, &c);
            *(++dataPtr) = c.rgbRed;
            *(++dataPtr) = c.rgbGreen;
            *(++dataPtr) = c.rgbBlue;
            if (bpp == 32)
                *(++dataPtr) = c.rgbReserved;
            else
                *(++dataPtr) = 255;
        }

    FreeImage_Unload(pBitmap);
    return data;
}

FIBITMAP* Texture::checkFileFormat(const char* filename)
{
    FREE_IMAGE_FORMAT ImageFormat = FreeImage_GetFileType(filename, 0);
    if (ImageFormat == FIF_UNKNOWN)
        ImageFormat = FreeImage_GetFIFFromFilename(filename);

    if (ImageFormat == FIF_UNKNOWN)
    {
        print("Texture::load | Warning - Unknown texture format", filename);
    }

    FIBITMAP* pBitmap = FreeImage_Load(ImageFormat, filename);

    if (pBitmap == NULL)
    {
        print("Texture::load | Unable to open texture image", filename, true);
    }
    FREE_IMAGE_TYPE Type = FreeImage_GetImageType(pBitmap);
    assert(Type == FIT_BITMAP);

    unsigned int Width = FreeImage_GetWidth(pBitmap);
    unsigned int Height = FreeImage_GetHeight(pBitmap);
    this->Width = Width;
    this->Height = Height;
    return pBitmap;
}

unsigned char* Texture::LoadDataPtr(const char* filename)
{
    FREE_IMAGE_FORMAT ImageFormat = FreeImage_GetFileType(filename, 0);
    if (ImageFormat == FIF_UNKNOWN)
        ImageFormat = FreeImage_GetFIFFromFilename(filename);

    if (ImageFormat == FIF_UNKNOWN)
    {
        print("Texture::load | Warning - Unknown texture format", filename);
    }

    FIBITMAP* pBitmap = FreeImage_Load(ImageFormat, filename);

    if (pBitmap == NULL)
    {
        print("Texture::load | Warning - Unable to open texture image", filename, true);
    }
    FREE_IMAGE_TYPE Type = FreeImage_GetImageType(pBitmap);
    assert(Type == FIT_BITMAP);

    unsigned int Width = FreeImage_GetWidth(pBitmap);
    unsigned int Height = FreeImage_GetHeight(pBitmap);

    unsigned int bpp = FreeImage_GetBPP(pBitmap);
    assert(bpp == 32 || bpp == 16 || bpp == 24);

    unsigned char* data = new unsigned char[Width * Height * 4];
    unsigned char* dataPtr = data - 1;

    if (data == NULL)
    {
        FreeImage_Unload(pBitmap);
        return nullptr;
    }


    RGBQUAD c;
    for (unsigned int i = 0; i < Height; ++i)
        for (unsigned int j = 0; j < Width; ++j)
        {
            FreeImage_GetPixelColor(pBitmap, j, Height - i - 1, &c);
            *(++dataPtr) = c.rgbRed;
            *(++dataPtr) = c.rgbGreen;
            *(++dataPtr) = c.rgbBlue;
            if (bpp == 32)
                *(++dataPtr) = c.rgbReserved;
            else
                *(++dataPtr) = 255;
        }

    FreeImage_Unload(pBitmap);
    return data;
}

void Texture::activate(int slot) const
{
    int maxTex;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTex);
    if (m_TextureID == 0 || slot < 0 || slot > maxTex)
        return;
    CurrentTextureUnit = slot;

    glActiveTexture(GL_TEXTURE0 + CurrentTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::activateCubeMap(int slot) const
{
    int maxTex;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTex);
    if (m_TextureID == 0 || slot < 0 || slot > maxTex)
        return;

    CurrentTextureUnit = slot;

    glActiveTexture(GL_TEXTURE0 + CurrentTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}

void Texture::deactivate() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    if (CurrentTextureUnit > 0)
        glActiveTexture(GL_TEXTURE0 + CurrentTextureUnit - 1);
    CurrentTextureUnit = 0;

}

const RGBImage* Texture::getRGBImage() const
{
    return m_pImage;
}
