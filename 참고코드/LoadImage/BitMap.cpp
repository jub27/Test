#include "BitMap.h"

BitMap::BitMap()
{
}

void BitMap::Init(HDC hdc, LPCWSTR FileName)
{
	MemDC = CreateCompatibleDC(hdc);

	//1번째 인수 : NULL고정
	//2번째 인수 : 파일경로(프로젝트 폴더가 디폴트)
	//3번째 인수 : IMAGE_BITMAP, IMAGE_ICON, IMAGE_CURSOR
	//4, 5번째 인수 : 아이콘이나 커서일 경우 너비와 높이, 비트맵이면 0, 0
	//6번째 인수 : 여러가지 플레그 옵션으로 그리기 모드를 바꿀 수 있다.
	//파일 그대로 표현하는 옵션 - LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE 고정
	m_BitMap = (HBITMAP)LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0
					, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	m_OldBitMap = (HBITMAP)SelectObject(MemDC, m_BitMap);
	BITMAP BitMap_Info;
	GetObject(m_BitMap, sizeof(BitMap_Info), &BitMap_Info);
	m_size.cx = BitMap_Info.bmWidth;
	m_size.cy = BitMap_Info.bmHeight;
}
void BitMap::Draw(HDC hdc, int x, int y, float spX, float spY)
{
	StretchBlt(hdc, x, y, m_size.cx * spX, m_size.cy * spY, MemDC, 0, 0, m_size.cx, m_size.cy, SRCCOPY);
}

BitMap::~BitMap()
{
	SelectObject(MemDC, m_OldBitMap);
	DeleteObject(m_BitMap);
	DeleteDC(MemDC);
}
