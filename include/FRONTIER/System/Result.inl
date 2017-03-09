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
#ifndef FRONTIER_RESULT_INL_INCLUDED
#define FRONTIER_RESULT_INL_INCLUDED

namespace fm
{
	namespace priv
	{
		inline void append(std::vector<std::string> &details)
		{
			(void)details;
		}
		
		template<class StrClass,class... StringClass>
		inline void append(std::vector<std::string> &details,const StrClass &str,const StringClass &... detailsp)
		{
			details.push_back(str);
			
			fm::priv::append(details,detailsp...);
		}
	}
	
	/////////////////////////////////////////////////////////////
	template<class... StringClass>
	inline Result::Result(const std::string &type,
						  ErrorLevel level,
						  const std::string &error,
						  const std::string &func,
						  const std::string &file,
						  fm::Size line,
						  const StringClass &... detailsp) : type(type),
															 level(level),
															 error(error),
															 func(func),
															 file(file),
															 line(line)
	{
		priv::append(details,detailsp...);
	}
}

#include <FRONTIER/System/util/PredefStreams.hpp>

template<class CharT,class CharTraitT>
inline std::basic_ostream<CharT,CharTraitT> &operator<<(std::basic_ostream<CharT,CharTraitT> &out, const fm::Result &res)
{
	return out<<res.toString();
}

#endif // FRONTIER_RESULT_INL_INCLUDED
