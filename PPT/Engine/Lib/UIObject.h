#pragma once
#include "POINT.h"

namespace JEngine
{
	class UIObject
	{
	protected:
		JEngine::RECT			m_rcPos;

		void					SetPos(int left, int top, int right, int bottom);
	public:
		virtual void			Draw() = 0;
		virtual bool			Update() = 0;

		UIObject();
		virtual ~UIObject();
	};
}

