#pragma once
#include "GlobalDefine.h"
#include "JEngine.h"

class Game2 : public JEngine::Scene
{
private:
	JEngine::BitMap* m_pBack;
	float time;
public:
	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();
	//ool ClickEvent();

	Game2();
	~Game2();
};

