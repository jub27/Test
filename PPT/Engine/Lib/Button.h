#pragma once
#include "GlobalDefine.h"
#include "UIObject.h"
#include "BitMap.h"

typedef std::function<bool()> pBoolFunc;
namespace JEngine
{
	class Button : public JEngine::UIObject
	{
	private:
		ANCHOR					m_anchor;
		POINT					m_ptDraw;
		RECT					m_rcCol;
		BitMap*					m_pDefault;
		pBoolFunc				m_pCallBack;
	public:
		void			Init(int x , int y , string btnImg , pBoolFunc callback);
		virtual bool	Update();
		virtual void	Draw();

		Button();
		virtual ~Button();
	};

}


