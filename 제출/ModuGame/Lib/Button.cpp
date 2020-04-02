#include "Button.h"
#include "ResoucesManager.h"
#include "InputManager.h"

namespace JEngine
{
	Button::Button()
	{
		m_anchor = ANCHOR_LT;
	}

	Button::~Button()
	{

	}

	bool Button::Update()
	{
		if (InputManager::GetInstance()->isKeyUp(VK_LBUTTON))
		{
			if (m_rcCol.isPtin(InputManager::GetInstance()->GetMousePoint()))
				return m_pCallBack();
		}

		return false;
	}

	void Button::Init(int x, int y, string btnImg, pBoolFunc callback)
	{
		m_pDefault = ResoucesManager::GetInstance()->GetBitmap(btnImg);
		m_pCallBack = callback;

		m_ptDraw.x = x;
		m_ptDraw.y = y;

		m_rcCol.left = x;
		m_rcCol.top = y;
		m_rcCol.right = m_rcCol.left + m_pDefault->GetWidth();
		m_rcCol.bottom = m_rcCol.top + m_pDefault->GetHeight();
	}

	void Button::Draw()
	{
		m_pDefault->Draw(m_ptDraw);
	}

}


