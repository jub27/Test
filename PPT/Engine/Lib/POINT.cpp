#include "POINT.h"

namespace JEngine
{
	POINT::POINT()
	{
		x = 0;
		y = 0;
	}

	POINT::POINT(int x_, int y_) 
	{
		x = x_;
		y = y_;
	}

	POINT::~POINT()
	{

	}


	POINTF::POINTF()
	{
		x = 0;
		y = 0;
	}

	POINTF::POINTF(float x_, float y_) 
	{
		x = x_;
		y = y_;
	}

	POINTF::~POINTF()
	{

	}

	RECT::RECT()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	RECT::RECT(int left_, int top_, int right_, int bottom_)
	{
		left = left_;
		top = top_;
		right = right_;
		bottom = bottom_;
	}

	RECT::~RECT()
	{

	}

	bool RECT::isCollision(RECT rcOther)
	{
		if (left < rcOther.right && top <= rcOther.bottom && right > rcOther.left && bottom > rcOther.top)
			return true;
		else
			return false;
	}

	bool RECT::isPtin(POINT ptOther)
	{
		if (left < ptOther.x && right > ptOther.x &&  top < ptOther.y && bottom > ptOther.y)
			return true;
		else
			return false;
	}
}
