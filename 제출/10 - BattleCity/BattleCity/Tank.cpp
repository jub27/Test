#include "Tank.h"

Tank::Tank(int x, int y, direction dir, bool Player) {
	this->x = x;
	this->y = y;
	this->m_dir = dir;
	this->Player = Player;
	motion = 0;
	state = DEAD;
}

double Tank::GetX() {
	return x;
}

double Tank::GetY() {
	return y;
}

direction Tank::GetDir() {
	return m_dir;
}

bool Tank::Collision() {
	return false;
}

void Tank::ShieldOn() {
	shield = true;
	ShieldTick = GetTickCount();
}

void Tank::ShieldOff() {
	shield = false;
}

Missile* Tank::Shot() {
	if (GetTickCount() - ShotTick >= 500) {
		ShotTick = GetTickCount();
	}
	else
		return  NULL;
	int missile_x = 0, missile_y = 0;
	switch (m_dir) {
	case UP:
		missile_x = x + TANK_WIDTH / 2 - MISSILE_WIDTH / 2;
		missile_y = y - MISSILE_HEIGHT;
		break;
	case DOWN:
		missile_x = x + TANK_WIDTH / 2 - MISSILE_WIDTH / 2;
		missile_y = y + TANK_HEIGHT + MISSILE_HEIGHT / 2;
		break;
	case LEFT:
		missile_x = x - MISSILE_HEIGHT;
		missile_y = y + TANK_HEIGHT / 2 - MISSILE_WIDTH / 2;
		break;
	case RIGHT:
		missile_x = x + TANK_WIDTH;
		missile_y = y + TANK_HEIGHT / 2 - MISSILE_WIDTH / 2;
		break;
	}
	return new Missile(missile_x, missile_y, m_dir);
}

bool Tank::Moveable(direction dir) {
	RECT rect;
	switch (dir) {
	case DOWN:
		rect = { x + 4, y + TANK_SPEED + 4, x + TANK_WIDTH - 4, y + TANK_HEIGHT + TANK_SPEED - 4};
		if (y >= 13 * 26 - TANK_HEIGHT || Map::GetInstance()->TankCollision(rect))
			return false;
		else
			return true;
	case LEFT:
		rect = { x - TANK_SPEED + 4, y + 4, x - TANK_SPEED + TANK_WIDTH - 4, y + TANK_HEIGHT - 4 };
		if (x <= 0 || Map::GetInstance()->TankCollision(rect))
			return false;
		else
			return true;
	case RIGHT:
		rect = { x + TANK_SPEED + 4, y + 4, x + TANK_SPEED + TANK_WIDTH - 4, y + TANK_HEIGHT - 4 };
		if (x >= 16 * 26 - TANK_WIDTH || Map::GetInstance()->TankCollision(rect))
			return false;
		else
			return true;
	case UP:
		rect = { x + 4, y - TANK_SPEED + 3, x + TANK_WIDTH - 4, y - TANK_SPEED + TANK_HEIGHT - 4 };
		if (y <= 0 || Map::GetInstance()->TankCollision(rect))
			return false;
		return true;
	}

	return true;
}

void Tank::Move(direction dir) {
	if (!Player) {
		if (GetTickCount() - MoveTick > MoveTime) {
			m_dir = dir;
			MoveTick = GetTickCount();
			MoveTime = rand() % 500;
		}
	}
	else
		m_dir = dir;
	switch (m_dir) {
	case UP:
		if(Moveable(m_dir))
			y -= TANK_SPEED;
		break;
	case DOWN:
		if (Moveable(m_dir))
			y += TANK_SPEED;
		break;
	case LEFT:
		if (Moveable(m_dir))
			x -= TANK_SPEED;
		break;
	case RIGHT:
		if (Moveable(m_dir))
			x += TANK_SPEED;
		break;
	}
	if (motion == 0)
		motion = 1;
	else
		motion = 0;
}

int Tank::GetMotion() {
	return motion;
}

void Tank::Live() {
	state = LIVE;
	ShotTick = GetTickCount();
	MoveTick = GetTickCount();
	MoveTime = rand() % 500;
	ShieldOn();
}

void Tank::Die() {
	state = DEAD;
}

void Tank::Explosion() {
	state = EXPLOSION;
	ExplosionTick = GetTickCount();
}

DWORD Tank::GetExplisionTick() {
	return ExplosionTick;
}

DWORD Tank::GetShieldTick() {
	return ShieldTick;
}

bool Tank::is_Shield() {
	return shield;
}

tank_state Tank::GetState() {
	return state;
}

Tank::~Tank() {

}