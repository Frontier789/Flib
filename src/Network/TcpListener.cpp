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
        if (fm::Error err = getImpl().create(ip.isIpv6(),true)) return err;
        if (fm::Error err = getImpl().bind(ip)) return err;
        if (fm::Error err = getImpl().listen()) return err;

        return fm::Result();
    }
    fm::Result TcpListener::listen(const IpAddress &ip,fm::Uint16 port)
    {
        if (fm::Error err = getImpl().create(ip.isIpv6(),true)) return err;
        if (fm::Error err = getImpl().bind(IpAddress(ip,port))) return err;
        if (fm::Error err = getImpl().listen()) return err;

        return fm::Result();
    }
    fm::Result TcpListener::listen(fm::Uint16 port,bool useIpv6)
    {
        if (fm::Error err = getImpl().create(useIpv6,true)) return err;
        if (fm::Error err = getImpl().bind(IpAddress::localHost(port,useIpv6))) return err;
		if (fm::Error err = getImpl().listen()) return err;

        return fm::Result();
    }

    fm::Result TcpListener::accept(TcpSocket &soc)
    {
        return getImpl().accept(soc.getImpl());
    }
}
