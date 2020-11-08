#pragma once

template < typename T >
class SingleTon
{
private:
	static T * m_pThis;
protected:
	SingleTon()
	{

	}

	virtual ~SingleTon()
	{

	}

public:
	static T * GetInstance()
	{
		if (m_pThis == nullptr)
			m_pThis = new T;
		return m_pThis;
	};

	static void DestroyInstance()
	{
		if (m_pThis)
		{
			delete m_pThis;
			m_pThis = nullptr;
		}
	};
};

template <typename T> T * SingleTon<T>::m_pThis = 0;

