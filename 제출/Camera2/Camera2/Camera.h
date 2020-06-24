#pragma once
#ifndef _ZCAMERA_H_
#define _ZCAMERA_H_

#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
private:
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 m_vUp;

	D3DXVECTOR3 m_vView;
	D3DXVECTOR3 m_vCross;

	D3DXMATRIXA16 m_matView;
	D3DXMATRIXA16 m_matBill;
	D3DXMATRIXA16* m_matBlock;
public:
	Camera();
	D3DXMATRIXA16* GetViewMatrix() { return &m_matView; }
	D3DXMATRIXA16* GetBillMatrix() { return &m_matBill; }
	void InitBlock(D3DXMATRIXA16* matBlock);
	D3DXMATRIXA16* SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookAt, D3DXVECTOR3* pvUp);
	void SetEye(D3DXVECTOR3* pv) { m_vEye = *pv; }
	D3DXVECTOR3* GetEye() { return &m_vEye; }
	void SetLookAt(D3DXVECTOR3* pv) { m_vLookAt = *pv; }
	D3DXVECTOR3* GetLookat() { return &m_vLookAt; }
	void SetUp(D3DXVECTOR3* pv) { m_vUp = *pv; }
	D3DXVECTOR3* GetUp() { return &m_vUp; }
	void Flush() { SetView(&m_vEye, &m_vLookAt, &m_vUp); }
	D3DXMATRIXA16* RotateLocalX(float angle);
	D3DXMATRIXA16* RotateLocalY(float angle);
	D3DXMATRIXA16* RotateLocalZ(float angle);
	D3DXMATRIXA16* MoveTo(D3DXVECTOR3* pv);
	D3DXMATRIXA16* MoveLocalX(float dist);
	D3DXMATRIXA16* MoveLocalY(float dist);
	D3DXMATRIXA16* MoveLocalZ(float dist);
};

#endif