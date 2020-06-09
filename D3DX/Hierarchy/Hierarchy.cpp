/**========================================================================
 * 계층 구조
 * 계층 구조를 이해하고 계층간의 행렬 실행 순서를 공부한다.
 *=========================================================================*/

#include <d3d9.h>
#include <d3dx9.h>

 /**========================================================================
  * 전역 변수
  *=========================================================================*/

LPDIRECT3D9				g_pD3D			= NULL;
LPDIRECT3DDEVICE9		g_pd3dDevice	= NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVB			= NULL;
LPDIRECT3DINDEXBUFFER9	g_pIB = NULL;

D3DXMATRIXA16			g_matTMAncestor; //조상의 변환 행렬
D3DXMATRIXA16			g_matRAncestor; //조상의 회전 행렬

D3DXMATRIXA16			g_matTMParent; //조상의 변환 행렬
D3DXMATRIXA16			g_matRParent; //조상의 회전 행렬
D3DXMATRIXA16			g_matRevParent; //조상의 회전 행렬

D3DXMATRIXA16			g_matSChild; //자식의 스케일 행렬
D3DXMATRIXA16			g_matTMChild; //자식의 변환 행렬
D3DXMATRIXA16			g_matRChild; //자식의 회전 행렬
D3DXMATRIXA16			g_matRevChild; //자식의 공전 행렬

struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct MYINDEX
{
	WORD _0, _1, _2;
};

/**========================================================================
 * Direct3D 초기화
 *=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
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
									&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

/**========================================================================
 * 정점 버퍼
 * 정점 버퍼를 생성하고 정점값을 채워넣는다. 
 *=========================================================================*/

HRESULT InitVB()
{
	CUSTOMVERTEX vertices[] =
	{
		{-1, 1, 1, 0xffff0000},
		{ 1, 1, 1, 0xff00ff00},
		{ 1, 1,-1, 0xff0000ff},
		{-1, 1,-1, 0xffffff00},

		{-1,-1, 1, 0xffff0000},
		{ 1,-1, 1, 0xffff0000},
		{ 1,-1,-1, 0xffff0000},
		{-1,-1,-1, 0xffff0000},
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
												D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	void* pVertices;

	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

/**========================================================================
 * 인덱스 버퍼
 * 인덱스 버퍼를 생성하고 정점값을 채워넣는다.
 *=========================================================================*/

HRESULT InitIB()
{
	MYINDEX indices[]=
	{
		{0, 1, 2}, {0, 2, 3},
		{4, 6, 5}, {4, 7, 6},
		{0, 3, 7}, {0, 7, 4},
		{1, 5, 6}, {1, 6, 2},
		{3, 2, 6}, {3, 6, 7},
		{0, 4, 5}, {0, 5, 1},
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
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

/**========================================================================
 * 기하 정보 초기화
 *=========================================================================*/

HRESULT InitGeometry()
{
	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;

	return S_OK;
}

/**========================================================================
 * 카메라 행렬 설정
 *=========================================================================*/
void SetupCamera()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 10.0f, -25.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

/**========================================================================
 * 애니메이션 행렬 생성
 *=========================================================================*/
void Animate()
{
	//조상의 메시는 원점에 있으므로 TM은 단위 행렬
	D3DXMatrixIdentity(&g_matTMAncestor);

	//조상 메시의 Y축 회전 행렬
	D3DXMatrixRotationY(&g_matRAncestor, GetTickCount() / 5000.0f);

	//부모 메시의 z축 회전 행렬
	D3DXMatrixRotationZ(&g_matRParent, GetTickCount() / 10000.0f);

	D3DXVECTOR3 nomalPVec(-1, 1, 0);

	//특정 vector를 축으로 하는 행렬
	D3DXMatrixRotationAxis(&g_matRevParent, &nomalPVec, GetTickCount() / 2500.0f);

	//부모 메시는 원점으로 부터 (3, 3, 3)의 거리에 있다.
	D3DXMatrixTranslation(&g_matTMParent, 3, 3, 3);

	//자식의 스케일 조절
	D3DXMatrixScaling(&g_matSChild, 0.5f, 0.5f, 0.5f);

	//자식 메시의 z축 회전 행렬
	D3DXMatrixRotationZ(&g_matRChild, GetTickCount() / 7000.0f);

	D3DXVECTOR3 nomalCVec(-1, 1, 0);

	//특정 vector를 축으로 하는 행렬
	D3DXMatrixRotationAxis(&g_matRevChild, &nomalCVec, GetTickCount() / 500.0f);

	//자식 메시는 부모로 부터 (1.5, 1.5, 1.5)의 거리에 있다.
	D3DXMatrixTranslation(&g_matTMChild, 1.5f, 1.5f, 1.5f);
}

/**========================================================================
 * Release - 초기화 객체들 소거
 *=========================================================================*/
void Cleanup()
{
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
 * 메시 그리기
 *=========================================================================*/
void DrawMesh(D3DXMATRIXA16* pMat)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

/**========================================================================
 * 화면 그리기
 *=========================================================================*/
void Render()
{
	D3DXMATRIXA16 matWorld;

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	Animate();

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//그리는 순서가 아닌 행렬 적용 순서가 중요하다.

		//부모의 변환을 만든다 - 회전 * 기본 변환
		//matWorld = g_matRAncestor * g_matTMAncestor;
		matWorld = g_matTMAncestor;

		//적용된 변환을 기반으로 부모 객체를 그린다.
		DrawMesh(&matWorld);

		//자식의 변환을 만든다 - 자신의 회전 * 자신의 기본 변환 * 부모의 회전 * 부모의 기본변환
		//스케일 * 자전 * 이동변환 * 공전 * 부모
		//matWorld = g_matRParent * g_matTMParent * g_matRAncestor * g_matTMAncestor;
		//matWorld = g_matRParent * g_matTMParent * matWorld;
		matWorld = g_matRParent * g_matTMParent * g_matRevParent * matWorld;
		//matWorld = g_matTMParent * matWorld;

		//적용된 변환을 기반으로 자식 객체를 그린다.
		DrawMesh(&matWorld);

		matWorld = g_matSChild * g_matRChild * g_matTMChild * g_matRevChild * matWorld;

		//적용된 변환을 기반으로 자식 객체를 그린다.
		DrawMesh(&matWorld);

		matWorld = g_matSChild * g_matRChild * g_matTMChild * g_matRevChild * matWorld;

		//적용된 변환을 기반으로 자식 객체를 그린다.
		DrawMesh(&matWorld);

		g_pd3dDevice->EndScene();
	}
	
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

/**========================================================================
 * 윈도우 프로시저
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
 * WinMain
 *=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc,
		0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Hierarchy", NULL};

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D Hierarchy", L"D3D Hierarchy", WS_OVERLAPPEDWINDOW,
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