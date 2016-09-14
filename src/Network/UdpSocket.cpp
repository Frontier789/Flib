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
        return m_soc.create(ipv6,false);
    }
    
    void UdpSocket::close()
    {
        m_soc.close();
    }

    fm::Result UdpSocket::bind(const IpAddress &ip)
    {
        if (!isValid())
            create(ip.isIpv6());

        return m_soc.bind(ip);
    }

    bool UdpSocket::isValid() const
    {
        return m_soc.isValid();
    }
    UdpSocket::operator bool() const
    {
        return m_soc.isValid();
    }

    IpAddress UdpSocket::getRemoteAddress() const
    {
        return m_soc.getRemoteAddress();
    }

    fm::Result UdpSocket::sendTo(const void *data,fm::Size byteCount,const IpAddress &targetIp)
    {
        return m_soc.sendTo(data,byteCount,targetIp);
    }
	fm::Result UdpSocket::sendTo(const Message &msg,const IpAddress &targetIp)
    {
        return m_soc.sendTo(msg,targetIp);
    }
    fm::Result UdpSocket::recvFrom(void *data,fm::Size byteCount,IpAddress &sourceIp)
    {
        return m_soc.recvFrom(data,byteCount,sourceIp);
    }
	fm::Result UdpSocket::recvFrom(Message &msg,IpAddress &sourceIp)
    {
        return m_soc.recvFrom(msg,sourceIp);
    }
	
	const Socket &UdpSocket::getSocket() const
	{
		return m_soc;
	}
	
	Socket &UdpSocket::getSocket()
	{
		return m_soc;
	}

    SocketID UdpSocket::getID() const
    {
        return m_soc.getID();
    }
}


