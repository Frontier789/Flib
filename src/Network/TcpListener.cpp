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
#include <FRONTIER/Network/TcpListener.hpp>
#include <FRONTIER/Network/TcpSocket.hpp>
#include <FRONTIER/System/Error.hpp>

namespace fn
{
    fm::Result TcpListener::listen(const IpAddress &ip)
    {
        if (fm::Error err = m_socket.create(ip.isIpv6(),true)) return err;
        if (fm::Error err = m_socket.bind(ip)) return err;
        if (fm::Error err = m_socket.listen()) return err;

        return fm::Result();
    }
    fm::Result TcpListener::listen(const IpAddress &ip,fm::Uint16 port)
    {
        if (fm::Error err = m_socket.create(ip.isIpv6(),true)) return err;
        if (fm::Error err = m_socket.bind(IpAddress(ip,port))) return err;
        if (fm::Error err = m_socket.listen()) return err;

        return fm::Result();
    }
    fm::Result TcpListener::listen(fm::Uint16 port,bool useIpv6)
    {
        if (fm::Error err = m_socket.create(useIpv6,true)) return err;
        if (fm::Error err = m_socket.bind(IpAddress::localHost(port,useIpv6))) return err;
		if (fm::Error err = m_socket.listen()) return err;

        return fm::Result();
    }

    fm::Result TcpListener::accept(TcpSocket &soc)
    {
        return m_socket.accept(soc.m_soc);
    }
	
	const Socket &TcpListener::getSocket() const
	{
		return m_socket;
	}
	
	Socket &TcpListener::getSocket()
	{
		return m_socket;
	}

    SocketID TcpListener::getID() const
	{
		return m_socket.getID();
	}
    
    bool TcpListener::isValid() const
    {
        return m_socket.isValid();
    }

    TcpListener::operator bool() const
    {
        return isValid();
    }
}
