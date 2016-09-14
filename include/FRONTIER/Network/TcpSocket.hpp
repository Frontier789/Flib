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
#ifndef FRONTIER_TCPSOCKET_HPP_INCLUDED
#define FRONTIER_TCPSOCKET_HPP_INCLUDED

#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Network/Socket.hpp>

namespace fn
{
	class FRONTIER_API TcpSocket
    {
    public:
        Socket m_soc;
    public:

        fm::Result create(bool ipv6 = false);
        void close();

        fm::Result connect(const IpAddress &ip);
        fm::Result connect(const IpAddress &ip,fm::Uint16 port);

        fm::Result bind(const IpAddress &ip);
        fm::Result listen();
        fm::Result accept(TcpSocket &soc) const;

        bool isValid() const;
        operator bool() const;

        IpAddress getRemoteAddress() const;

        fm::Result send(const void *data,fm::Size byteCount);
		fm::Result send(const Message &msg);
        fm::Result recv(void *data,fm::Size byteCount);
		fm::Result recv(Message &msg);

		const Socket &getSocket() const;
		Socket &getSocket();
		
        SocketID getID() const;
        
		friend class TcpListener;
    };
}

#endif // FRONTIER_TCPSOCKET_HPP_INCLUDED
