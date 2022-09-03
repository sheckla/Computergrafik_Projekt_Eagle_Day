#include "GUINumericPointerMeter.h"

#include <sstream>

#include "ApplicationSettings.h"
#include "GUIConstantQuadWrapper.h"
#include "GUIConstantTriangle.h"
#include "GUIText.h"
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

GUIText* GUINumericPointerMeter::number(int startX, int startY, int i, Vector scale)
{
	GUIText* numberText;
	float y = 8;
	// Speed Text (left)
	if (speedMeterMode)
	{
		numberText = new GUIText(5 + startX, startY+y, speedTexts[i], ARIAL_BLACK);
	}
	// Altitude text (right)
	else
	{
		numberText = new GUIText(ApplicationSettings::WIDTH - 40, startY+y, altitudeTexts[i], ARIAL_BLACK);
	}


	numberText->scale(scale);
	numberText->charSpace(0.5);
	return numberText;
}

void GUINumericPointerMeter::initMeterQuads(int startX, int startY, Vector& barWidths, Vector& barHeights,
	int& bigXOffset, int& mediumXOffset, int& smallXOffset)
{
	int bigHeight = barHeights.X;
	int mediumHeight = barHeights.Y;
	int smallHeight = barHeights.Z;


	int bigWidth = barWidths.X;
	int mediumWidth = barWidths.Y;
	int smallWidth = barWidths.Z;

	// Track Speed
	if (speedMeterMode) {
		MeterQuad = new GUIConstantQuad(startX + bigWidth + 30, startY, smallWidth, bigHeight);
		MeterTriangle = new GUIConstantTriangle(startX + bigWidth + 20, startY, 10, bigHeight, false);
	}
	else
		// Track Altitude
	{
		MeterQuad = new GUIConstantQuad(ApplicationSettings::WIDTH - startX - bigWidth - 90, startY, smallWidth, bigHeight);
		MeterTriangle = new GUIConstantTriangle(ApplicationSettings::WIDTH - startX - bigWidth - 20 - 20, startY, 10, bigHeight, true);
		MeterTriangle->rotate180();
		bigXOffset = ApplicationSettings::WIDTH - bigXOffset - bigWidth;
		mediumXOffset = ApplicationSettings::WIDTH - mediumXOffset - mediumWidth;
		smallXOffset = ApplicationSettings::WIDTH - smallXOffset - smallWidth;
	}

	MeterQuad->constantColorMode(true);
	MeterQuad->color(COL_DARK);

	MeterTriangle->constantColorMode(true);
	MeterTriangle->color(COL_DARK);
}

void GUINumericPointerMeter::initMeterText(int startY)
{
	GUIText* text;
	if (speedMeterMode)
	{
		text = new GUIText(10, startY + 32, "             ", ARIAL);
	} else
	{
		text = new GUIText(ApplicationSettings::WIDTH - 220, startY + 32, "                     ", ARIAL);
	}
	meterText = text;
	meterText->width(30);
	meterText->height(30);
	meterText->charSpace(0.7);
}

GUINumericPointerMeter::GUINumericPointerMeter(int startX, int startY, bool speedMeterMode, int gap, Vector barWidths, 
	Vector barHeights, Vector cornerSize, Vector numberScale) : speedMeterMode(speedMeterMode)
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
	initMeterQuads(startX, startY, barWidths, barHeights, bigXOffset, mediumXOffset, smallXOffset);

	// Pro Loop werden 2 BigBars, 1 MediumBar & 4 SmallBars sowie 2 Numbers

	for(int i = 0; i < 6; i++)
	{
		GUITexture* number1, * number2;

		// 0 - Big & Number
		Components.push_back(cornerBar(bigXOffset, startY, bigWidth, bigHeight, cornerWidth, cornerHeight));
		Numbers.push_back(number(0, startY - bigHeight/2, i, numberScale));
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
		if (i == 5) Numbers.push_back(number(0, startY - bigHeight/2, 6, numberScale));
		Components.push_back(cornerBar(bigXOffset, startY, bigWidth, bigHeight, cornerWidth, cornerHeight));

		// Init MeterMaxHeight for barPointer
		MeterMaxHeight = startY;
	}

	// Text
	initMeterText(startY);

	// Init Params fuer alle Quads
	for (auto quad : GUIConstantQuadWrapper::extractQuadsFromComponentList(Components))
	{
		quad->constantColorMode(true);
		quad->color(COL_LIGHT);
		quad->mouseoverHighlightColor(COL_DARK);
		quad->mouseoverHighlight(false);
	}
	ComponentAmount = Components.size();
}

GUINumericPointerMeter::~GUINumericPointerMeter()
{
	for (auto component : Components) delete component;
	for (auto number : Numbers) delete number;
	delete MeterQuad;
	delete MeterTriangle;
	delete meterText;
}


void GUINumericPointerMeter::draw()
{
	// ---- Numeric Quad Coloring -----
	float speedToComponentAmountRemap = MathUtil::remapBounds(plane->getSpeed(), 0, MAX_SPEED, 0, ComponentAmount); // speed
	if (!speedMeterMode) 
		speedToComponentAmountRemap = MathUtil::remapBounds(plane->getParts()[0]->transform().translation().Y, 0, 300, 0, ComponentAmount); // altitude

	// [0, Components.size] -> quads werden je nach Auslatung gefaerbt
	int hitBars = 0;
	for (auto component : Components)
	{
		for (auto quad : GUIConstantQuadWrapper::extractQuadsFromComponent(component)) 
			(hitBars < speedToComponentAmountRemap) ? quad->forceMouseoverHighlight(true) : quad->forceMouseoverHighlight(false);
		hitBars++;
		component->draw();
	}
	// ---------

	// Numbers
	for (auto number : Numbers) number->draw();

	// ---- Pointer Y Translation -----
	float speedToMeterMaxHeightRemap = MathUtil::remapBounds(plane->getSpeed(), 0, MAX_SPEED, 20, MeterMaxHeight); // Speed Mode

	// Altitude Mode
	if (!speedMeterMode)
	{
		float y = plane->getParts()[0]->transform().translation().Y;
		if (y > 300) y = 300;
		if (y < 0) y = 0;
		speedToMeterMaxHeightRemap = MathUtil::remapBounds(y, 0, 300, 20, MeterMaxHeight);
	}
	// Pointer movement - Y
	MeterQuad->startPixel(Vector(MeterQuad->startPixel().X, speedToMeterMaxHeightRemap, 0));
	MeterTriangle->startPixel(Vector(MeterTriangle->startPixel().X, speedToMeterMaxHeightRemap, 0));


	MeterQuad->draw();
	MeterTriangle->draw();
	// -------

	// Text (Speed || Altitude)
	std::stringstream ss;
	(speedMeterMode) ? ss << "Speed:" << std::fixed << std::setprecision(1) << plane->getSpeed() :
						ss << "Altitude:" << std::fixed << std::setprecision(1)  << plane->getParts()[0]->transform().translation().Y;
	std::string textString{ ss.str() };
	meterText->text(textString.c_str());
	meterText->draw();

}

void GUINumericPointerMeter::update(float delta)
{
	for (auto component : Components) component->update(delta);
	MeterTriangle->update(delta);
	MeterQuad->update(delta);
}
