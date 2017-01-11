////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/System/Result.hpp>
#include <string>

namespace fm
{
	/////////////////////////////////////////////////////////////
	Result::operator bool() const
	{
		return (level != OPFailed && type == "");
	}
	
	/////////////////////////////////////////////////////////////
	bool Result::operator!() const
	{
		return !((bool)(*this));
	}
	
	/////////////////////////////////////////////////////////////
	bool Result::operator==(const std::string &errorType) const
	{
		return type == errorType;
	}
	
	/////////////////////////////////////////////////////////////
	bool Result::operator==(ErrorLevel errorLevel) const
	{
		return level == errorLevel;
	}
	
	/////////////////////////////////////////////////////////////
	bool Result::operator!=(const std::string &errorType) const
	{
		return type != errorType;
	}
	
	/////////////////////////////////////////////////////////////
	bool Result::operator!=(ErrorLevel errorLevel) const
	{
		return level != errorLevel;
	}
	
	/////////////////////////////////////////////////////////////
	Result &Result::operator+=(const Result &result)
	{
		if (*this) *this = result;
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	std::string Result::toString(bool fullDesc) const
	{
		if (!(*this))
		{
			std::string ret = func + " failed [" + type + "." + error + "]";
			
			if (fullDesc)
			{
				ret += " in file " + file;
				ret += " at line ";
				
				fm::Size linec = line;
				
				std::string lines;
				
				if (linec == 0) lines = "0";
				else
				{
					while (linec)
					{
						lines = char(linec%10+'0') + lines;
						linec /= 10;
					}
				}
				
				ret += lines;
			}
			
			if (details.size())
			{
				ret += " {";
			
				for (fm::Size i=0;i<details.size();++i)
				{
					ret += "\""+details[i]+"\"";
					
					if (i+1 != details.size())
						ret += ",";
				}
				
				ret += "}";
			}
			
			return ret;
		}
		
		return std::string();
	}
}
