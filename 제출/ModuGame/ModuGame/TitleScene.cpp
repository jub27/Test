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

	m_pGameSelect = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\GameSelect.bmp");
	m_pTitle = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\Title.bmp");

	JEngine::UIManager::GetInstance()->AddButton(GAME_WIDTH - 100, 120, "res\\OnSelect.bmp", std::bind(&TitleScene::OnClick1, this));
	JEngine::UIManager::GetInstance()->AddButton(GAME_WIDTH - 100, 200, "res\\OnSelect.bmp", std::bind(&TitleScene::OnClick2, this));
	m_pScore[0] = new JEngine::Label();
	m_pScore[1] = new JEngine::Label();

	LoadScore();
}

bool TitleScene::Input(float fETime)
{
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_SPACE))
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);
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
	m_pTitle->DrawBitblt(0,0);
	m_pGameSelect->DrawBitblt(10, 10);
	string a = "best score : ";
	m_pScore[0]->Init(a.append(to_string(score[0])), 120, 150, DT_CENTER | DT_WORDBREAK);
	a = "best score : ";
	m_pScore[1]->Init(a.append(to_string(score[1])), 120, 230, DT_CENTER | DT_WORDBREAK);
	m_pScore[0]->Draw();
	m_pScore[1]->Draw();
}

void TitleScene::Release()
{

}

bool TitleScene::OnClick1()
{
	JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_GAME1);
	return true;
}

bool TitleScene::OnClick2()
{
	JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_GAME2);
	return true;
}

void TitleScene::LoadScore() {
	for (int i = 0; i < 2; i++) {
		HANDLE hFile = CreateFile(saveData[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		DWORD readB;
		ReadFile(hFile, &score[i], sizeof(int), &readB, NULL);
		CloseHandle(hFile);
	}
}