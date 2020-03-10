#pragma once
#include "Map.h"
#define MISSILE_SPEED 2

#define MISSILE_WIDTH 5
#define MISSILE_HEIGHT 8

enum direction {
	DOWN, LEFT, RIGHT , UP
};

class Missile
{
private:
	double x, y;
	direction dir;

public:
	Missile(double x, double y, direction dir);
	bool Move();
	double GetX();
	double GetY();
	direction GetDir();
	~Missile();
};
