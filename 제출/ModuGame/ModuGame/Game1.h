#pragma once
#include "GlobalDefine.h"
#include "JEngine.h"
#include "Label.h"

enum Color {
	BLUE, GREEN, RED, YELLOW
};

enum Direction {
	UP, DOWN, LEFT, RIGHT
};

class Game1 : public JEngine::Scene
{
private:
	JEngine::BitMap* m_pBack;
	JEngine::BitMap* m_pColorList[4];
	JEngine::BitMap* m_pTimeBar;
	JEngine::Label* m_pPoint;
	JEngine::Label* m_pPaperPoint;
	JEngine::BitMap* m_pFever[3];
	JEngine::BitMap* m_pStar[3];

	Color Board[2];
	bool is_moving;
	Direction paperDirection;
	
	int point;
	int paperPoint;
	bool is_star;

	int comboCount;
	int feverLevel;
	float feverGauge;

	float gameTime;
	float paperX, paperY;
	float moveSpeed;
	float time;
public:
	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();
	//ool ClickEvent();

	void MovePaper();

	Game1();
	~Game1();
};

