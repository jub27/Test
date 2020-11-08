#pragma once
#include "GlobalDefine.h"

namespace JEngine
{
	/**
	* @brief 게임에 사용되는 Scene들의 부모 클레스
	* @details 게임에 사용되는 Scene들의 부모 클레스.Scene구현해야 하는 항목을 순수 가상 함수로 가지고 있다.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class Scene
	{
	public:
		virtual void Init(HWND hWnd) = 0;
		virtual bool Input(float fETime) = 0;
		virtual void Update(float fETime) = 0;
		virtual void Draw(HDC hdc) = 0;
		virtual void Release() = 0;

		Scene();
		virtual ~Scene();
	};
}

