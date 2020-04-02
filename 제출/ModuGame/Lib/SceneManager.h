#pragma once
#include "GlobalDefine.h"

namespace JEngine
{
	class Scene;
	class InputManager;
	/**
	* @brief 게임에 사용되는 Scene들을 관리하는 클레스
	* @details 게임에 사용되는 Scene들을 관리하는 클레스로 로직적 메인을 담당하는 클레스입니다.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/
	class SceneManager
	{
	private:
		bool					m_bInit;
		HWND					m_hWnd;
		vector<Scene*>			m_vecScene;
		Scene*					m_curScene;
		Scene*					m_lastScene;
		InputManager*			m_input;
		int						m_iWinCX;
		int						m_iWinCY;

		static SceneManager*	s_this;
		SceneManager();
	public:
		static SceneManager*	GetInstance()
		{
			if (s_this == NULL)
			{
				s_this = new SceneManager();
			}

			return s_this;
		}

		void					Init(HWND hWnd, int win_cx, int win_cy);
		void					RegistScene(Scene* Reg);
		bool					Update();
		void					Release();

		void					LoadScene(int SceneIndex);

		~SceneManager();
	};
}
