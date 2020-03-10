#pragma once
#include "Map.h"
#define MISSILE_SPEED 0.3

#define MISSILE_WIDTH 5
#define MISSILE_HEIGHT 8

enum direction {
	DOWN, LEFT, RIGHT, UP
};

class Missile
{
private:
	double x, y;
	bool playerMissile;
	direction dir;

public:
	Missile(double x, double y, direction dir, bool playerMissile);
	bool Move();
	double GetX();
	double GetY();
	bool is_PlayerMissile();
	direction GetDir();
	~Missile();
};
