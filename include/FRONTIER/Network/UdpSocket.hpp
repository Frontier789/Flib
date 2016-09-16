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
#ifndef FRONTIER_UDPSOCKET_HPP_INCLUDED
#define FRONTIER_UDPSOCKET_HPP_INCLUDED

#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Network/Socket.hpp>

namespace fn
{
	class FRONTIER_API UdpSocket : public Socket
    {
    public:

        fm::Result create(bool ipv6 = false);
        
        fm::Result bind(const IpAddress &ip);

        fm::Result sendTo(const void *data,fm::Size byteCount,const IpAddress &targetIp);
		fm::Result sendTo(const Message &msg,const IpAddress &targetIp);
		fm::Result recvFrom(void *data,fm::Size byteCount,fm::Ref<IpAddress> sourceIp = fm::nullPtr);
		fm::Result recvFrom(Message &msg,fm::Ref<IpAddress> sourceIp = fm::nullPtr);
    };
}

#endif // FRONTIER_UDPSOCKET_HPP_INCLUDED
