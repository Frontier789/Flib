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
#ifndef FRONTIER_IPADDRESS_HPP_INCLUDED
#define FRONTIER_IPADDRESS_HPP_INCLUDED

#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>

namespace fn
{
    class IpAddress
    {
        void *m_data;
        void init(const fm::String &addr,fm::Uint16 port,bool preferIpv6);
    public:

        IpAddress();
        IpAddress(fm::Uint8 n0,fm::Uint8 n1,fm::Uint8 n2,fm::Uint8 n3,fm::Uint16 port = 0);

        IpAddress(const fm::String &address,fm::Uint16 port = 0,bool preferIpv6 = false);

        template<fm::Size LEN>
        IpAddress(const char (&address)[LEN],fm::Uint16 port = 0,bool preferIpv6 = false);

        IpAddress(const IpAddress &addr);
        IpAddress(const IpAddress &addr,fm::Uint16 port);
        IpAddress &operator=(const IpAddress &addr);
        ~IpAddress();
        IpAddress &set(const fm::String &address,fm::Uint16 port = 0,bool preferIpv6 = false);
        IpAddress &operator=(const fm::String &ip);

        void reset();

        bool isValid() const;
        bool isIpv4() const;
        bool isIpv6() const;

        operator bool() const;

        fm::String toString() const;

        fm::Uint16 getPort() const;
        void setPort(fm::Uint16 port);

        void *getData();
        const void *getData() const;

        static IpAddress invalidIp();
        static IpAddress broadcastIp();
        static IpAddress localHost(fm::Uint16 port = 0,bool useIpv6 = false);
        static IpAddress localAddress();
    };
}

#ifndef FRONTIER_DONT_INCLUDE_INL
    #include <FRONTIER/Network/IpAddress.inl>
#endif

#endif // FRONTIER_IPADDRESS_HPP_INCLUDED
