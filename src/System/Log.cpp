#include <FRONTIER/System/Log.hpp>
#include <ostream>
namespace fm
{
	/// Constructors //////////////////////////////////////////////////////////
	Log::Log() : m_newLog(true),
				 m_promptName(false),
				 m_os(NULL)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Log::Log(std::ostream &os) : m_newLog(true),
								 m_promptName(false),
								 m_os(&os)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Log::Log(std::ostream *os) : m_newLog(true),
								 m_promptName(false), 
								 m_os(os)
	{
		 
	}
	
	/// Operators //////////////////////////////////////////////////////////
	Log::reference Log::operator=(std::ostream &os)
	{
		m_newLog  = true;
		m_lastLog = std::string();
		m_os = &os;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator=(std::ostream *os)
	{
		m_newLog  = true;
		m_lastLog = std::string();
		m_os = os;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(std::ostream &(*func)(std::ostream &))
	{
		if (func==(std::ostream &(*)(std::ostream &))std::endl)
			m_newLog  = true;
		
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
	
	/////////////////////////////////////////////////////////////
	bool Log::hasLog() const
	{
		return m_lastLog.length()!=0;
	}
	
	/////////////////////////////////////////////////////////////
	std::string Log::getLastLog()
	{
		std::string cpy = m_lastLog;
		m_lastLog.resize(0);
		return cpy;
	}
	
	/////////////////////////////////////////////////////////////
	const std::string &Log::getName() const
	{
		return m_name;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::setName(const std::string &name)
	{
		m_name = name;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	bool Log::doesPromptName() const
	{
		return m_promptName;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::promptName(bool prompt)
	{
		m_promptName = prompt;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::enableLogRecall(bool enable)
	{
		m_canRecallLog = enable;
		if (!m_canRecallLog)
			m_newLog  = true,
			m_lastLog = std::string();
		return *this;
	}
}
