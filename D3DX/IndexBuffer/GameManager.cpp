#include "GameManager.h"



GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

void GameManager::Cleanup()
{
	if(m_Cube.GetpIB() != NULL)
		m_Cube.GetpIB()->Release();

	for (int i = 0; i < sizeof(m_Vertex) / sizeof(Vertex); i++)
	{
		if (m_Vertex[i].GetpVB() != NULL)
			m_Vertex[i].GetpVB()->Release();
	}
}

void GameManager::Update()
{
	SpeedAccel();
	/*if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	{
		GetCursorPos(&m_MousePos);
		ScreenToClient(m_hWnd, &m_MousePos);
	}*/
}

