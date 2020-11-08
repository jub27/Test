#include "InputManager.h"
#include <algorithm>


namespace JEngine
{
	InputManager::InputManager()
	{
		m_vecKeyRegist.clear();
	}


	InputManager::~InputManager()
	{

	}

	void InputManager::Init(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	bool InputManager::isExistKey(int key)
	{
		for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
		{
			if ((*iter).keyCode == key)
				return true;
		}

		return false;
	}


	void InputManager::RegistKeyCode(int key)
	{
		if (isExistKey(key))
		{
			return;
		}
		else
		{
			KEY_INFO code;
			memset(&code, 0, sizeof(KEY_INFO));
			code.keyCode = key;
			m_vecKeyRegist.push_back(code);
		}
	}

	void InputManager::Update()
	{
		::POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);
		m_ptMouse.x = pt.x;
		m_ptMouse.y = pt.y;

		for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
		{
			if (GetKeyState((*iter).keyCode) & 0x8000)
			{
				if ((*iter).KeyPrecce == false)//키 다운
				{
					(*iter).KeyDown = true;
				}
				else
				{
					(*iter).KeyDown = false;
				}

				(*iter).KeyPrecce = true;
				(*iter).keyUp = false;
			}
			else
			{
				if ((*iter).KeyPrecce)//키 업
				{
					(*iter).keyUp = true;
				}
				else
				{
					(*iter).keyUp = false;
				}

				(*iter).KeyDown = false;
				(*iter).KeyPrecce = false;
			}
		}
	}

	void InputManager::Clear()
	{
		m_vecKeyRegist.clear();
	}

	POINT InputManager::GetMousePoint()
	{
		return m_ptMouse;
	}

	bool InputManager::isKeyPress(int keyCode)
	{
		for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
		{
			if ((*iter).keyCode == keyCode)
			{
				return (*iter).KeyPrecce;
			}
		}

		assert(!("isKeyPress keyCode not regist!!"));
		return false;
	}

	bool InputManager::isKeyUp(int keyCode)
	{
		for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
		{
			if ((*iter).keyCode == keyCode)
			{
				return (*iter).keyUp;
			}
		}

		assert(!("isKeyUp keyCode not regist!!"));
		return false;
	}

	bool InputManager::isKeyDown(int keyCode)
	{
		for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
		{
			if ((*iter).keyCode == keyCode)
			{
				return (*iter).KeyDown;
			}
		}

		assert(!("isKeyDown keyCode not regist!!"));
		return false;
	}
}