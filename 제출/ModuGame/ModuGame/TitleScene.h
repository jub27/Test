#pragma once
#include "GlobalDefine.h"
#include "JEngine.h"
#include "Label.h"
#include "defines.h"

class TitleScene : public JEngine::Scene
{
private:
	JEngine::BitMap* m_pGameSelect;
	JEngine::BitMap* m_pTitle;
	JEngine::Label* m_pScore[2];

	float				m_fTitleX;
	float				m_fTitleY;

	float				m_fSpeedX;

	std::function<bool()> clickEvent;

	int score[2];
	LPCSTR saveData[2] = { "save1.txt", "save2.txt" };

public:
	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();
	bool OnClick1();
	bool OnClick2();

	void LoadScore();

	TitleScene();
	virtual ~TitleScene();
};