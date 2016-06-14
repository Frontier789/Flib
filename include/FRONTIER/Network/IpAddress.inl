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
#ifndef FRONTIER_IPADDRESS_INL_INCLUDED
#define FRONTIER_IPADDRESS_INL_INCLUDED

namespace fn
{
    template<fm::Size LEN>
    IpAddress::IpAddress(const char (&address)[LEN],fm::Uint16 port,bool preferIpv6) : m_data(fm::nullPtr)
    {
        init(address,port,preferIpv6);
    }
}

#endif // FRONTIER_IPADDRESS_INL_INCLUDED
