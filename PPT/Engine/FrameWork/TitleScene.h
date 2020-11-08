#pragma once
#include "GlobalDefine.h"
#include "JEngine.h"

class TitleScene : public JEngine::Scene
{
private:
	JEngine::BitMap*	m_pBack;
	JEngine::BitMap*	m_pTitle;

	float				m_fTitleX;
	float				m_fTitleY;

	float				m_fSpeedX;

	std::function<bool()> clickEvent;

public:
	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();
	bool OnClick();

	TitleScene();
	virtual ~TitleScene();
};

