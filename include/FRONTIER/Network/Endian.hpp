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
#ifndef FRONTIER_ENDIAN_HPP_INCLUDED
#define FRONTIER_ENDIAN_HPP_INCLUDED

#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/API.h>

namespace fn
{
	bool FRONTIER_API littleEndian();
	
	template<class T> 
	inline T htonc(const T& num);
	
	template<class T> 
	inline T ntohc(const T& num);
	
	/** faster specialisations **/
	
	template<> 
	fm::Uint8 htonc(const fm::Uint8& num);
	
	template<> 
	fm::Uint16 htonc(const fm::Uint16& num);
	
	template<> 
	fm::Uint32 htonc(const fm::Uint32& num);
	
	template<> 
	fm::Int8 htonc(const fm::Int8& num);
	
	template<> 
	fm::Int16 htonc(const fm::Int16& num);
	
	template<> 
	fm::Int32 htonc(const fm::Int32& num);
	
	template<> 
	fm::Uint8 ntohc(const fm::Uint8& num);
	
	template<> 
	fm::Uint16 ntohc(const fm::Uint16& num);
	
	template<> 
	fm::Uint32 ntohc(const fm::Uint32& num);
	
	template<> 
	fm::Int8 ntohc(const fm::Int8& num);
	
	template<> 
	fm::Int16 ntohc(const fm::Int16& num);
	
	template<> 
	fm::Int32 ntohc(const fm::Int32& num);
	
	/** ------------- **/
}

#endif // FRONTIER_ENDIAN_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Network/Endian.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
