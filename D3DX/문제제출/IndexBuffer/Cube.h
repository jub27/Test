#pragma once
#include <d3d9.h>
#include <d3dx9.h>

struct MYINDEX
{
	WORD _0, _1, _2;	// �Ϲ������� �ε����� 16��Ʈ�� ũ�⸦ ���´�.
};

class Cube
{
private:
	LPDIRECT3DINDEXBUFFER9 g_pIB = NULL; // �ε����� ������ �ε��� ����
public:
	Cube();
	~Cube();

	HRESULT InitIB(LPDIRECT3DDEVICE9 &pDevice);
	inline LPDIRECT3DINDEXBUFFER9 GetpIB()
	{
		return g_pIB;
	}
};

