#pragma once
#include "GlobalDefine.h"
#include "JEngine.h"
#include <vector>

struct Bullet {
	float x;
	float y;
	float dx;
	float dy;

	Bullet(float x, float y, float dx, float dy) {
		this->x = x;
		this->y = y;
		this->dx = dx;
		this->dy = dy;
	}

	void Move() {
		x += dx;
		y += dy;
	}

	bool rangeOut() {
		if (x < 0 || y < 0 || x > 414 || y > 641)
			return true;
		return false;
	}
};

struct Star {
	float x;
	float y;
	float dx;
	float dy;

	Star(float x, float y, float dx, float dy) {
		this->x = x;
		this->y = y;
		this->dx = dx;
		this->dy = dy;
	}

	void Move() {
		x += dx;
		y += dy;
	}

	bool rangeOut() {
		if (x < 0 || y < 0 || x > 414 || y > 641)
			return true;
		return false;
	}
};

class Game2 : public JEngine::Scene
{
private:
	JEngine::BitMap* m_pBack;
	JEngine::BitMap* m_pTimeBar;
	JEngine::BitMap* m_pFlight;
	JEngine::BitMap* m_pBullet;
	JEngine::BitMap* m_pExplosion[2];

	JEngine::POINT flightPoint;

	vector<Bullet*> bulletList;
	float bulletTime;

	vector<Star*> starList;
	float starTime;

	bool explosion;
	float explosionX;
	float explosionY;

	float explosionTime;
	float gameTime;
public:
	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();
	//ool ClickEvent();
	void UpdateBullet();
	void UpdateStar();
	bool collisionCheck();

	Game2();
	~Game2();
};

