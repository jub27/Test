#pragma once
#include "GlobalDefine.h"
#include "SingleTon.h"
#include "UIObject.h"

namespace JEngine
{
	class UIManager : public SingleTon<UIManager>
	{
	private:
		vector<JEngine::UIObject*>		m_vecUI;
	public:
		void AddButton(int x, int y, string btnImg, std::function<bool()> callback);
		void AddLabel(string str, int x, int y, DWORD drawOption);

		void Update();
		void Draw();

		void RelaaseUI();

		UIManager();
		~UIManager();
	};
}



