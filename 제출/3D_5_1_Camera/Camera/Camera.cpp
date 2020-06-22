#include "Camera.h"

Camera::Camera() {
	D3DXVECTOR3 eye(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lookAt(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matBill);
	SetView(&eye, &lookAt, &up);
}

D3DXMATRIXA16* Camera::SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookAt, D3DXVECTOR3* pvUp) {

	m_vEye = *pvEye;
	m_vLookAt = *pvLookAt;
	m_vUp = *pvUp;
	D3DXVec3Normalize(&m_vView, &(m_vLookAt - m_vEye));
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookAt, &m_vUp);
	D3DXMatrixInverse(&m_matBill, NULL, &m_matView);
	m_matBill._41 = 0.0f;
	m_matBill._42 = 0.0f;
	m_matBill._43 = 0.0f;

	return &m_matView;
}

D3DXMATRIXA16* Camera::RotateLocalX(float angle) {
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vCross, angle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);
	vNewDst += m_vEye;

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16* Camera::RotateLocalY(float angle) {
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vUp, angle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);
	vNewDst += m_vEye;

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}


D3DXMATRIXA16* Camera::MoveLocalX(float dist) {
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookAt;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vCross);

	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}


D3DXMATRIXA16* Camera::MoveLocalY(float dist) {
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookAt;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vUp);

	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16* Camera::MoveLocalZ(float dist) {
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookAt;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vView);

	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16* Camera::MoveTo(D3DXVECTOR3* pv) {
	D3DXVECTOR3 dv = *pv - m_vEye;
	m_vEye = *pv;
	m_vLookAt += dv;
	return SetView(&m_vEye, &m_vLookAt, &m_vUp);
}