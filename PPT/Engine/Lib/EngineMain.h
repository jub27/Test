#pragma once
#include "GlobalDefine.h"

/**
* @mainpage JEngine Made By JBC
* @details JEngine 은 게임아트학원 교육용으로 만들어진 엔진입니다. \n
	엔진으로의 기능은 미미하지만 게임엔진이라는 것은 이런것이다를 보여주기 위해 제작되었습니다.
*/

/**
* @namespace JEngine
* @brief JEngine 네임 스페이스
* @details 이름 충돌을 피하기 위하여 만든 JEngine의 namespace입니다.
*/
namespace JEngine
{
	/**
	* @brief 엔진의 Main이 존재하는 클레스
	* @details 엔진의 Main문을 관리하는 클레스 윈도우 사이즈 타이틀변경등을 담당한다.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class EngineMain
	{
	private:
		string		m_strTitle;
		int			m_winSizeX;
		int			m_winSizeY;
	public:
		int			StartEngine(HINSTANCE hInstance);
		EngineMain(string title, int cx, int cy);
		~EngineMain();
	};
}

