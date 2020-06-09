#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

class Vertex
{
private:
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
public:
	Vertex();
	~Vertex();

	HRESULT InitVB(LPDIRECT3DDEVICE9 &pDevice, float x, float y, float z, float Size);
	inline LPDIRECT3DVERTEXBUFFER9 GetpVB()
	{
		return g_pVB;
	}
};

