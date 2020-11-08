#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "ResoucesManager.h"
#include "UIManager.h"
#include "TimeManager.h"

namespace JEngine
{
	SceneManager* SceneManager::s_this = NULL;

	SceneManager::SceneManager()
	{
		m_bInit = false;
	}


	SceneManager::~SceneManager()
	{
		
	}

	void SceneManager::Init(HWND hWnd, int win_cx, int win_cy)
	{
		m_hWnd = hWnd;
		m_iWinCX = win_cx;
		m_iWinCY = win_cy;

		::RECT rcClient, rcWindow;
		GetClientRect(hWnd, &rcClient);
		GetWindowRect(hWnd, &rcWindow);

		int CXFrame = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
		int CYFrame = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);

		MoveWindow(hWnd, 0, 0, win_cx + CXFrame, win_cy + CYFrame, true);

		HDC hdc = GetDC(m_hWnd);

		JEngine::ResoucesManager::GetInstance()->initBack(hdc, m_iWinCX, m_iWinCY);
		JEngine::InputManager::GetInstance()->Init(hWnd);

		ReleaseDC(m_hWnd, hdc);

		m_curScene = m_vecScene[0];

		m_curScene->Init(m_hWnd);
	}

	void SceneManager::RegistScene(Scene* Reg)
	{
		m_vecScene.push_back(Reg);
	}

	bool SceneManager::Update()
	{

		if (!JEngine::TimeManager::GetInstance()->Update())
			return false;

		JEngine::InputManager::GetInstance()->Update();
		JEngine::UIManager::GetInstance()->Update();

		if (m_curScene->Input(JEngine::TimeManager::GetInstance()->GetElipseTime()))
			return true;

		m_curScene->Update(JEngine::TimeManager::GetInstance()->GetElipseTime());

		HDC hdc = GetDC(m_hWnd);
		JEngine::ResoucesManager::GetInstance()->DrawBack(hdc);
		m_curScene->Draw(hdc);
		JEngine::UIManager::GetInstance()->Draw();
		ReleaseDC(m_hWnd, hdc);

		return false;
	}

	void SceneManager::Release()
	{
		for (auto iter = m_vecScene.begin(); iter != m_vecScene.end();)
		{
			Scene* pDelete = *iter;
			pDelete->Release();
			iter++;
			SAFE_DELETE(pDelete);
		}

		SAFE_DELETE(s_this);
	}

	void SceneManager::LoadScene(int SceneIndex)
	{
		UIManager::GetInstance()->RelaaseUI();
		m_lastScene = m_curScene;
		m_curScene = m_vecScene[SceneIndex];

		m_lastScene->Release();
		m_curScene->Init(m_hWnd);
	}
}