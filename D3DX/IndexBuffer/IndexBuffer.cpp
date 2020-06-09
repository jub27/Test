/**========================================================================
 * �ε��� ���� ����
 * �ε����� �����ϱ� ���� ���� ��ü�� ���� ����
 *=========================================================================*/
#pragma once
#include "GameManager.h"

/**========================================================================
 * ���� ����
 *=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;
GameManager GM;

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
 * ��� ���� : ��Ŀ��� 3������ ������, ���� ����, ��, �������� ����̴�.
 *=========================================================================*/
void SetupMatrices()
{
	D3DXMatrixIdentity(GM.GetmatWorld()); //���� ����� ���� ��ķ� ����

	//D3DXMatrixRotationY(&matWorld, GetTickCount() / 500.0f); // Y���� ȸ�������� ȸ�� ����� �����Ѵ�.
	D3DXMatrixTranslation(GM.GetmatWorld(), GM.GetSpeed(), 0.0f, 0.0f);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, GM.GetmatWorld()); // ������ ȸ�� ����� ���� ��ķ� ����̽��� �����Ѵ�.

	//�� ���(ī�޶�)�� �����ϱ� ���ؼ��� 3���� ���� �ʿ��ϴ�.
	D3DXVECTOR3 vEyePt(0.0f, 20.0f, -20.0f); //���� ��ġ(0.0f, 3.0f, -5.0f)
	D3DXVECTOR3 vLookatPt(GM.GetSpeed(), 0.0f, 0.0f); // ���� �ٶ󺸴� ��ġ(0.0f, 0.0f, 0.0f)
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
	GM.Cleanup();
	//���� ������ �߿� �������̽� ������ �������� ��������.
	
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
		g_pd3dDevice->SetStreamSource(0, GM.GetVertex(0)->GetpVB(), 0, sizeof(CUSTOMVERTEX));

		// D3D���� ���� ���̴� ������ �����Ѵ�. ��κ��� ��쿡�� FVF�� �����Ѵ�.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// �ε��� ���۸� �����Ѵ�.
		g_pd3dDevice->SetIndices(GM.GetCube()->GetpIB());

		// ���� ������ ����ϱ� ���� DrawIndexPrimitive() �Լ��� ȣ���Ѵ�.
		// ù ��° : �׸����� �ϴ� �⺻ Ÿ����
		// �� ��° : ���� ������Ʈ�� �ϳ��� ������ ������ �ѹ�.
		// �� ��° : ������ �ּ� �ε��� ��
		// �� ��° : �̹� ȣ�⿡ ������ ���ؽ��� ��
		// �ټ� ��° : �ε��� ���� ������ �б⸦ ������ ��ҷ��� �ε���
		// ���� ��° : �׸����� �ϴ� �⺻���� ��
		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		g_pd3dDevice->SetStreamSource(0, GM.GetVertex(1)->GetpVB(), 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

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

	//��ϵ� ������Ʈ winclass ������.
	UnregisterClass(L"D3D IndexBuffer", wc.hInstance);
	return 0;
}