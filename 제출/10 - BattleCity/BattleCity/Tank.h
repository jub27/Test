#pragma once
#include <Windows.h>
#include "Missile.h"
#include "Map.h"

#define TANK_SPEED 1
#define TANK_WIDTH 32
#define TANK_HEIGHT 26

enum tank_state {
	LIVE, DEAD, EXPLOSION
};

class Tank
{
protected:
	int x, y;
	direction m_dir;
	int motion;
	tank_state state;
	bool Player;
	DWORD MoveTick;
	DWORD MoveTime;
	DWORD ShotTick;
	DWORD ExplosionTick;
	DWORD ShieldTick;
	bool shield;
public:
	Tank(int x, int y, direction dir, bool Player);
	Missile * Shot();
	void Move(direction dir);
	bool Collision();
	int GetMotion();
	double GetX();
	double GetY();
	bool Moveable(direction dir);
	void Live();
	void Die();
	void Explosion();
	void ShieldOn();
	void ShieldOff();
	DWORD GetExplisionTick();
	DWORD GetShieldTick();
	bool is_Shield();
	tank_state GetState();
	direction GetDir();
	~Tank();
};