#pragma once
#include <d3d9.h>
#include <d3dx9.h>

struct MYINDEX
{
	WORD _0, _1, _2;	// 일반적으로 인덱스는 16비트의 크기를 갖는다.
};

class Cube
{
private:
	LPDIRECT3DINDEXBUFFER9 g_pIB = NULL; // 인덱스를 저장할 인덱스 버퍼
public:
	Cube();
	~Cube();

	HRESULT InitIB(LPDIRECT3DDEVICE9 &pDevice);
	inline LPDIRECT3DINDEXBUFFER9 GetpIB()
	{
		return g_pIB;
	}
};

