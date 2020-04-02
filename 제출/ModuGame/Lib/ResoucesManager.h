#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include "SingleTon.h"
#include "BitMap.h"
using namespace std;

namespace JEngine
{
	/**
	* @brief JEngine::BitMap클레스를 관리하는 클레스
	* @details JEngine::BitMap클레스를 관리하는 클레스 , 같은 JEngine::BitMap 객체가 생성되는 것을 막아주고 파일로드를 담당한다.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class ResoucesManager : public SingleTon<ResoucesManager>
	{
	private:
		JEngine::BitMap*				m_pBack;
		map<string , JEngine::BitMap*>	m_mapBitmap;
		JEngine::BitMap*				initBitmap(std::string strFileName);
	public:
		void							initBack(HDC hdc, int cx, int cy);
		HDC								GetBackDC();
		void							DrawBack(HDC hdc);
		JEngine::BitMap*				GetBitmap(std::string strFileName);

		ResoucesManager();
		~ResoucesManager();
	};
}


