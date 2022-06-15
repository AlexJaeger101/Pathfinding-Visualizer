#include "Color.h"

Color::Color(int r, int g, int b, int a /*= 255*/)
	:mR(r)
	,mG(g)
	,mB(b)
	,mA(a)
{
}

Color::Color(float r, float g, float b, float a /*= 1.0f*/)
	:mR((int)(r*255))
	,mG((int)(g*255))
	,mB((int)(b*255))
	,mA((int)(a*255))
{
}

bool Color::operator<(const Color& rhs) const
{
	if (mR < rhs.mR)
		return true;
	else if (mR == rhs.mR)
	{
		if (mG < rhs.mG)
			return true;
		else if (mG == rhs.mG)
		{
			if (mB < rhs.mB)
				return true;
			else if (mB == rhs.mB)
			{
				if (mA < rhs.mA)
					return true;
			}
		}
	}

	return false;
}

std::ostream& operator<<(std::ostream& out, const Color& color)
{
	out << color.getR() << " " << color.getG() << " " << color.getB() << " " << color.getA() << std::endl;
	return out;
}
