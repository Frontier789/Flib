////////////////////////////////////////////////////////////////////////// <!--
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
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_BUFFER_INL_INCLUDED
#define FRONTIER_BUFFER_INL_INCLUDED
namespace fg
{
	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S])
	{
		return setData(&data[0],sizeof(data));
	}

	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S],Usage usage)
	{
		return setData(&data[0],sizeof(data),m_type,usage);
	}

	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S],BufferType type)
	{
		return setData(&data[0],sizeof(data),type,m_usage);
	}

	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S],BufferType type,Usage usage)
	{
		return setData(&data[0],sizeof(data),type,usage);
	}
}
#endif //FRONTIER_BUFFER_INL_INCLUDED