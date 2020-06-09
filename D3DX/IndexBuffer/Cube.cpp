#include "Cube.h"

Cube::Cube()
{
}


Cube::~Cube()
{
}

HRESULT Cube::InitIB(LPDIRECT3DDEVICE9 & pDevice)
{
	MYINDEX indices[] =
	{
		{0, 1, 2}, {0, 2, 3}, // ����
		{4, 5, 6}, {4, 7, 6}, // �Ʒ���
		{0, 3, 7}, {0, 7, 4}, // �޸�
		{1, 5, 6}, {1, 6, 2}, // ������
		{3, 2, 6}, {3, 6, 7}, // �ո�
		{0, 4, 5}, {0, 5, 1}, // �޸�
	};

	//�ε��� ���� ����
	//D3DFMT_INDEX16�� �ε����� ������ 16��Ʈ��� ��.
	//MYINDEX ����ü���� WORD������ �������Ƿ� D3DFMT_INDEX16�� ����Ѵ�.
	if (FAILED(pDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
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