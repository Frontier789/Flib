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
#include <FRONTIER/Network/TcpSocket.hpp>

namespace fn
{
    bool TcpSocket::create(bool ipv6)
    {
        return m_soc.create(ipv6,true);
    }
    void TcpSocket::destroy()
    {
        m_soc.destroy();
    }
    void TcpSocket::close()
    {
        m_soc.close();
    }

    bool TcpSocket::connect(const IpAddress &ip)
    {
        if (!isValid())
            create(ip.isIpv6());

        return m_soc.connect(ip);
    }
    bool TcpSocket::connect(const IpAddress &ip,fm::Uint16 port)
    {
        if (!isValid())
            create(ip.isIpv6());

        return m_soc.connect(ip,port);
    }

    bool TcpSocket::bind(const IpAddress &ip)
    {
        if (!isValid())
            create(ip.isIpv6());

        return m_soc.bind(ip);
    }
    bool TcpSocket::listen()
    {
        return m_soc.listen();
    }
    bool TcpSocket::accept(TcpSocket &soc) const
    {
        return m_soc.accept(soc.m_soc);
    }

    bool TcpSocket::isValid() const
    {
        return m_soc.isValid();
    }
    TcpSocket::operator bool() const
    {
        return isValid();
    }

    IpAddress TcpSocket::getRemoteAddress() const
    {
        return m_soc.getRemoteAddress();
    }

    bool TcpSocket::send(const void *data,fm::Size byteCount)
    {
        return m_soc.send(data,byteCount);
    }
    bool TcpSocket::recv(void *data,fm::Size byteCount)
    {
        return m_soc.recv(data,byteCount);
    }

    SocketID TcpSocket::getID() const
    {
        return m_soc.getID();
    }
}

