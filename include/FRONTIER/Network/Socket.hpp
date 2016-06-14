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

namespace fn
{
    #ifdef FRONTIER_OS_WINDOWS

        typedef unsigned int SocketID;

    #elif defined(FRONTIER_OS_LINUX)

        typedef int SocketID;

    #endif

    class Socket
    {
        SocketID m_id;
    public:

        Socket();
        ~Socket();

        bool create(bool ipv6 = false,bool tcp = true);
        void destroy();
        void close();

        bool connect(const IpAddress &ip);
        bool connect(const IpAddress &ip,fm::Uint16 port);

        bool bind(const IpAddress &ip);
        bool listen();
        bool accept(Socket &soc) const;

        bool isValid() const;
        operator bool() const;

        IpAddress getRemoteAddress() const;

        bool send(const void *data,fm::Size byteCount);
        bool sendTo(const void *data,fm::Size byteCount,const IpAddress &targetIp);

        bool recv(void *data,fm::Size byteCount);
        bool recvFrom(void *data,fm::Size byteCount,IpAddress &sourceIp);

        SocketID getID() const;
    };
}

#endif // FRONTIER_SOCKET_HPP_INCLUDED
