#include "GUINumericPointerMeter.h"

#include "GUIConstantQuadWrapper.h"
#include "GUIConstantTriangle.h"
#include "GUITexture.h"
#include "MathUtil.h"
#include "MouseLogger.h"

GUIBaseComponent* GUINumericPointerMeter::cornerBar(int startX, int startY, int width, int height, int cornerWidth, int cornerHeight)
{
	GUIConstantQuad* bar, *cornerLeft, *cornerRight;
	GUIConstantQuadWrapper* barWrapper = new GUIConstantQuadWrapper();

	bar = new GUIConstantQuad(startX, startY, width, height);
	cornerLeft = new GUIConstantQuad(startX - cornerWidth, startY - cornerHeight / 2, cornerWidth, height + cornerHeight);
	cornerRight = new GUIConstantQuad(startX + width, startY - cornerHeight / 2, cornerWidth, height + cornerHeight);

	barWrapper->add(bar);
	barWrapper->add(cornerLeft);
	barWrapper->add(cornerRight);

	return barWrapper;
}

GUIBaseComponent* GUINumericPointerMeter::bar(int startX, int startY, int width, int height)
{
	return new GUIConstantQuad(startX, startY, width, height);
}

GUITexture* GUINumericPointerMeter::number(int startX, int startY, const char* numberPath, Vector scale)
{
	GUITexture* number = new GUITexture(startX, startY, new Texture(numberPath), true);
	number->constantColorMode(true);
	number->color(Color(1, 1, 1));
	number->scale(scale);
	//number->centred(true);
	return number;
}

GUINumericPointerMeter::GUINumericPointerMeter(int startX, int startY, bool leftBound, int gap, Vector barWidths, 
	Vector barHeights, Vector cornerSize, Vector numberScale) : leftBound(leftBound)
{
	// Gap zwischen allen Bars

	// Bar widths
	int bigWidth = barWidths.X;
	int mediumWidth = barWidths.Y;
	int smallWidth = barWidths.Z;

	// Bar Heights
	int bigHeight = barHeights.X;
	int mediumHeight = barHeights.Y;
	int smallHeight= barHeights.Z;

	// Offsets zum zentrieren der Bars
	int bigXOffset = startX;
	int mediumXOffset = bigWidth / 2 - mediumWidth / 2 + startX;
	int smallXOffset = bigWidth / 2 - smallWidth / 2 + startX;

	// Corner Parameter
	int cornerHeight = cornerSize.X;
	int cornerWidth = cornerSize.Y;

	// Pointer
	if (leftBound) {
		MeterQuad = new GUIConstantQuad(startX + bigWidth + 30, startY, smallWidth, bigHeight);
		MeterTriangle = new GUIConstantTriangle(startX + bigWidth + 20, startY, 10, bigHeight);
	} else
	{
		MeterQuad = new GUIConstantQuad(ASPECT_WIDTH - startX - bigWidth - 90, startY, smallWidth, bigHeight);
		MeterTriangle = new GUIConstantTriangle(ASPECT_WIDTH - startX - bigWidth - 20 - 20, startY, 10, bigHeight);
		MeterTriangle->rotate180();
		bigXOffset = ASPECT_WIDTH - bigXOffset - bigWidth;
		mediumXOffset = ASPECT_WIDTH - mediumXOffset - mediumWidth;
		smallXOffset = ASPECT_WIDTH - smallXOffset - smallWidth;
	}

	// Pro Loop werden 2 BigBars, 1 MediumBar & 4 SmallBars sowie 2 Numbers

	for(int i = 0; i < 6; i++)
	{
		GUITexture* number1, * number2;

		// 0 - Big & Number
		Components.push_back(cornerBar(bigXOffset, startY, bigWidth, bigHeight, cornerWidth, cornerHeight));
		if (leftBound)
		{
			Numbers.push_back(number(bigXOffset / 2, startY - bigHeight / 2, numberPaths[i], numberScale));
		} else
		{
			GUITexture* tex = number(ASPECT_WIDTH - startX, startY, numberPaths[i], numberScale);
			Numbers.push_back(number(ASPECT_WIDTH - startX/2 - tex->width(), startY - bigHeight / 2, numberPaths[i], numberScale));
			delete tex;
		}
		startY += gap + bigHeight;

		// 1 - small
		Components.push_back(cornerBar(smallXOffset, startY, smallWidth, smallHeight, cornerWidth, cornerHeight));
		startY += gap + smallHeight;

		// 2 - small
		Components.push_back(cornerBar(smallXOffset, startY, smallWidth, smallHeight, cornerWidth, cornerHeight));
		startY += gap + smallHeight;

		// 3 - medium
		Components.push_back(cornerBar(mediumXOffset, startY, mediumWidth, mediumHeight, cornerWidth, cornerHeight));
		startY += gap + mediumHeight;

		// 4 - small
		Components.push_back(cornerBar(smallXOffset, startY, smallWidth, smallHeight, cornerWidth, cornerHeight));
		startY += gap + smallHeight;

		// 5 - small
		Components.push_back(cornerBar(smallXOffset, startY, smallWidth, smallHeight, cornerWidth, cornerHeight));
		startY += gap + smallHeight;

		// 6 - Big & Number
		if (leftBound)
		{
			Numbers.push_back(number(bigXOffset / 2, startY - bigHeight / 2, numberPaths[i+1], numberScale));
		}
		else
		{
			GUITexture* tex = number(ASPECT_WIDTH - startX, startY, numberPaths[i], numberScale);
			Numbers.push_back(number(ASPECT_WIDTH - startX / 2 - tex->width(), startY - bigHeight / 2, numberPaths[i+1], numberScale));
			delete tex;
		}
		Components.push_back(cornerBar(bigXOffset, startY, bigWidth, bigHeight, cornerWidth, cornerHeight));

		// Init MeterMaxHeight for barPointer
		MeterMaxHeight = startY;
	}

	// Init Params fuer alle Quads
	for (auto quad : GUIConstantQuadWrapper::extractQuadsFromComponentList(Components))
	{
		quad->constantColorMode(true);
		quad->color(Color(1, 1, 1));
		quad->mouseoverHighlightColor(Color(0.8, 0.8, 0.8));
	}
	ComponentAmount = Components.size();
}

GUINumericPointerMeter::~GUINumericPointerMeter()
{
	for (auto component : Components) delete component;
	for (auto number : Numbers) delete number;
	delete MeterQuad;
	delete MeterTriangle;
}


void GUINumericPointerMeter::draw()
{
	// Quads
	float hitBarsMax = MathUtil::remapBounds(plane->getSpeed(), 0, MAX_SPEED, 0, ComponentAmount);
	if (!leftBound) hitBarsMax = MathUtil::remapBounds(plane->getParts()[0]->transform().translation().Y, 0, 500, 0, ComponentAmount);
	int hitBars = 0;
	for (auto component : Components)
	{
		for (auto quad : GUIConstantQuadWrapper::extractQuadsFromComponent(component)) 
			(hitBars < hitBarsMax) ? quad->forceMouseoverHighlight(true) : quad->forceMouseoverHighlight(false);
		hitBars++;
		component->draw();
	}

	// Numbers
	for (auto number : Numbers) number->draw();

	// Pointer
	float pointerMaxHeight = MathUtil::remapBounds(plane->getSpeed(), 0, MAX_SPEED, 20, MeterMaxHeight);
	float y = plane->getParts()[0]->transform().translation().Y;
	if (y > 200) y = 200;
	if (!leftBound) pointerMaxHeight = MathUtil::remapBounds(y, 0, 200, 20, MeterMaxHeight);
	MeterQuad->startPixel(Vector(MeterQuad->startPixel().X, pointerMaxHeight, 0));
	MeterTriangle->startPixel(Vector(MeterTriangle->startPixel().X, pointerMaxHeight, 0));
	if (!leftBound) MeterTriangle->rotate180();
	MeterQuad->draw();
	MeterTriangle->draw();

}
