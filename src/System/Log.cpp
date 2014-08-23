#include <FRONTIER/System/Log.hpp>
#include <ostream>
namespace fm
{
	/// Constructors //////////////////////////////////////////////////////////
	Log::Log() : m_os(NULL)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Log::Log(std::ostream &os) : m_os(&os)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Log::Log(std::ostream *os) : m_os(os)
	{
		
	}
	
	/// Operators //////////////////////////////////////////////////////////
	Log::reference Log::operator=(std::ostream &os)
	{
		m_os = &os;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator=(std::ostream *os)
	{
		m_os = os;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(std::ostream &(*func)(std::ostream &))
	{
		if (m_os)
			(*m_os)<<func;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(std::basic_ios<char, std::char_traits<char> > &(*func)(std::basic_ios<char, std::char_traits<char> > &))
	{
		if (m_os)
			(*m_os)<<func;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(std::ios_base &(*func)(std::ios_base &))
	{
		if (m_os)
			(*m_os)<<func;
		return *this;
	}
}
