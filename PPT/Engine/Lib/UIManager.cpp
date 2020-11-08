#include "UIManager.h"
#include "ResoucesManager.h"
#include "Button.h"
#include "Label.h"

namespace JEngine
{

	UIManager::UIManager()
	{
		SetBkMode(JEngine::ResoucesManager::GetInstance()->GetBackDC(), TRANSPARENT);
	}


	UIManager::~UIManager()
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			auto del = (*iter);
			SAFE_DELETE(del);
		}
	}

	void UIManager::AddButton(int x, int y, string btnImg, std::function<bool()> callback)
	{
		Button *pNew = new Button();
		pNew->Init(x, y, btnImg, callback);
		m_vecUI.push_back(pNew);
	}

	void UIManager::AddLabel(string str, int x, int y, DWORD drawOption)
	{
		Label *pNew = new Label();
		pNew->Init(str , x, y, drawOption);
		m_vecUI.push_back(pNew);
	}

	void UIManager::Update()
	{
		int i = m_vecUI.size();

		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			if ((*iter)->Update())
				return;
		}
	}

	void UIManager::Draw()
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			(*iter)->Draw();
		}
	}

	void UIManager::RelaaseUI()
	{

		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			SAFE_DELETE((*iter));
		}
		m_vecUI.clear();
	}
}
