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
#ifndef FRONTIER_TCPLISTENER_HPP_INCLUDED
#define FRONTIER_TCPLISTENER_HPP_INCLUDED

#include <FRONTIER/Network/TcpSocket.hpp>

namespace fn
{
    class TcpListener
    {
        TcpSocket m_socket;
    public:

        bool listen(const IpAddress &ip);
        bool listen(const IpAddress &ip,fm::Uint16 port);
        bool listen(fm::Uint16 port,bool useIpv6 = false);

        bool accept(TcpSocket &soc);

        bool isValid() const;

        operator bool() const;
    };
}

#endif // FRONTIER_TCPLISTENER_HPP_INCLUDED
