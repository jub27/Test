#pragma once
#include "SingleTon.h"
#include <Windows.h>

namespace JEngine
{
	/**
	* @brief 게임에 사용되는 시간과 FPS를 담당하는 클레스
	* @details scene의 Input Update 에서의 인자로 넘어가는 흘러간시간을 관리하는 클레스 FPS조절 기능도 가지고 있습니다.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class TimeManager : public SingleTon<TimeManager>
	{
	private:
		bool				m_bUseFPS;
		int					m_nFPS;
		DWORD				m_dwLastTime;
		DWORD				m_dwCurTime;
		float				m_fElapseTime;
	public:
		void				init(bool useFPS, int FPS);
		bool				Update();
		inline float		GetElipseTime()
		{
			return m_fElapseTime;
		}
		TimeManager();
		~TimeManager();
	};
}

