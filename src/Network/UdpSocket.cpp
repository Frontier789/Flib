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
#include <FRONTIER/Network/UdpSocket.hpp>

namespace fn
{
    fm::Result UdpSocket::create(bool ipv6)
    {
        return getImpl().create(ipv6,false);
    }

    fm::Result UdpSocket::bind(const IpAddress &ip)
    {
        create(ip.isIpv6());

        return getImpl().bind(ip);
    }

    fm::Result UdpSocket::sendTo(const void *data,fm::Uint32 byteCount,const IpAddress &targetIp)
    {
        return getImpl().sendTo(data,byteCount,targetIp);
    }
	fm::Result UdpSocket::sendTo(const Message &msg,const IpAddress &targetIp)
    {
        return getImpl().sendTo(msg,targetIp);
    }
    fm::Result UdpSocket::recvFrom(void *data,fm::Uint32 byteCount,fm::Ref<IpAddress> sourceIp)
    {
        return getImpl().recvFrom(data,byteCount,sourceIp);
    }
	fm::Result UdpSocket::recvFrom(Message &msg,fm::Ref<IpAddress> sourceIp)
    {
        return getImpl().recvFrom(msg,sourceIp);
    }
}


