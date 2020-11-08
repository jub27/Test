#include "BitMap.h"
#include "ResoucesManager.h"

namespace JEngine
{
	BitMap::BitMap()
	{
		m_eAnchorType = ANCHOR_LT;
	}


	BitMap::~BitMap()
	{
		SelectObject(m_hMemDC, m_hOldBitmap);
		DeleteObject(m_hBitMap);
		DeleteDC(m_hMemDC);
	}

	void BitMap::initBack(HDC hdc, int x, int y)
	{
		m_hMemDC = CreateCompatibleDC(hdc);
		m_hBitMap = CreateCompatibleBitmap(hdc, x, y);
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

		m_size.cx = x;
		m_size.cy = y;
	}

	void BitMap::init(HDC hdc, string file_name)
	{
		m_hMemDC = CreateCompatibleDC(hdc);
		m_hBitMap = (HBITMAP)LoadImage(NULL, file_name.c_str(), IMAGE_BITMAP, 0, 0
			, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

		if (m_hBitMap == NULL)
		{
			MessageBox(NULL, file_name.c_str(), "File Not Find", MB_OK);
			return;
		}

		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);


		BITMAP bmp_info;
		GetObject(m_hBitMap, sizeof(bmp_info), &bmp_info);
		m_size.cx = (int)bmp_info.bmWidth;
		m_size.cy = (int)bmp_info.bmHeight;
	}

	void BitMap::AdjustAnchorPoint(int& x, int& y)
	{
		x -= m_size.cx * m_ptAnchor.x;
		y -= m_size.cy * m_ptAnchor.y;
	}

	void BitMap::DrawBitblt(int x, int y)
	{
		AdjustAnchorPoint(x, y);
		BitBlt(ResoucesManager::GetInstance()->GetBackDC(), x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
	}

	void BitMap::Draw(int x, int y)
	{
		AdjustAnchorPoint(x, y);
		TransparentBlt(ResoucesManager::GetInstance()->GetBackDC(), x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
	}

	void BitMap::DrawBack(HDC hdc)
	{
		BitBlt(hdc, 0, 0, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);

	}

	void BitMap::SetAnchor(ANCHOR type)
	{
		m_eAnchorType = type;
		switch (m_eAnchorType)
		{
		case ANCHOR_LT:
			m_ptAnchor.x = 0.0f;
			m_ptAnchor.y = 0.0f;
			break;
		case ANCHOR_TOP:
			m_ptAnchor.x = 0.5f;
			m_ptAnchor.y = 0.0f;
			break;
		case ANCHOR_RT:
			m_ptAnchor.x = 1.0f;
			m_ptAnchor.y = 0.0f;
			break;
		case ANCHOR_LEFT:
			m_ptAnchor.x = 0.0f;
			m_ptAnchor.y = 0.5f;
			break;
		case ANCHOR_CENTER:
			m_ptAnchor.x = 0.5f;
			m_ptAnchor.y = 0.5f;
			break;
		case ANCHOR_RIGHT:
			m_ptAnchor.x = 1.0f;
			m_ptAnchor.y = 0.5f;
			break;
		case ANCHOR_LB:
			m_ptAnchor.x = 0.0f;
			m_ptAnchor.y = 1.0f;
			break;
		case ANCHOR_BOTTOM:
			m_ptAnchor.x = 0.5f;
			m_ptAnchor.y = 1.0f;
			break;
		case ANCHOR_RB:
			m_ptAnchor.x = 1.0f;
			m_ptAnchor.y = 1.0f;
			break;
		}
	}

	void BitMap::SetAnchorPoint(JEngine::POINTF pt)
	{
		m_eAnchorType = ANCHOR_CUSTOM;
		m_ptAnchor = pt;
	}

	HDC BitMap::GetDC()
	{
		return m_hMemDC;
	}

	void BitMap::Draw(JEngine::POINT pt)
	{
		Draw(pt.x, pt.y);
	}
}