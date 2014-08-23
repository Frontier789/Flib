#ifndef FRONTIER_LOG_INL_INCLUDED
#define FRONTIER_LOG_INL_INCLUDED
#include <ostream>
namespace fm
{
	////////////////////////////////////////////////////////////
	template<class T>
	inline Log::reference Log::operator<<(const T &t)
	{
		if (m_os)
			(*m_os)<<t;
		return *this;
	}
}
#endif