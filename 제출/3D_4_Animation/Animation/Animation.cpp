/**========================================================================
 * KeyFrameAnimation
 * 선형 보간을 이용한 애니메이션 처리
 *=========================================================================*/

#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include<cmath>

 /**========================================================================
  * 전역 변수
  *=========================================================================*/

LPDIRECT3D9				g_pD3D = NULL;
LPDIRECT3DDEVICE9		g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVB = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIB = NULL;

D3DXMATRIXA16			g_matTMParent; //부모의 기본 행렬
D3DXMATRIXA16			g_matRParent; //부모의 회전 행렬

D3DXMATRIXA16			g_matTMChild; //자식의 기본 행렬
D3DXMATRIXA16			g_matRChild; //자식의 회전 행렬

float					g_fRotHorizon = 0.0f;
float					g_fRotVertical = 0.0f;
float					g_fPosX = 0.0f;
float					g_fPosY = 0.0f;

bool					g_bStart = false;

LPD3DXFONT				g_Font;
RECT					g_Rect;

struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

#define ROT_DELTA 0.1f;
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct MYINDEX
{
	WORD _0, _1, _2;
};

D3DXVECTOR3			g_aniPos[2];
D3DXQUATERNION		g_aniRot[2];

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
	MYINDEX indices[] =
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
 * 애니메이션 초기화
 *=========================================================================*/
void SetAniRot(int index, FLOAT Yaw, FLOAT Pitch, FLOAT Roll) {
	D3DXQuaternionRotationYawPitchRoll(&g_aniRot[index], Yaw, Pitch, Roll);
}

void SetAniPos(int index, FLOAT x, FLOAT y, FLOAT z) {
	g_aniPos[index] = D3DXVECTOR3(x, y, z);
}

void InitAnimation()
{
	SetAniPos(0, 0, 0, 0); // 위치 변화에 사용 할 벡터 값
	SetAniPos(0, 0, 0, 0); // 위치 변화에 사용 할 벡터 값

	FLOAT Yaw = 0; // Y축 90도 회전
	FLOAT Pitch = 0;
	FLOAT Roll = 0;

	SetAniRot(0, Yaw, Pitch, Roll); // 사원수(Y축 90도)

	Yaw = 0;
	Pitch = 0; // X축 90도 회전
	Roll = 0;
	SetAniRot(1, Yaw, Pitch, Roll); // 사원수(X축 90도)
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

	InitAnimation();

	D3DXCreateFont(g_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_Font);

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

	D3DXVECTOR3 vEyePt(0.0f, 10.0f, -20.0f);
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
	static float t = 0;
	static float add = 0.005;
	float x, y, z;
	D3DXQUATERNION quat;

	D3DXVECTOR3 v;
	//이동값의 선형 보간
	D3DXVec3Lerp(&v, &g_aniPos[0], &g_aniPos[1], t);
	//이동 행렬값으로 변환
	D3DXMatrixTranslation(&g_matTMParent, v.x, v.y, v.z);

	//회전값의 구면 선형 보간
	D3DXQuaternionSlerp(&quat, &g_aniRot[0], &g_aniRot[1], t);
	//사원수를 회전 행렬값으로 변환
	D3DXMatrixRotationQuaternion(&g_matRParent, &quat);

	if (t < 0.0f || t > 1.0f)
		add *= -1;

	t += add;

	//자식 메시의 z축 회전 행렬
	D3DXMatrixRotationZ(&g_matRChild, GetTickCount() / 500.0f);

	//자식 메시는 원점으로 부터 (3, 3, 3)의 거리에 있다.
	D3DXMatrixTranslation(&g_matTMChild, 3, 0, 0);
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
	if (!g_bStart)
		return;
	D3DXMATRIXA16 matWorld;

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	g_Font->DrawText(NULL, "방향키 = 각도, WASD = 이동, space = 정지", -1, &g_Rect, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	Animate();

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//그리는 순서가 아닌 행렬 적용 순서가 중요하다.

		//부모의 변환을 만든다 - 회전 * 기본 변환
		matWorld = g_matRParent * g_matTMParent;

		//적용된 변환을 기반으로 부모 객체를 그린다.
		DrawMesh(&matWorld);

		//자식의 변환을 만든다 - 자신의 회전 * 자신의 기본 변환 * 부모의 회전 * 부모의 기본변환
		matWorld = g_matRChild * g_matTMChild * matWorld;
		//matWorld = g_matRChild * g_matTMChild * matWorld;

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
	case WM_KEYDOWN:

		if (wParam == VK_SPACE)
			g_bStart = !g_bStart;

		if (!g_bStart)
			break;

		if (wParam == 'W')
			g_fPosY += ROT_DELTA;
		if (wParam == 'A')
			g_fPosX -= ROT_DELTA;
		if (wParam == 'S')
			g_fPosY -= ROT_DELTA;
		if (wParam == 'D')
			g_fPosX += ROT_DELTA;

		if (wParam == VK_LEFT)
			g_fRotHorizon -= ROT_DELTA;

		if (wParam == VK_UP)
			g_fRotVertical += ROT_DELTA;

		if (wParam == VK_RIGHT)
			g_fRotHorizon += ROT_DELTA;

		if (wParam == VK_DOWN)
			g_fRotVertical -= ROT_DELTA;

		SetAniRot(1, g_fRotHorizon, g_fRotVertical, 0);
		SetAniPos(1, g_fPosX, g_fPosY, 0);

		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**========================================================================
 * WinMain
 *=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc,
		0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Animation", NULL };

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3D Animation", "D3D Animation", WS_OVERLAPPEDWINDOW,
		100, 100, 500, 500, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	g_bStart = true;

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

	UnregisterClass("D3D Animation", wc.hInstance);
	return 0;
}