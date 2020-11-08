#include "UIObject.h"


namespace JEngine
{
	UIObject::UIObject()
	{
	}


	UIObject::~UIObject()
	{
	}

	void UIObject::SetPos(int left, int top, int right, int bottom)
	{
		m_rcPos.Set(left, top, right, bottom);
	}
}