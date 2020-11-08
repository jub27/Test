#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pSunVB = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pEarthVB = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pMoonVB = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIB = NULL;

D3DXMATRIXA16 g_matTMSun;
D3DXMATRIXA16 g_matRSun;

D3DXMATRIXA16 g_matSEarth;
D3DXMATRIXA16 g_matTMEarth;
D3DXMATRIXA16 g_matREarth;
D3DXMATRIXA16 g_matRevEarth;

D3DXMATRIXA16 g_matSMoon;
D3DXMATRIXA16 g_matTMMoon;
D3DXMATRIXA16 g_matRMoon;
D3DXMATRIXA16 g_matRevMoon;

struct CUSTOMVERTEX {
	FLOAT x, y, z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct MYINDEX {
	WORD _0, _1, _2;
};

HRESULT InitD3D(HWND hWnd) {

	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		return E_FAIL;
	}

	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;

}

HRESULT InitVB() {
	CUSTOMVERTEX sunVertices[] =
	{
		{-1, 1, 1, D3DCOLOR_XRGB(255,255,0)},
		{ 1, 1, 1, D3DCOLOR_XRGB(255,0,0)},
		{ 1, 1,-1, D3DCOLOR_XRGB(255,255,0)},
		{-1, 1,-1, D3DCOLOR_XRGB(255,0,0)},

		{-1,-1, 1, D3DCOLOR_XRGB(255,255,0)},
		{ 1,-1, 1, D3DCOLOR_XRGB(255,0,0)},
		{ 1,-1,-1, D3DCOLOR_XRGB(255,255,0)},
		{-1,-1,-1, D3DCOLOR_XRGB(255,0,0)},
	};

	CUSTOMVERTEX earthVertices[] =
	{
		{-1, 1, 1, 0x008000},
		{ 1, 1, 1, D3DCOLOR_XRGB(0,0,255)},
		{ 1, 1,-1, 0x008000},
		{-1, 1,-1, D3DCOLOR_XRGB(0,0,255)},

		{-1,-1, 1, D3DCOLOR_XRGB(0,0,255)},
		{ 1,-1, 1, 0x008000},
		{ 1,-1,-1, D3DCOLOR_XRGB(0,0,255)},
		{-1,-1,-1, 0x008000},
	};

	CUSTOMVERTEX moonVertices[] =
	{
		{-1, 1, 1, 0x808080},
		{ 1, 1, 1, 0x808080},
		{ 1, 1,-1, 0x808080},
		{-1, 1,-1, 0x808080},

		{-1,-1, 1, 0x808080},
		{ 1,-1, 1, 0x808080},
		{ 1,-1,-1, 0x808080},
		{-1,-1,-1, 0x808080},
	};

	void* pVertices;

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pSunVB, NULL)))
	{
		return E_FAIL;
	}


	if (FAILED(g_pSunVB->Lock(0, sizeof(sunVertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, sunVertices, sizeof(sunVertices));

	g_pSunVB->Unlock();
	////////////////////
	if (FAILED(g_pD3DDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pEarthVB, NULL)))
	{
		return E_FAIL;
	}


	if (FAILED(g_pEarthVB->Lock(0, sizeof(earthVertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, earthVertices, sizeof(earthVertices));

	g_pEarthVB->Unlock();
	/////////////////////////
	if (FAILED(g_pD3DDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pMoonVB, NULL)))
	{
		return E_FAIL;
	}


	if (FAILED(g_pMoonVB->Lock(0, sizeof(moonVertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, moonVertices, sizeof(moonVertices));

	g_pMoonVB->Unlock();

	return S_OK;
}

HRESULT InitIB()
{
	MYINDEX indices[] =
	{
		{0, 1, 2}, {0, 2, 3},
		{4, 6, 5}, {4, 7, 6},
		{0, 3, 7}, {0, 7, 4},
		{1, 5, 6}, {1, 6, 2},
		{3, 2, 6}, {3, 6, 7},
		{0, 4, 5}, {0, 5, 1},
	};

	if (FAILED(g_pD3DDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
	{
		return E_FAIL;
	}

	void** pIndices;

	if (FAILED(g_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pIndices, indices, sizeof(indices));

	g_pIB->Unlock();

	return S_OK;
}

HRESULT InitGeometry()
{
	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;

	return S_OK;
}

void SetupCamera()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 10.0f, -25.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Animate() {

	/*
D3DXMATRIXA16 g_matTMSun;
D3DXMATRIXA16 g_matRSun;

D3DXMATRIXA16 g_matSEarth;
D3DXMATRIXA16 g_matTMEarth;
D3DXMATRIXA16 g_matREarth;
D3DXMATRIXA16 g_matRevEarth;

D3DXMATRIXA16 g_matSMoon;
D3DXMATRIXA16 g_matTMMoon;
D3DXMATRIXA16 g_matRMoon;
D3DXMATRIXA16 g_matRevMoon;
*/
	D3DXVECTOR3 nomalpVec(0, 1, 0);

	D3DXMatrixIdentity(&g_matTMSun);

	D3DXMatrixRotationY(&g_matRSun, GetTickCount() / 5000.0f);



	D3DXMatrixScaling(&g_matSEarth, 0.5f, 0.5f, 0.5f);

	D3DXMatrixRotationAxis(&g_matRevEarth, &nomalpVec, GetTickCount() / 2500.0f);

	D3DXMatrixTranslation(&g_matTMEarth, 3, 0, 0);

	D3DXMatrixRotationY(&g_matREarth, GetTickCount() / 5000.0f);




	D3DXMatrixScaling(&g_matSMoon, 0.5f, 0.5f, 0.5f);

	D3DXMatrixTranslation(&g_matTMMoon, 3, 0, 0);

	D3DXMatrixRotationY(&g_matRMoon, GetTickCount() / 5000.0f);

	D3DXMatrixRotationAxis(&g_matRevMoon, &nomalpVec, GetTickCount() / 2500.0f);

}

void Cleanup()
{
	if (g_pIB != NULL)
		g_pIB->Release();

	if (g_pSunVB != NULL)
		g_pSunVB->Release();

	if (g_pEarthVB != NULL)
		g_pEarthVB->Release();

	if (g_pMoonVB != NULL)
		g_pMoonVB->Release();

	if (g_pD3DDevice != NULL)
		g_pD3DDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

void DrawMesh(D3DXMATRIXA16* pMat, LPDIRECT3DVERTEXBUFFER9* pVB)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pD3DDevice->SetStreamSource(0, *pVB, 0, sizeof(CUSTOMVERTEX));
	g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pD3DDevice->SetIndices(g_pIB);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void Render() {
	D3DXMATRIXA16 matWorld;

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	Animate();

	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {

		matWorld = g_matRSun * g_matTMSun;
		DrawMesh(&matWorld, &g_pSunVB);

		matWorld = g_matSEarth * g_matREarth * g_matTMEarth * g_matRevEarth * matWorld;
		DrawMesh(&matWorld, &g_pEarthVB);

		matWorld = g_matSMoon * g_matRMoon * g_matTMMoon * g_matRevMoon * matWorld;
		DrawMesh(&matWorld, &g_pMoonVB);

		g_pD3DDevice->EndScene();
	}
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

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

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc,
		0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"SolarSystem", NULL };

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"SolarSystem", L"SolarSystem", WS_OVERLAPPEDWINDOW,
		100, 100, 500, 500, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitGeometry()))
		{
			SetupCamera();

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

	UnregisterClass(L"D3D Hierarchy", wc.hInstance);
	return 0;
}