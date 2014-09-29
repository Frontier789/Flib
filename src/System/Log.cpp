//////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////
#include <FRONTIER/System/Log.hpp>
#include <sstream>
#include <ostream>
namespace fm
{
	/// Constructors //////////////////////////////////////////////////////////
	Log::Log() : m_newLog(true),
				 m_lastLog(new std::string()),
				 m_name(new std::string()),
				 m_tmp(new std::string()),
				 m_promptName(false),
				 m_os(NULL)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Log::Log(std::ostream &os) : m_newLog(true),
								 m_lastLog(new std::string()),
								 m_name(new std::string()),
								 m_tmp(new std::string()),
								 m_promptName(false),
								 m_os(&os)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Log::Log(std::ostream *os) : m_newLog(true),
								 m_lastLog(new std::string()),
								 m_name(new std::string()),
								 m_tmp(new std::string()),
								 m_promptName(false), 
								 m_os(os)
	{
		 
	}
	
	/////////////////////////////////////////////////////////////
	Log::~Log()
	{
		if (m_lastLog)
			delete (std::string*)m_lastLog;
		if (m_name)
			delete (std::string*)m_name;
		if (m_tmp)
			delete (std::string*)m_tmp;
	}
	
	/// Operators //////////////////////////////////////////////////////////
	Log::reference Log::operator=(std::ostream &os)
	{
		m_newLog  = true;
		(*m_lastLog) = std::string();
		m_os = &os;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator=(std::ostream *os)
	{
		m_newLog  = true;
		(*m_lastLog) = std::string();
		m_os = os;
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	std::ostream *Log::getStream()
	{
		return m_os;
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
		return (*m_lastLog).length()!=0;
	}
	
	/////////////////////////////////////////////////////////////
	bool Log::hasStream() const
	{
		return m_os!=NULL;
	}
	
	/////////////////////////////////////////////////////////////
	const std::string &Log::getLastLog()
	{
		(*m_tmp) = (*m_lastLog);
		(*m_lastLog).resize(0);
		return (*m_tmp);
	}
	
	/////////////////////////////////////////////////////////////
	const std::string &Log::getName() const
	{
		return (*m_name);
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::setName(const std::string &name)
	{
		(*m_name) = name;
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
			(*m_lastLog) = std::string();
		return *this;
	}
	
	template<class T>
	void putToLog(Log &log,const T &data,bool &newLog,bool canRecallLog,std::string &lastLog)
	{
		if (log.hasStream())
		{
			if (log.doesPromptName() && newLog)
				(*log.getStream())<<log.getName()<<' ';
			newLog=false;
			
			if (canRecallLog)
			{
				std::stringstream ss;
				ss<<lastLog<<data;
				lastLog = ss.str();
			}
			
			(*log.getStream())<<data;
		}
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const char *text)
	{
		putToLog(*this,text,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const std::string &text)
	{
		putToLog(*this,text,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const char &character)
	{
		putToLog(*this,character,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const int &number)
	{
		putToLog(*this,number,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const long &number)
	{
		putToLog(*this,number,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const unsigned char &number)
	{
		putToLog(*this,number,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const unsigned int &number)
	{
		putToLog(*this,number,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const unsigned long &number)
	{
		putToLog(*this,number,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const float &number)
	{
		putToLog(*this,number,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const double &number)
	{
		putToLog(*this,number,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Log::reference Log::operator<<(const void *address)
	{
		putToLog(*this,address,m_newLog,m_canRecallLog,(*m_lastLog));
		return *this;
	}
}
