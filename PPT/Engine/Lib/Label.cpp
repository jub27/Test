#include "Label.h"
#include "ResoucesManager.h"

namespace JEngine
{
	Label::Label()
	{
	}


	Label::~Label()
	{
	}

	void Label::Init(string str , int x, int y , DWORD drawOption)
	{
		::SIZE size;
		GetTextExtentPoint32(ResoucesManager::GetInstance()->GetBackDC(), str.c_str(), str.length(), &size);
		SetPos(x , y , x + size.cx , y + size.cy);

		m_strPrint = str;
		m_dwDrawOption = drawOption;
	}

	void Label::Draw()
	{
		
		DrawText(ResoucesManager::GetInstance()->GetBackDC(), m_strPrint.c_str(), m_strPrint.length(),
			&m_rcPos.ToWinRECT(), m_dwDrawOption);
	}
}