#include "Game2.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResoucesManager.h"
#include <Windows.h>
#include "defines.h"

Game2::Game2()
{
}

Game2::~Game2()
{
}

void Game2::Init(HWND hWnd)
{
	JEngine::InputManager::GetInstance()->Clear();
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_ESCAPE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_UP);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_DOWN);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);
	time = 0.0f;
	m_pBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("res\\FlightGameBack.bmp");
}

bool Game2::Input(float fETime)
{
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_SPACE))
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_ESCAPE))
		return true;

	return false;
}

void Game2::Update(float fETime)
{
	time = fETime;
}

void Game2::Draw(HDC hdc)
{

}

void Game2::Release()
{

}