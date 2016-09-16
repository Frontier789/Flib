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
#ifndef FRONTIER_SOCKETSELECTOR_HPP_INCLUDED
#define FRONTIER_SOCKETSELECTOR_HPP_INCLUDED

#include <FRONTIER/Network/UdpSocket.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Time.hpp>
#include <FRONTIER/System/Ref.hpp>
#include <vector>

namespace fn
{
	class Socket;
	
	class FRONTIER_API SocketSelector
	{
		UdpSocket m_stopperSoc;
		std::vector<Socket *> m_sockets;
	public:
		
		SocketSelector();
		
		SocketSelector &addSocket(Socket &socket);
		SocketSelector &remSocket(Socket &socket);
		
		fm::Result select(const fm::Time &timeout = fm::Time::Inf,fm::Ref<fm::Size> readyCount = fm::nullPtr);
		
		void stop();
		void clear();
		fm::Size getSize();
		
		static fm::Size getMaxSize();
	};
	
	SocketSelector &operator<<(SocketSelector &selector,Socket &socket);
}

#endif // FRONTIER_SOCKETSELECTOR_HPP_INCLUDED
