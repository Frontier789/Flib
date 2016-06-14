////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_UDPSOCKET_HPP_INCLUDED
#define FRONTIER_UDPSOCKET_HPP_INCLUDED

#include <FRONTIER/Network/Socket.hpp>

namespace fn
{
    class UdpSocket
    {
        Socket m_soc;
    public:

        bool create(bool ipv6 = false);
        void destroy();
        void close();

        bool bind(const IpAddress &ip);

        bool isValid() const;
        operator bool() const;

        IpAddress getRemoteAddress() const;

        bool sendTo(const void *data,fm::Size byteCount,const IpAddress &targetIp);
        bool recvFrom(void *data,fm::Size byteCount,IpAddress &sourceIp);

        SocketID getID() const;
    };
}

#endif // FRONTIER_UDPSOCKET_HPP_INCLUDED
