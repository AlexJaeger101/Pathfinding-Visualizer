#pragma once

#include <Trackable.h>

class GraphicsSystem;

class Color :public Trackable
{
	//friend GraphicsSystem;
public:
	Color(int r=255, int g=255, int b=255, int a = 255);
	Color(float r, float g, float b, float a = 1.0f);

	int getR() const { return mR; };
	int getG() const { return mG; };
	int getB() const { return mB; };
	int getA() const { return mA; };
	bool operator<(const Color& rhs) const;

private:
	int mR = 255;
	int mG = 255;
	int mB = 255;
	int mA = 255;
};

const Color BLACK_COLOR = Color(0, 0, 0);
const Color WHITE_COLOR = Color(255, 255, 255);
const Color RED_COLOR = Color(255, 0, 0);
const Color GREEN_COLOR = Color(0, 255, 0);
const Color BLUE_COLOR = Color(0, 0, 255);

std::ostream& operator<< (std::ostream& out, const Color& color);

