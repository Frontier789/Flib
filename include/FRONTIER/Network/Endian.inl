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
#ifndef FRONTIER_ENDIAN_INL_INCLUDED
#define FRONTIER_ENDIAN_INL_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>

namespace fn
{
	namespace priv
	{
		class EndianDetect
		{
		public:
			bool big;
			
			EndianDetect()
			{
				const int one = 1;
				const char sig = *(char*)&one;
				
				big = (sig == 0);
			}
			
			static EndianDetect endianDetect;
		};
	}
	
	template<typename T> 
	inline T htonc(const T& num)
	{
		if (priv::EndianDetect::endianDetect.big) return num; // for big endian machine just return the input

		T ret = num;
		for (fm::Size i = 0;i<sizeof(T);++i)
		{
			*((char*)&ret + i) = *((char*)&num + sizeof(T) - 1 - i);
		}
		
		return ret;
	}
	
	template<typename T> 
	inline T ntohc(const T& num)
	{
		return htonc(num);
	}
}

#endif // FRONTIER_ENDIAN_INL_INCLUDED
