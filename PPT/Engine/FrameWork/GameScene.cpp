#include "GameScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResoucesManager.h"
#include <Windows.h>
#include "defines.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(HWND hWnd)
{
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_ESCAPE);
	m_pBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("back_game.bmp");
}

bool GameScene::Input(float fETime)
{
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_SPACE))
		JEngine::SceneManager::GetInstance()->LoadScene(SCENE_INDEX_TITLE);
	if (JEngine::InputManager::GetInstance()->isKeyUp(VK_ESCAPE))
		return true;

	return false;
}

float time = 0.0f;

void GameScene::Update(float fETime)
{
	time = fETime;
}

void GameScene::Draw(HDC hdc)
{

}

void GameScene::Release()
{

}
