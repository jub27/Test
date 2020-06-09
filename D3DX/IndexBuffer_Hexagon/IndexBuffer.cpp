/**========================================================================
 * �ε��� ���� ����
 * �ε����� �����ϱ� ���� ���� ��ü�� ���� ����
 *=========================================================================*/
#include <d3d9.h>
#include <d3dx9.h>

/**========================================================================
 * ���� ����
 *=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

LPDIRECT3DINDEXBUFFER9 g_pIB = NULL; // �ε����� ������ �ε��� ����

struct CUSTOMVERTEX
{
	FLOAT x, y, z; 
	DWORD color;		
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct MYINDEX
{
	WORD _0, _1, _2;	// �Ϲ������� �ε����� 16��Ʈ�� ũ�⸦ ���´�.
};

/**========================================================================
 * Direct3D �ʱ�ȭ
 *=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	// ����̽��� �����ϱ� ���� D3D ��ü ����
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;				
	ZeroMemory(&d3dpp, sizeof(d3dpp));			

	d3dpp.Windowed = TRUE;						
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	
	
	d3dpp.EnableAutoDepthStencil = TRUE; // ���̰� �ִ� z���۰� �ʿ��ϹǷ� �����Ѵ�.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//Z ���� ����� �Ҵ�.
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

/**========================================================================
 * ���� �ʱ�ȭ : ���� ���۸� �����ϰ� �������� ä�� �ִ´�.
 *=========================================================================*/
HRESULT InitVB()
{
	// ������ü�� �������ϱ� ���� 12���� ���� ����
	CUSTOMVERTEX vertices[] =
	{
		{-1, 1, 1, 0xffff0000},	//v0
		{ 0, 1, 2, 0xff00ff00}, //v1
		{ 1, 1, 1, 0xff00ff00}, //v2
		{ 1, 1,-1, 0xff0000ff}, //v3
		{ 0, 1,-2, 0xff00ff00}, //v4
		{-1, 1,-1, 0xffffff00}, //v5

		{-1,-1, 1, 0xffff0000}, //v6
		{ 0,-1, 2, 0xff00ff00}, //v7
		{ 1,-1, 1, 0xff00ff00}, //v8
		{ 1,-1,-1, 0xff0000ff}, //v9
		{ 0,-1,-2, 0xff0000ff}, //v10
		{-1,-1,-1, 0xffffff00}, //v11
	};

	// ���� ���۸� �����Ѵ�.
	// ���� ����ü 3���� ������ �޸𸮸� �Ҵ��Ѵ�.
	// FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(12 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	//������ ���۸� ������ ä���.
	//���� ������ Lock() �Լ��� ȣ���Ͽ� �����͸� ���´�.
	void* pVertices;

	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

/**========================================================================
 * �ε��� ���� ����
 *=========================================================================*/
HRESULT InitIB()
{
	MYINDEX indices[] =
	{
		{0, 1, 2}, {0, 2, 3}, {0, 3, 5}, {5, 3, 4},// ����
		{6, 7, 8}, {6, 8, 9}, {6, 9, 11}, {11, 9, 10},// �Ʒ���
		{0, 6, 7}, {0, 7, 1}, // �޸�
		{1, 7, 8}, {1, 8, 2}, // ������
		{2, 8, 9}, {2, 9, 3}, // �ո�
		{3, 9, 10}, {3, 10, 4}, // �޸�
		{4, 10, 11}, {4, 11, 5}, // �޸�
		{5, 11, 6}, {5, 6, 0}, // ������
	};

	//�ε��� ���� ����
	//D3DFMT_INDEX16�� �ε����� ������ 16��Ʈ��� ��.
	//MYINDEX ����ü���� WORD������ �������Ƿ� D3DFMT_INDEX16�� ����Ѵ�.
	if (FAILED(g_pd3dDevice->CreateIndexBuffer(20 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
	{
		return E_FAIL;
	}

	//�ε��� ���۸� ������ ä���.
	void* pIndices;
	//�ε��� ������ Lock() �Լ��� ȣ���Ͽ� �����͸� ���ͼ� ������ ä���.
	if (FAILED(g_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pIndices, indices, sizeof(indices));

	g_pIB->Unlock();

	return S_OK;
}

/**========================================================================
 * ��� ���� : ��Ŀ��� 3������ ������, ���� ����, ��, �������� ����̴�.
 *=========================================================================*/
void SetupMatrices()
{
	//���� ���
	D3DXMATRIXA16 matWorld;

	D3DXMatrixIdentity(&matWorld); //���� ����� ���� ��ķ� ����

	D3DXMatrixRotationY(&matWorld, GetTickCount() / 500.0f); // Y���� ȸ�������� ȸ�� ����� �����Ѵ�.

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld); // ������ ȸ�� ����� ���� ��ķ� ����̽��� �����Ѵ�.

	//�� ���(ī�޶�)�� �����ϱ� ���ؼ��� 3���� ���� �ʿ��ϴ�.
	D3DXVECTOR3 vEyePt(0.0f, 5.0f, -10.0f); //���� ��ġ(0.0f, 3.0f, -5.0f)
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f); // ���� �ٶ󺸴� ��ġ(0.0f, 0.0f, 0.0f)
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // �� ������ ��Ÿ���� ��� ����(0.0f, 1.0f, 0.0f)

	//�� ���(ī�޶�)
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);// 3���� Point�� �̿��� �� ����� �����Ѵ�.

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); // ������ �� ����� ����̽��� �����Ѵ�.

	// �������� ����� �����ϱ� ���ؼ��� �þ߰�(FOV = Field Of View)�� ��Ⱦ��(aspect ratio), Ŭ���� ����� ���� �ʿ��ϴ�.
	D3DXMATRIXA16 matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj); // ������ �������� ����� ����̽��� ����.
}

/**========================================================================
 * �ʱ�ȭ ��ü�� Release
 *=========================================================================*/
void Cleanup()
{
	//���� ������ �߿� �������̽� ������ �������� ��������.
	if (g_pIB != NULL)
		g_pIB->Release();

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

	// �ĸ� ���۸� �Ķ���(0, 0, 255)���� ����� Z���۸� ����ϱ� ���� �ʱ�ȭ �Ѵ�.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//����, ��, �������� ����� �����Ѵ�.
	SetupMatrices();

	// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// ���� ������ �ﰢ���� �׸���.

		// ���� ������ ��� �ִ� ���� ���۸� ��� ��Ʈ������ �Ҵ��Ѵ�.
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		// D3D���� ���� ���̴� ������ �����Ѵ�. ��κ��� ��쿡�� FVF�� �����Ѵ�.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// �ε��� ���۸� �����Ѵ�.
		g_pd3dDevice->SetIndices(g_pIB);

		// ���� ������ ����ϱ� ���� DrawIndexPrimitive() �Լ��� ȣ���Ѵ�.
		// ù ��° : �׸����� �ϴ� �⺻ Ÿ����
		// �� ��° : ���� ������Ʈ�� �ϳ��� ������ ������ �ѹ�.
		// �� ��° : ������ �ּ� �ε��� ��
		// �� ��° : �̹� ȣ�⿡ ������ ���ؽ��� ��
		// �ټ� ��° : �ε��� ���� ������ �б⸦ ������ ��ҷ��� �ε���
		// ���� ��° : �׸����� �ϴ� �⺻���� ��
		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 12, 0, 20);


		//������ ����
		g_pd3dDevice->EndScene();
	}

	// �ĸ� ���۸� ���̴� ȭ������ ��ȯ.
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
	//������ Ŭ���� ���
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D IndexBuffer", NULL };

	//winclass �������Ϳ� ���
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D IndexBuffer", L"D3D IndexBuffer", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitVB()))
		{
			if (SUCCEEDED(InitIB()))
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
	}

	//��ϵ� ������Ʈ winclass ������.
	UnregisterClass(L"D3D IndexBuffer", wc.hInstance);
	return 0;
}



