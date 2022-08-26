#ifndef FNTManager_h
#define FNTManager_h
#include <vector>

struct CharData
{
	CharData(int id, int x, int y, int width, int height, int xOffset, int yOffset, int xAdvance) :
		id(id), x(x), y(y), width(width), height(height), xOffset(xOffset), yOffset(yOffset), xAdvance(xAdvance) {}
	CharData(): id(0), x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0) {}
	int id;
	int x;
	int y;
	int width;
	int height;
	int xOffset;
	int yOffset;
	int xAdvance;
};

struct FNT_DATA
{
	std::vector<CharData> fontData;
	const char* fontName;
};

class FNTManager
{
	static FNTManager* pFNTManager;
	std::vector<FNT_DATA> fntData;
public:
	FNTManager& instance();

	void loadFont(const char* path);
};

#endif