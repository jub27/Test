/**========================================================================
 * ���� ������
 * ������ ������ �ϴ� ���� �����ϰ� �ȴ�.
 *=========================================================================*/

 // Direct3D9�� ����ϱ� ���� ���
#include <d3d9.h>
#include <d3dx9.h>

/**========================================================================
 * ���� ����
 *=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; //������ ������ ���� ����
LPDIRECT3DVERTEXBUFFER9 g_pVB2 = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB3 = NULL;
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; //������ ��ȯ�� ��ǥ(rhw ���� ������ ��ȯ�� �Ϸ�� �����̴�.)
	DWORD color;		//������ ����
};

CUSTOMVERTEX vertices[] =
{
	{ 200.0f, 100.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 200.0f, 200.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 100.0f,  100.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), }, // x, y, z, rhw, color
	{ 100.0f, 200.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
};

CUSTOMVERTEX vertices2[] =
{
	{ 300.0f, 100.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 350.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 325.0f, 150.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), }, // x, y, z, rhw, color
	{ 400.0f, 100.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 375.0f, 150.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
};

CUSTOMVERTEX vertices3[] =
{
	{ 500.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 550.0f, 83.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 450.0f, 83.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), }, // x, y, z, rhw, color
	{ 550.0f, 116.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 450.0f, 116.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
	{ 500.0f, 150.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0), },
};


float Speed_X = 2.0f;

// ����� ���� ����ü�� ���� ������ ��Ÿ���� FVF��
// ����ü�� x, y, z, RHW ���� Diffuse ���� ������ �̷���� ������ �� �� �ִ�.
// D3DFVF_DIFFUSE �ɼ����� �������� ������ �ִٴ°� �˷��ش�.
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

/**========================================================================
 * Direct3D �ʱ�ȭ
 *=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	// ����̽��� �����ϱ� ���� D3D ��ü ����
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;				// ����̽� ������ ���� ����ü
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// �ݵ�� ZeroMemory() �Լ��� �̸� ����ü�� ������ ������ �Ѵ�.

	d3dpp.Windowed = TRUE;						// â ���� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ���� ȿ������ SWAP ȿ��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// ���� ����ȭ�� ��忡 ���缭 �ĸ� ���۸� ����.

	// ����̽��� �����ؼ� ����
	// ����Ʈ ����ī�带 ����ϰ�, HAL ����̽��� �����Ѵ�.
	// ���� ó���� ��� ī�忡�� �����ϴ� SWó���� �����Ѵ�.
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}


	// ����̽� ���� ������ ó���� ��� ���⿡�� �Ѵ�.

	return S_OK;
}

/**========================================================================
 * ���� �ʱ�ȭ : ���� ���۸� �����ϰ� �������� ä�� �ִ´�.
 *
 * ���� ���۶� �⺻������ ���� ������ ���� �ִ� �޸� ����̴�.
 * ���� ���۸� ������ �������� �ݵ�� Lock()�� Unlock()���� �����͸� ����
 * ���� ������ ���� ���ۿ� ��־�� �Ѵ�.
 * ���� D3D�� �ε��� ���۵� ��� �����ϴٴ� ���� �������.
 * ���� ���۳� �ε��� ���۴� �⺻ �ý��� �޸� �ܿ� ����̽� �޸�(����ī�� �޸�)��
 * ������ �� �ִµ�, ��κ��� ����ī�忡���� �̷��� �� ��� ��û�� �ӵ�
 * ����� ���� �� �ִ�.
 *=========================================================================*/
HRESULT InitVB()
{
	// �ﰢ���� �������ϱ� ���� 3���� ���� ����

	// ���� ���۸� �����Ѵ�.
	// ���� ����ü 3���� ������ �޸𸮸� �Ҵ��Ѵ�.
	// FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(5 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB2, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB3, NULL)))
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

	if (FAILED(g_pVB2->Lock(0, sizeof(vertices2), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices2, sizeof(vertices2));

	g_pVB2->Unlock();

	if (FAILED(g_pVB3->Lock(0, sizeof(vertices3), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices3, sizeof(vertices3));

	g_pVB2->Unlock();

	return S_OK;
}

/**========================================================================
 * �ʱ�ȭ ��ü�� Release
 *=========================================================================*/
void Cleanup()
{
	//���� ������ �߿� �������̽� ������ �������� ��������.
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

	// �ĸ� ���۸� �Ķ���(0, 0, 255)���� �����.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		g_pd3dDevice->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 3);

		g_pd3dDevice->SetStreamSource(0, g_pVB3, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

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
		L"D3D Vertices", NULL };

	//winclass �������Ϳ� ���
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D Vertices", L"D3D Vertices", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
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
				{
					Render();
				}
			}
		}
	}

	//��ϵ� ������Ʈ winclass ������.
	UnregisterClass(L"D3D Vertices", wc.hInstance);
	return 0;
}