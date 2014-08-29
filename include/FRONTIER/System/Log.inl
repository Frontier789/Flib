#ifndef FRONTIER_LOG_INL_INCLUDED
#define FRONTIER_LOG_INL_INCLUDED
#include <sstream>
namespace fm
{
	////////////////////////////////////////////////////////////
	template<class T>
	inline Log::reference Log::operator<<(const T &t)
	{
		if (m_os)
		{
			if (m_newLog)
				(*m_os)<<m_name<<' ',
				m_newLog = false;
				
			if (m_canRecallLog)
			{
				std::stringstream ss; 
				ss<<m_lastLog<<t;
				m_lastLog = ss.str();
			}
			
			(*m_os)<<t;
		}
		return *this;
	}
}
#endif