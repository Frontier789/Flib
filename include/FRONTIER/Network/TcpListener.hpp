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
#ifndef FRONTIER_TCPLISTENER_HPP_INCLUDED
#define FRONTIER_TCPLISTENER_HPP_INCLUDED

#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Network/Socket.hpp>
#include <FRONTIER/System/Result.hpp>

namespace fn
{
	class TcpSocket;
	
	class FRONTIER_API TcpListener
    {
    public:
        Socket m_socket;
    public:

        fm::Result listen(const IpAddress &ip);
        fm::Result listen(const IpAddress &ip,fm::Uint16 port);
        fm::Result listen(fm::Uint16 port,bool useIpv6 = false);

        fm::Result accept(TcpSocket &soc);

		const Socket &getSocket() const;
		Socket &getSocket();
		
        SocketID getID() const;

        bool isValid() const;

        operator bool() const;
    };
}

#endif // FRONTIER_TCPLISTENER_HPP_INCLUDED
