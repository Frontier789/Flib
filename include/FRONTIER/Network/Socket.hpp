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
#ifndef FRONTIER_SOCKET_HPP_INCLUDED
#define FRONTIER_SOCKET_HPP_INCLUDED

#include <FRONTIER/Network/IpAddress.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Result.hpp>

namespace fn
{
    #ifdef FRONTIER_OS_WINDOWS

        typedef unsigned int SocketID;

    #elif defined(FRONTIER_OS_LINUX)

        typedef int SocketID;

    #endif
    
	class Message;

	class FRONTIER_API Socket
    {
        SocketID m_id;
		bool m_blocking;
    public:

        Socket();
        ~Socket();

        fm::Result create(bool ipv6 = false,bool tcp = true);
        void close();
		void shutdown(bool read = true,bool write = true);

        fm::Result connect(const IpAddress &ip);
        fm::Result connect(const IpAddress &ip,fm::Uint16 port);

        fm::Result bind(const IpAddress &ip);
        fm::Result listen();
        fm::Result accept(Socket &soc) const;

        bool isValid() const;
        operator bool() const;
        
		void setBlocking(bool blocking = true);
		bool isBlocking() const;

        IpAddress getRemoteAddress() const;

		fm::Result send(const Message &msg);
        fm::Result send(const void *data,fm::Size byteCount);
        fm::Result sendTo(const Message &msg,const IpAddress &targetIp);
        fm::Result sendTo(const void *data,fm::Size byteCount,const IpAddress &targetIp);

		fm::Result recv(Message &msg);
        fm::Result recv(void *data,fm::Size byteCount);
		fm::Result recvFrom(Message &msg,IpAddress &sourceIp);
        fm::Result recvFrom(void *data,fm::Size byteCount,IpAddress &sourceIp);

        SocketID getID() const;
    };
}

#endif // FRONTIER_SOCKET_HPP_INCLUDED
