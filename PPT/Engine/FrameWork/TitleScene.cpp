#include "TitleScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResoucesManager.h"
#include "defines.h"


TitleScene::TitleScene()
{
	m_fTitleX = 1024.0f;
	m_fTitleY = 768.0f;

	m_fSpeedX = 200;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(HWND hWnd)
{
	JEngine::InputManager::GetInstance()->Clear();
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_ESCAPE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);

	m_pBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("back.bmp");
	m_pTitle = JEngine::ResoucesManager::GetInstance()->GetBitmap("char.bmp");

	JEngine::UIManager::GetInstance()->AddButton(10, 20, "OnSelect.bmp", std::bind(&TitleScene::OnClick, this));
	m_pTitle->SetAnchor(JEngine::ANCHOR_RB);
}

bool TitleScene::Input(float fETime)
{
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_SPACE))
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_GAME);
	else if (JEngine::InputManager::GetInstance()->isKeyUp(VK_ESCAPE))
		return true;
	else if (JEngine::InputManager::GetInstance()->isKeyPress(VK_LEFT))
	{
		m_fTitleX -= fETime * m_fSpeedX;
	}
	else if (JEngine::InputManager::GetInstance()->isKeyPress(VK_RIGHT))
	{
		m_fTitleX += fETime * m_fSpeedX;
	}


	return false;
}

void TitleScene::Update(float fETime)
{

}

void TitleScene::Draw(HDC hdc)
{
	m_pBack->DrawBitblt(0, 0);
	m_pTitle->Draw((int)m_fTitleX, (int)m_fTitleY);
}

void TitleScene::Release()
{

}

bool TitleScene::OnClick()
{
	return true;
}