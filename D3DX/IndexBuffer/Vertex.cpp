#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::~Vertex()
{
}

HRESULT Vertex::InitVB(LPDIRECT3DDEVICE9 &pDevice, float x, float y, float z, float Size)
{
	// 상자(cube)를 랜더링하기 위해 8개의 정점 선언
	CUSTOMVERTEX vertices[] =
	{
		{-Size + x, Size + y, Size + z, 0xffff0000}, //v0
		{ Size + x, Size + y, Size + z, 0xff00ff00}, //v1
		{ Size + x, Size + y,-Size + z, 0xff0000ff}, //v2
		{-Size + x, Size + y,-Size + z, 0xffffff00}, //v3

		{-Size + x,-Size + y, Size + z, 0xffff0000}, //v4
		{ Size + x,-Size + y, Size + z, 0xff00ff00}, //v5
		{ Size + x,-Size + y,-Size + z, 0xff0000ff}, //v6
		{-Size + x,-Size + y,-Size + z, 0xffffff00}, //v7
	};

	if (FAILED(pDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	//정점의 버퍼를 값으로 채운다.
	//정점 버퍼의 Lock() 함수를 호출하여 포인터를 얻어온다.
	void* pVertices;

	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}