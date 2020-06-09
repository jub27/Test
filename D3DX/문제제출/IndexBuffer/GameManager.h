#pragma once
#include"Vertex.h"
#include"Cube.h"

class GameManager
{
private:
	//월드 행렬
	HWND m_hWnd;
	D3DXMATRIXA16 matWorld;
	Vertex m_Vertex[2];
	Cube m_Cube;
	POINT m_MousePos;
	float m_iSpeed = 0.0f;
public:
	GameManager();
	~GameManager();

	void Init(HWND hWnd);
	void Cleanup();
	void Update();

	inline void SpeedAccel()
	{
		m_iSpeed += 0.1f;
	}
	inline D3DXMATRIXA16* GetmatWorld()
	{
		return &matWorld;
	}
	inline Vertex* GetVertex(int i)
	{
		return &m_Vertex[i];
	}
	inline Cube* GetCube()
	{
		return &m_Cube;
	}
	inline float GetSpeed()
	{
		return m_iSpeed;
	}
};

