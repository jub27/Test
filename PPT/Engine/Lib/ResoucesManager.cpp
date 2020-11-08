#include "ResoucesManager.h"
#include "GlobalDefine.h"

namespace JEngine
{
	ResoucesManager::ResoucesManager()
	{
	}


	ResoucesManager::~ResoucesManager()
	{
		SAFE_DELETE(m_pBack);

		for (auto iter = m_mapBitmap.begin(); iter != m_mapBitmap.end(); iter++)
		{
			auto del = (*iter).second;
			SAFE_DELETE(del);
		}
	}

	void ResoucesManager::initBack(HDC hdc , int cx, int cy)
	{
		m_pBack = new JEngine::BitMap();
		m_pBack->initBack(hdc , cx , cy);
	}


	JEngine::BitMap* ResoucesManager::initBitmap(std::string strFileName)
	{
		auto iter = m_mapBitmap.find(strFileName);
		if (iter == m_mapBitmap.end())
		{
			JEngine::BitMap* pBitmap = new JEngine::BitMap();
			pBitmap->init(m_pBack->GetDC(), strFileName);

			m_mapBitmap.insert(make_pair(strFileName, pBitmap));
			return pBitmap;
		}
		else
			return iter->second;

	}

	HDC ResoucesManager::GetBackDC()
	{
		return m_pBack->GetDC();
	}

	void ResoucesManager::DrawBack(HDC hdc)
	{
		m_pBack->DrawBack(hdc);
	}

	JEngine::BitMap* ResoucesManager::GetBitmap(std::string strFileName)
	{
		auto iter = m_mapBitmap.find(strFileName);
		if (iter != m_mapBitmap.end())
		{
			return iter->second;
		}
		else
		{
			return initBitmap(strFileName);
		}
	}
}

