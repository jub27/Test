/**========================================================================
 * 인덱스 버퍼 생성
 * 인덱스를 보관하기 위한 전용 개체에 대한 공부
 *=========================================================================*/
#pragma once
#include "GameManager.h"

/**========================================================================
 * 전역 변수
 *=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;
GameManager GM;

/**========================================================================
 * Direct3D 초기화
 *=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	// 디바이스를 생성하기 위한 D3D 객체 생성
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	
	d3dpp.EnableAutoDepthStencil = TRUE; // 깊이가 있는 z버퍼가 필요하므로 설정한다.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//Z 버퍼 기능을 켠다.
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

/**========================================================================
 * 행렬 설정 : 행렬에는 3가지가 있으며, 각각 월드, 뷰, 프로젝션 행렬이다.
 *=========================================================================*/
void SetupMatrices()
{
	D3DXMatrixIdentity(GM.GetmatWorld()); //월드 행렬을 단위 행렬로 생성

	//D3DXMatrixRotationY(&matWorld, GetTickCount() / 500.0f); // Y축을 회전축으로 회전 행렬을 생성한다.
	D3DXMatrixTranslation(GM.GetmatWorld(), GM.GetSpeed(), 0.0f, 0.0f);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, GM.GetmatWorld()); // 생성한 회전 행렬을 월드 행렬로 디바이스에 설정한다.

	//뷰 행렬(카메라)을 정의하기 위해서는 3가지 값이 필요하다.
	D3DXVECTOR3 vEyePt(0.0f, 20.0f, -20.0f); //눈의 위치(0.0f, 3.0f, -5.0f)
	D3DXVECTOR3 vLookatPt(GM.GetSpeed(), 0.0f, 0.0f); // 눈이 바라보는 위치(0.0f, 0.0f, 0.0f)
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 윗 방향을 나타내는 상방 벡터(0.0f, 1.0f, 0.0f)

	//뷰 행렬(카메라)
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);// 3가지 Point를 이용해 뷰 행렬을 생성한다.

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); // 생성한 뷰 행렬을 디바이스에 설정한다.

	// 프로젝션 행렬을 정의하기 위해서는 시야각(FOV = Field Of View)과 종횡비(aspect ratio), 클리핑 평면의 값이 필요하다.
	D3DXMATRIXA16 matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj); // 생성한 프로젝션 행렬을 디바이스에 설정.
}

/**========================================================================
 * 초기화 객체들 Release
 *=========================================================================*/
void Cleanup()
{
	GM.Cleanup();
	//해제 순서가 중요 인터페이스 생성의 역순으로 해제하자.
	
	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

/**========================================================================
 * 화면 그리기
 *=========================================================================*/
void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	// 후면 버퍼를 파란색(0, 0, 255)으로 지우고 Z버퍼를 사용하기 위해 초기화 한다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//월드, 뷰, 프로젝션 행렬을 설정한다.
	SetupMatrices();

	// 랜더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// 정점 버퍼의 삼각형을 그린다.

		// 정점 정보가 담겨 있는 정점 버퍼를 출력 스트림으로 할당한다.
		g_pd3dDevice->SetStreamSource(0, GM.GetVertex(0)->GetpVB(), 0, sizeof(CUSTOMVERTEX));

		// D3D에게 정점 셰이더 정보를 지정한다. 대부분의 경우에는 FVF만 지정한다.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// 인덱스 버퍼를 지정한다.
		g_pd3dDevice->SetIndices(GM.GetCube()->GetpIB());

		// 기하 정보를 출력하기 위한 DrawIndexPrimitive() 함수를 호출한다.
		// 첫 번째 : 그리고자 하는 기본 타입형
		// 두 번째 : 여러 오브젝트를 하나로 묶을때 더해질 넘버.
		// 세 번째 : 참조할 최소 인덱스 값
		// 네 번째 : 이번 호출에 참조될 버텍스의 수
		// 다섯 번째 : 인덱스 버퍼 내에서 읽기를 시작할 요소로의 인덱스
		// 여섯 번째 : 그리고자 하는 기본형의 수
		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		g_pd3dDevice->SetStreamSource(0, GM.GetVertex(1)->GetpVB(), 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		//렌더링 종료
		g_pd3dDevice->EndScene();
	}

	// 후면 버퍼를 보이는 화면으로 전환.
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
 * Window 생성
 *=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	//윈도우 클래스 등록
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D IndexBuffer", NULL };

	//winclass 레지스터에 등록
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D IndexBuffer", L"D3D IndexBuffer", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(GM.GetVertex(0)->InitVB(g_pd3dDevice, 0, 0, 0, 1)) && SUCCEEDED(GM.GetVertex(1)->InitVB(g_pd3dDevice, 2, 0, 0, 1)))
		{
			if (SUCCEEDED(GM.GetCube()->InitIB(g_pd3dDevice)))
			{
				ShowWindow(hWnd, SW_SHOWDEFAULT);
				UpdateWindow(hWnd);
				GM.Init(hWnd);

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
						GM.Update();
						Render();
					}
				}
			}
		}
	}

	//등록된 레지스트 winclass 릴리즈.
	UnregisterClass(L"D3D IndexBuffer", wc.hInstance);
	return 0;
}