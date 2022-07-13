#include "rgbimage.h"
#include "color.h"
#include "assert.h"
#include <vector>

RGBImage::RGBImage(unsigned int Width, unsigned int Height) : m_Width(Width), m_Height(Height), m_Image(new Color[Width * Height])
{
}

RGBImage::~RGBImage()
{
	delete[] this->m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c)
{
	if (!coordsInBounds(x, y)) return;
	this->m_Image[m_ImageIndexOffset(x, y)] = c;
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const
{
	if (!coordsInBounds(x, y)) return Color(0, 0, 0); // TODO exception
	return this->m_Image[m_ImageIndexOffset(x, y)];
}

unsigned int RGBImage::width() const
{
	return this->m_Width;
}
unsigned int RGBImage::height() const
{
	return this->m_Height;
}

unsigned char RGBImage::convertColorChannel(float v)
{
	return v * 255;
}

RGBImage& RGBImage::SobelFilter(RGBImage& dst, const RGBImage& src, float factor)
{
	int width = src.width();
	int height = src.height();

	// compare width/height
	//TODO
	
	// Convert src to grayscale
	RGBImage src_grayscale(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			src_grayscale.setPixelColor(i, j, grayscaleColor(src.getPixelColor(i, j)));
		}
	}

	RGBImage sob_horizontal(width, height);
	RGBImage sob_vertical(width, height);
	int k_x[3][3] = { {1,0,-1},
					{2,0,-2},
					{1,0,-1} };
	int k_y[3][3] = { {1,2,1},
					{0,0,0},
					{-1,-2,-1} };

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// apply horizontal sobel-filter to pixel(i,j)
			float sob_horizontal_c = 0;
			float sob_vertical_c = 0;
			for (int sob_i = 0; sob_i < 3; sob_i++) {
				for (int sob_j = 0; sob_j < 3; sob_j++) {
					sob_horizontal_c += src_grayscale.getPixelColor(i-sob_i, j-sob_j).R * k_x[sob_i][sob_j];
					sob_vertical_c += src_grayscale.getPixelColor(i - sob_i, j - sob_j).R * k_y[sob_i][sob_j];
				}
			}
			sob_horizontal.setPixelColor(i,j,Color(sob_horizontal_c, sob_horizontal_c, sob_horizontal_c));
			sob_vertical.setPixelColor(i,j, Color(sob_vertical_c, sob_vertical_c, sob_vertical_c));
			float val = sqrt(sob_horizontal_c * sob_horizontal_c + sob_vertical_c * sob_vertical_c) * factor;
			Color d(val, val, val);
			dst.setPixelColor(i, j, d);
		}
	}
	return dst;
}

Color RGBImage::grayscaleColor(const Color& c)
{
	float gray = (c.R + c.G + c.B) / 3;
	Color x(gray, gray, gray);
	return x;
}

bool RGBImage::saveToDisk(const char* Filename) {
	FILE* f = fopen(Filename, "wb");

	int w = 1080;
	w = this->m_Width;
	int h = 1080;
	h = this->m_Height;

	int filesize = 54 + 3 * w * h;

	byte bmpfileheader[14] = {
		'B', 'M',				// bfType
		0, 0, 0, 0,				// bfSize
		0, 0, 0, 0,				// bfReversed	
		54, 0, 0, 0 };			// bfOffBits

	byte bmpinfoheader[40] = {
		40, 0, 0, 0,			// biSize
		0, 0, 0, 0,				// biWidth
		0, 0, 0, 0,				// biHeight
		1, 0,					// biPlanes
		24, 0 };				// biBitCount

	byte bmppad[3] = { 0, 0, 0 };

	// bfSize
	bmpfileheader[2] = (byte)(filesize);
	bmpfileheader[3] = (byte)(filesize >> 8);
	bmpfileheader[4] = (byte)(filesize >> 16);
	bmpfileheader[5] = (byte)(filesize >> 24);

	// biWidth
	bmpinfoheader[4] = (byte)(w);
	bmpinfoheader[5] = (byte)(w >> 8);
	bmpinfoheader[6] = (byte)(w >> 16);
	bmpinfoheader[7] = (byte)(w >> 24);

	// biHeight
	bmpinfoheader[8] = (byte)(h);
	bmpinfoheader[9] = (byte)(h >> 8);
	bmpinfoheader[10] = (byte)(h >> 16);
	bmpinfoheader[11] = (byte)(h >> 24);

	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	char* data = new char[w * h * 3];
	unsigned int i;
	for (int i = 0; i < (w * h * 3); i += 3) {
		data[i] = convertColorChannel(this->m_Image[i / 3].B);
		data[i + 1] = convertColorChannel(this->m_Image[i / 3].G);
		data[i + 2] = convertColorChannel(this->m_Image[i / 3].R);
	}

	for (i = 0; i < h; i++) {
		fwrite(data + (w * (h - i - 1) * 3), 3, w, f);
		fwrite(bmppad, 1, (4 - (100 * 3) % 4) % 4, f);
	}

	fclose(f);
	return true;
}


unsigned int RGBImage::m_ImageIndexOffset(unsigned int x, unsigned int y) const {
	return this->m_Width * y + x;
}

bool RGBImage::coordsInBounds(unsigned int x, unsigned int y) const {
	if (x >= this->m_Width) return false;
	if (y >= this->m_Height) return false;
	return true;
}
