#include "GUIMeter.h"

#include "GUIConstantTriangle.h"
#include "GUITexture.h"
#include "MouseLogger.h"

GUIMeter::GUIMeter(int startX, int startY)
{
	int gap = 5;

	int bigWidth = 110;
	int mediumWidth = 80;
	int smallWidth = 60;

	int bigHeight = 13;
	int mediumHeight = 9;
	int smallHeight= 4;

	int offsetHeight = startY;
	int offsetWidth = startX;

	int smallXOffset = bigWidth / 2 - smallWidth / 2 + startX;
	int mediumXOffset = bigWidth / 2 - mediumWidth / 2 + startX;
	int bigXOffset = startX;

	int cornerHeight = 2;
	int cornerWidth = 3;

	Vector numberScale(.08, .08, 0);

	char** ids = new char* [10];
	ids[0] = ASSETS "typography/0.png";
	ids[1] = ASSETS "typography/1.png";
	ids[2] = ASSETS "typography/2.png";
	ids[3] = ASSETS "typography/3.png";
	ids[4] = ASSETS "typography/4.png";
	ids[5] = ASSETS "typography/5.png";
	ids[6] = ASSETS "typography/6.png";
	ids[7] = ASSETS "typography/7.png";
	ids[8] = ASSETS "typography/8.png";
	ids[9] = ASSETS "typography/9.png";

	meter = new GUIConstantQuad(offsetWidth + bigWidth + 50, offsetHeight, 50, 10);
	pointer = new GUIConstantTriangle(offsetWidth + bigWidth + 40, offsetHeight, 10, 10);

	for(int i = 0; i < 6; i++)
	{
		GUIConstantQuad* big1, *big2;
		GUIConstantQuad* small1, *small2, *small3, *small4;
		GUIConstantQuad* medium;
		GUIConstantQuad* corner1, *corner2, *corner3, *corner4;
		GUITexture* number1, * number2;


		big1 = new GUIConstantQuad(bigXOffset, offsetHeight, bigWidth, bigHeight);
		corner1 = new GUIConstantQuad(bigXOffset - cornerWidth, offsetHeight - cornerHeight/2, cornerWidth, bigHeight + cornerHeight);
		corner2 = new GUIConstantQuad(bigXOffset + bigWidth, offsetHeight - cornerHeight/2, cornerWidth, bigHeight + cornerHeight);

		number1 = new GUITexture(bigXOffset / 2 - 15, offsetHeight - bigHeight, new Texture(ids[i]), true);
		number1->constantColorMode(true);
		number1->color(Color(1, 1, 1));
		number1->scale(numberScale);

		offsetHeight += gap + bigHeight;

		small1 = new GUIConstantQuad(smallXOffset, offsetHeight, smallWidth, smallHeight);
		offsetHeight += gap + smallHeight;

		small2 = new GUIConstantQuad(smallXOffset, offsetHeight, smallWidth, smallHeight);
		offsetHeight += gap + smallHeight;

		medium = new GUIConstantQuad(mediumXOffset, offsetHeight, mediumWidth, mediumHeight);
		offsetHeight += gap + mediumHeight;

		small3 = new GUIConstantQuad(smallXOffset, offsetHeight, smallWidth, smallHeight);
		offsetHeight += gap + smallHeight;

		small4 = new GUIConstantQuad(smallXOffset, offsetHeight, smallWidth, smallHeight);
		offsetHeight += gap + smallHeight;
		maxHeight = offsetHeight;

		big2 = new GUIConstantQuad(bigXOffset, offsetHeight, bigWidth, bigHeight);
		corner3 = new GUIConstantQuad(bigXOffset - cornerWidth, offsetHeight - cornerHeight / 2, cornerWidth, bigHeight + cornerHeight);
		corner4 = new GUIConstantQuad(bigXOffset + bigWidth, offsetHeight - cornerHeight / 2, cornerWidth, bigHeight + cornerHeight);

		number2 = new GUITexture(bigXOffset / 2 -15, offsetHeight - bigHeight, new Texture(ids[i+1]), true);
		number2->constantColorMode(true);
		number2->color(Color(1, 1, 1));
		number2->scale(numberScale);

		quads.push_back(big1);
		numbers.push_back(number1);
		//quads.push_back(corner1);
		//quads.push_back(corner2);

		quads.push_back(small1);
		quads.push_back(small2);

		quads.push_back(medium);

		quads.push_back(small3);
		quads.push_back(small4);

		quads.push_back(big2);
		//quads.push_back(corner3);
		//quads.push_back(corner4);
		numbers.push_back(number2);
	}

	for (auto quad : quads)
	{
		quad->constantColorMode(true);
		quad->color(Color(1, 1, 1));
		quad->mouseHitColor(Color(0.8, 0.8, 0.8));
		quad->mouseoverHighlight(true);
		totalQuads++;
	}



}

GUIMeter::~GUIMeter()
{
}

float remap(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void GUIMeter::draw()
{
	float remappedQuads = remap(plane->getSpeed(), 0, MAX_SPEED, 0, totalQuads);
	print("total", totalQuads);
	print("remap", remappedQuads);
	// Meter-Quads
	int i = 0;
	for (auto quad : quads)
	{
		(i++ < remappedQuads) ? quad->color(Color(0.5, 0.5, 0.5)) : quad->color(Color(1, 1, 1));
		quad->draw();
	}

	// Numbers 0-9
	for (auto number : numbers)
	{
		//number->scale(Vector(MouseLogger::normX(), abs(MouseLogger::normY()), 0));
		number->draw();
	}

	// Meter
	float remappedSpeed = remap(plane->getSpeed(), 0, MAX_SPEED, 20, maxHeight);
	meter->startPixel(Vector(meter->startPixel().X, remappedSpeed, 0));
	pointer->startPixel(Vector(pointer->startPixel().X, remappedSpeed, 0));
	meter->draw();
	pointer->draw();
}
