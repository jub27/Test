#include "Missile.h"
Missile::Missile(double x, double y, direction dir) {
	this->x = x;
	this->y = y;
	this->dir = dir;
}
bool Missile::Move() {
	RECT rect = { x, y, x + MISSILE_WIDTH, y + MISSILE_HEIGHT / 2 };
	if (Map::GetInstance()->MissileCollisioin(rect))
		return false;
	switch (dir) {
	case UP:
		y -= MISSILE_SPEED;
		if (y <= -MISSILE_HEIGHT)
			return false;
		break;
	case DOWN:
		y += MISSILE_SPEED;
		if (y >= BLOCK_HEIGHT * MAP_SIZE)
			return false;
		break;
	case LEFT:
		x -= MISSILE_SPEED;
		if (x <= -MISSILE_WIDTH)
			return false;
		break;
	case RIGHT:
		x += MISSILE_SPEED;
		if (x >= BLOCK_WIDTH * MAP_SIZE)
			return false;
		break;
	}
	return true;
}

double Missile::GetX() {
	return x;
}

double Missile::GetY() {
	return y;
}

direction Missile::GetDir() {
	return dir;
}

Missile::~Missile() {

}