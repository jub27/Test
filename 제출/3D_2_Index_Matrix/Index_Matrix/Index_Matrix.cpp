/**========================================================================
 * ��� ���
 * ����� �����ϰ� ��ȯ�ϴ� ���� �����ϰ� �ȴ�.
 *=========================================================================*/

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h> // TimeGetTime() �Լ��� ����ϱ� ���ؼ� �����ϴ� ���

 /**========================================================================
  * ���� ����
  *=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB2 = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB3 = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIB = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIB2 = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIB3 = NULL;

struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

struct MYINDEX {
	WORD _0, _1, _2;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

/**========================================================================
 * Direct3D �ʱ�ȭ
 *=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// �ø� ����� ����, �ﰢ���� �ո�, �޸��� ��� �������Ѵ�.
	//g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//������ ������ �����Ƿ� ���� ����� ����.
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

/**========================================================================
 * ���� �ʱ�ȭ : ���� ���۸� �����ϰ� �������� ä�� �ִ´�.
 *=========================================================================*/
HRESULT InitVB()
{
	void* pVertices;
	void* pIndices;
	MYINDEX indices[] = {
		{4, 6, 5}, {4, 7, 6}, // �Ʒ���
		{0, 1, 2}, {0, 2, 3}, // ����
		{0, 3, 7}, {0, 7, 4}, // �޸�
		{1, 5, 6}, {1, 6, 2}, // ������
		{3, 2, 6}, {3, 6, 7}, // �ո�
		{0, 4, 1}, {1, 4, 5}, // �޸�
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL))) {
		return E_FAIL;
	}

	if (FAILED(g_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0))) {
		return E_FAIL;
	}

	memcpy(pIndices, indices, sizeof(indices));

	g_pIB->Unlock();

	MYINDEX indices2[] = {
		{0,1,2},{0,2,3}
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(2 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB2, NULL))) {
		return E_FAIL;
	}

	if (FAILED(g_pIB2->Lock(0, sizeof(indices2), (void**)&pIndices, 0))) {
		return E_FAIL;
	}

	memcpy(pIndices, indices2, sizeof(indices2));

	g_pIB2->Unlock();


	MYINDEX indices3[] = {
		{0,1,2},{0,2,3},
		{0,3,4}
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(3 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB3, NULL))) {
		return E_FAIL;
	}

	if (FAILED(g_pIB3->Lock(0, sizeof(indices3), (void**)&pIndices, 0))) {
		return E_FAIL;
	}

	memcpy(pIndices, indices3, sizeof(indices3));

	g_pIB3->Unlock();

	CUSTOMVERTEX vertices2[] =
	{
		{ -1.0f, 6.0f, 1.0f, 0xffff0000, }, // x, y, z, color
		{ 1.0f, 6.0f, 1.0f, 0xff00ff00, },
		{ 1.0f, 4.0f, 1.0f, 0xff00ffff, },
		{ -1.0f, 4.0f, 1.0f, 0xffffff00, },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB2, NULL)))
	{
		return E_FAIL;
	}


	if (FAILED(g_pVB2->Lock(0, sizeof(vertices2), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices2, sizeof(vertices2));

	g_pVB2->Unlock();

	CUSTOMVERTEX vertices3[] =
	{
		{ 5.0f, 6.0f, 1.0f, 0xffff0000, }, // x, y, z, color
		{ 6.0f, 5.0f, 1.0f, 0xff00ff00, },
		{ 5.5f, 4.0f, 1.0f, 0xff00ffff, },
		{ 4.5f, 4.0f, 1.0f, 0xffffff00, },
		{ 4.0f, 5.0f, 1.0f, 0xff00ffff, },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(45 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB3, NULL)))
	{
		return E_FAIL;
	}


	if (FAILED(g_pVB3->Lock(0, sizeof(vertices3), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices3, sizeof(vertices3));

	g_pVB2->Unlock();

	CUSTOMVERTEX vertices[] =
	{
		{ -1.0f, 1.0f, 1.0f, 0xffff0000, }, // x, y, z, color
		{ 1.0f, 1.0f, 1.0f, 0xff00ff00, },
		{ 1.0f, 1.0f, -1.0f, 0xff00ffff, },
		{ -1.0f, 1.0f, -1.0f, 0xffffff00, },

		{ -1.0f, -1.0f, 1.0f, 0xff00ffff, },
		{ 1.0f, -1.0f, 1.0f, 0xffff00ff, },
		{ 1.0f, -1.0f, -1.0f, 0xff000000, },
		{ -1.0f, -1.0f, -1.0f, 0xffffffff, },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}


	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

/**========================================================================
 * ��� ���� : ��Ŀ��� 3������ ������, ���� ����, ��, �������� ����̴�.
 *=========================================================================*/
void SetupMatrices()
{
	//���� ���
	D3DXMATRIXA16 matWorld;

	UINT iTime = timeGetTime() % 1000; // float ������ ���е��� ���ؼ� 1000���� ������ �����Ѵ�.

	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f; // 1000�и��ʸ��� �� ������(2 * pi) ȸ�� �ִϸ��̼� ����� �����.

	//D3DXMatrixRotationY(&matWorld, GetTickCount() / 500.0f);

	D3DXMatrixRotationY(&matWorld, fAngle); // Y���� ȸ�������� ȸ�� ����� �����Ѵ�.

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld); // ������ ȸ�� ����� ���� ��ķ� ����̽��� �����Ѵ�.

	//�� ���(ī�޶�)�� �����ϱ� ���ؼ��� 3���� ���� �ʿ��ϴ�.
	D3DXVECTOR3 vEyePt(0.0f, 10.0f, -10.0f); //���� ��ġ(0.0f, 3.0f, -5.0f)
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f); // ���� �ٶ󺸴� ��ġ(0.0f, 0.0f, 0.0f)
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // �� ������ ��Ÿ���� ��� ����(0.0f, 1.0f, 0.0f)

	//�� ���(ī�޶�)
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);// 3���� Point�� �̿��� �� ����� �����Ѵ�.

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); // ������ �� ����� ����̽��� �����Ѵ�.

	// �������� ����� �����ϱ� ���ؼ��� �þ߰�(FOV = Field Of View)�� ��Ⱦ��(aspect ratio), Ŭ���� ����� ���� �ʿ��ϴ�.
	D3DXMATRIXA16 matProj;
	//ù ��° : ������ ���
	//�� ��° : �þ߰�
	//�� ��° : ��Ⱦ��
	//�� ��° : ���� Ŭ����
	//�ټ� ��° : ���Ÿ� Ŭ����
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj); // ������ �������� ����� ����̽��� ����.
}


/**========================================================================
 * �ʱ�ȭ ��ü�� Release
 *=========================================================================*/
void Cleanup()
{
	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

/**========================================================================
 * ȭ�� �׸���
 *=========================================================================*/
void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//����, ��, �������� ����� �����Ѵ�.
	SetupMatrices();

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		g_pd3dDevice->SetIndices(g_pIB);

		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		g_pd3dDevice->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));

		g_pd3dDevice->SetIndices(g_pIB2);

		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		g_pd3dDevice->SetStreamSource(0, g_pVB3, 0, sizeof(CUSTOMVERTEX));

		g_pd3dDevice->SetIndices(g_pIB3);

		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 3);

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

/**========================================================================
 * WinProc
 *=========================================================================*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**========================================================================
 * Window ����
 *=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Matrices", NULL };

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D Matrices", L"D3D Matrices", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitVB()))
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));

			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass(L"D3D Matrices", wc.hInstance);
	return 0;
}



