/*
 * Texture
 * - Laden von Textureinheiten
 * - erlaubt nun default SkyBox zerstueckelung (Skybox wird in einzelne Quadranten gesplitted)
 * - erlaubt nun default FontMaps
 *
 * Auf Basis von:
 * Created by Philipp Lensing on 24.10.14.
 * Copyright (c) 2014 Philipp Lensing. All rights reserved.
 */
#ifndef Texture_h
#define Texture_h

#include <iostream>
#include <map>
#include "Globals.h"

#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif

#include <FreeImage.h>
#include <vector>

#include "rgbimage.h"
#include "filesystem"

class Texture
{
public:
    Texture();
    Texture(unsigned int width, unsigned int height, unsigned char* data);
    Texture(unsigned int width, unsigned int height, GLint InternalFormat, GLint Format, GLint ComponentSize, GLint MinFilter, GLint MagFilter, GLint AddressMode, bool GenMipMaps);
    Texture(const char* Filename);
    Texture(const RGBImage& img);
    ~Texture();

    bool load(const char* Filename);
    bool loadCubeMap(const char* Filename);

    bool create(unsigned int width, unsigned int height, unsigned char* data);
    bool create(unsigned int width, unsigned int height, GLint InternalFormat, GLint Format, GLint ComponentSize, GLint MinFilter, GLint MagFilter, GLint AddressMode, bool GenMipMaps);
    bool create(const RGBImage& img);

    void activate(int slot = 0) const;
    void activateCubeMap(int slot = 0) const;

    void deactivate() const;
    bool isValid() const;

    unsigned int width() const;
    unsigned int height() const;

    static Texture* defaultTex();
    static Texture* defaultNormalTex();
    static Texture* defaultCubeMap();
    static Texture* defaultFontMap();

    static const Texture* LoadShared(const char* Filename);
    static void ReleaseShared(const Texture* pTex);

    const RGBImage* getRGBImage() const;
    GLuint ID() const;
    static unsigned char* LoadDataPtr(const char* filename);

protected:
    void release();
    unsigned char* LoadBMP(const char* Filename, unsigned int& width, unsigned int& height);
    static RGBImage* createImage(unsigned char* Data, unsigned int width, unsigned int height);
    struct CubeMapData
    {
        std::vector<std::string> facePaths;
        unsigned int faceWidth;
        unsigned int faceHeight;
    };
    static CubeMapData splitCubeMap(const char* path, unsigned char* Data,
                                                 unsigned int width, unsigned int height);

	unsigned char* loadDataPtr(const char* Filename);
    FIBITMAP* checkFileFormat(const char* Filename);

    mutable int CurrentTextureUnit;
    unsigned int Width;
    unsigned int Height;

    GLuint m_TextureID;
    RGBImage* m_pImage;

    static Texture* pDefaultTex;
    static Texture* pDefaultNormalTex;
    static Texture* pDefaultCubeMapTex;
    static Texture* pDefaultFontMap;

    struct TexEntry
    {
        int RefCount;
        const Texture* pTex;
    };

    typedef std::map<std::string, TexEntry> SharedTexMap;
    static SharedTexMap SharedTextures;

    unsigned int wSlice;
    unsigned int hSlice;

};

#endif 
