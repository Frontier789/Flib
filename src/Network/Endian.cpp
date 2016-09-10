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
#include <FRONTIER/Network/Endian.hpp>


namespace fn
{
	namespace priv
	{
		EndianDetect EndianDetect::endianDetect;
	}
	
	bool littleEndian()
	{
		return !priv::EndianDetect::endianDetect.big;
	}
	
	template<> 
	fm::Uint8 htonc(const fm::Uint8& num)
	{
		return num;
	}
	
	template<> 
	fm::Uint16 htonc(const fm::Uint16& num)
	{
		return (num << 8) | ((num >> 8) & 0x00ff);
	}
	
	template<> 
	fm::Uint32 htonc(const fm::Uint32& num)
	{
		return (num << 24) |
			  ((num <<  8) & 0x00ff0000) |
			  ((num >>  8) & 0x0000ff00) |
			  ((num >> 24) & 0x000000ff);
	}
	
	template<> 
	fm::Int8 htonc(const fm::Int8& num)
	{
		return num;
	}
	
	template<> 
	fm::Int16 htonc(const fm::Int16& num)
	{
		return (num << 8) | ((num >> 8) & 0x00ff);
	}
	
	template<> 
	fm::Int32 htonc(const fm::Int32& num)
	{
		return (num << 24) |
			  ((num <<  8) & 0x00ff0000) |
			  ((num >>  8) & 0x0000ff00) |
			  ((num >> 24) & 0x000000ff);
	}
	
	template<> 
	fm::Uint8 ntohc(const fm::Uint8& num)
	{
		return num;
	}
	
	template<> 
	fm::Uint16 ntohc(const fm::Uint16& num)
	{
		return (num << 8) | ((num >> 8) & 0x00ff);
	}
	
	template<> 
	fm::Uint32 ntohc(const fm::Uint32& num)
	{
		return (num << 24) |
			  ((num <<  8) & 0x00ff0000) |
			  ((num >>  8) & 0x0000ff00) |
			  ((num >> 24) & 0x000000ff);
	}
	
	template<> 
	fm::Int8 ntohc(const fm::Int8& num)
	{
		return num;
	}
	
	template<> 
	fm::Int16 ntohc(const fm::Int16& num)
	{
		return (num << 8) | ((num >> 8) & 0x00ff);
	}
	
	template<> 
	fm::Int32 ntohc(const fm::Int32& num)
	{
		return (num << 24) |
			  ((num <<  8) & 0x00ff0000) |
			  ((num >>  8) & 0x0000ff00) |
			  ((num >> 24) & 0x000000ff);
	}
}
