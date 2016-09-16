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
#include <FRONTIER/Network/SocketPredef.hpp>
#include <FRONTIER/Network/IpAddress.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <string>

namespace fn
{
    namespace priv
    {
        void loadAddr(const fm::String &address,fm::Uint16 port,sockaddr_storage *data,bool preferIpv6)
        {
            addrinfo *result = fm::nullPtr,hints;

            C(sizeof(hints)) ((char*)&hints)[i] = 0;
            hints.ai_family   = AF_UNSPEC;

            int iResult = getaddrinfo(address.str().c_str(), fm::toString(port).str().c_str(), &hints, &result);
            if (iResult == 0)
            {
                bool foundIpv4 = false;
                for(addrinfo *p = result;p;p = p->ai_next)
                {
                    if (p->ai_family == (preferIpv6 ? AF_INET6 : AF_INET))
                    {
                        *((sockaddr_in*)data) = *((sockaddr_in*)p->ai_addr);
                        foundIpv4 = true;
                        break;
                    }
                }
                if (!foundIpv4)
                {
                    if (result->ai_family == AF_INET6)
                        *((sockaddr_in6*)data) = *((sockaddr_in6*)result->ai_addr);
                    else
                        *((sockaddr_in*)data) = *((sockaddr_in*)result->ai_addr);
                }
            }

            freeaddrinfo(result);
        }
    }

    void IpAddress::init(const fm::String &addr,fm::Uint16 port,bool preferIpv6)
    {
        m_data = new sockaddr_storage;

        set(addr,port,preferIpv6);
    }

    IpAddress::IpAddress() : m_data(new sockaddr_storage)
    {
        reset();
    }

    IpAddress::IpAddress(fm::Uint8 n0,fm::Uint8 n1,fm::Uint8 n2,fm::Uint8 n3,
                         fm::Uint16 port) : m_data(new sockaddr_storage)
    {
        set(fm::toString(n0)+"."+fm::toString(n1)+"."+fm::toString(n2)+"."+fm::toString(n3),port);
    }

    IpAddress::IpAddress(const fm::String &address,fm::Uint16 port,bool preferIpv6) : m_data(new sockaddr_storage)
    {
        set(address,port,preferIpv6);
    }

    IpAddress::IpAddress(const IpAddress &addr) : m_data(new sockaddr_storage)
    {
        *((sockaddr_storage*)m_data) = *((sockaddr_storage*)addr.m_data);
    }

    IpAddress::IpAddress(const IpAddress &addr,fm::Uint16 port) : m_data(new sockaddr_storage)
    {
        *((sockaddr_storage*)m_data) = *((sockaddr_storage*)addr.m_data);
        setPort(port);
    }

    IpAddress &IpAddress::operator=(const IpAddress &addr)
    {
        *((sockaddr_storage*)m_data) = *((sockaddr_storage*)addr.m_data);

        return *this;
    }

    IpAddress::~IpAddress()
    {
        delete (sockaddr_storage*)m_data;
    }

    IpAddress &IpAddress::set(const fm::String &address,fm::Uint16 port,bool preferIpv6)
    {
        fm::String realAddr;

        reset();

        fm::Size beg = address.find(":");

        if (beg != fm::String::npos)
        {
            fm::Size last = address.find_last_of(":");

            if (beg == last)
            {
                if (!port)
                {
                    for (fm::Size i = beg+1;i<address.size();++i)
                    {
                        fm::Uint32 c = address[i];
                        if (c < '0' || c > '9')
                            return *this;

                        port = port*10 + c-'0';
                    }
                }
                realAddr = address.substr(0,beg);
            }
            else
            {
                bool per = true;
                fm::Size lt = address.find_last_of('/');

                if (lt == fm::String::npos)
                {
                    lt  = address.find_last_of(']');
                    per = false;
                }

                if (lt != fm::String::npos)
                {
                    if (!port)
                    {
                        for (fm::Size i = lt+1;i<address.size();++i)
                        {
                            fm::Uint32 c = address[i];

                            if (i == lt+1 && !per && c == ':') continue;

                            if (c < '0' || c > '9')
                            {
                                reset();
                                return *this;
                            }

                            port = port*10 + c-'0';
                        }
                    }

                    realAddr = address.substr(0,lt);
                }
            }
        }
        else 
            realAddr = address;

        fn::priv::loadAddr(realAddr,port,((sockaddr_storage*)m_data),preferIpv6);

        return *this;
    }

    IpAddress &IpAddress::operator=(const fm::String &ip)
    {
        return set(ip);
    }

    void IpAddress::reset()
    {
        for (fm::Size i=0;i<sizeof(sockaddr_storage);++i)
            ((fm::Uint8*)((sockaddr_storage*)m_data))[i] = 0;
    }

    bool IpAddress::isValid() const
    {
        return ((sockaddr*)m_data)->sa_family != 0;
    }

    bool IpAddress::isIpv4() const
    {
        return ((sockaddr*)m_data)->sa_family == PF_INET;
    }

    bool IpAddress::isIpv6() const
    {
        return ((sockaddr*)m_data)->sa_family == PF_INET6;
    }

    IpAddress::operator bool() const
    {
        return isValid();
    }

    fm::String IpAddress::toString() const
    {
        if (((sockaddr*)m_data)->sa_family == PF_INET)
        {
            sockaddr_in *addr = (sockaddr_in*)m_data;

            fm::String s;
            C(4)
            {
                if (i) s += ".";

                u_char num = ((u_char*)&addr->sin_addr)[i];
                s += fm::toString(num);
            }

            fm::Uint16 port = ntohs(addr->sin_port);

            if (port)
                s += ":" + fm::toString(port);

            return s;
        }

        if (((sockaddr*)m_data)->sa_family == PF_INET6)
        {
            sockaddr_in6 *addr = (sockaddr_in6*)m_data;

            fm::String s;
            C(8)
            {
                if (i) s += ":";

                u_short num = ntohs(((u_short*)(&addr->sin6_addr))[i]);

                Cx(4)
                {
                    char cs;
                    unsigned char n = ((num / 16) % 16);
                    num /= 16;

                    if (n < 10) cs = '0'+n;
                    else cs = 'a'+n-10;

                    s += cs;
                }
            }

            fm::Uint16 port = ntohs(addr->sin6_port);

            if (port)
                s += "/" + fm::toString(port);

            return s.str();
        }

        return "<invalid>";
    }

    fm::Uint16 IpAddress::getPort() const
    {
        if (isValid())
        {
            if (isIpv4())
                return ntohs(((sockaddr_in*)getData())->sin_port);
            else
                return ntohs(((sockaddr_in6*)getData())->sin6_port);
        }

        return 0;
    }

    void IpAddress::setPort(fm::Uint16 port)
    {
        if (isValid())
        {
            if (isIpv4())
                ((sockaddr_in*)getData())->sin_port   = htons(port);
            else
                ((sockaddr_in6*)getData())->sin6_port = htons(port);
        }
    }

    void *IpAddress::getData()
    {
        return m_data;
    }

    const void *IpAddress::getData() const
    {
        return m_data;
    }

    IpAddress IpAddress::invalidIp()
    {
        return IpAddress();
    }

    IpAddress IpAddress::broadcastIp()
    {
        return IpAddress("255.255.255.255");
    }

    IpAddress IpAddress::localHost(fm::Uint16 port,bool useIpv6)
    {
        addrinfo *result = fm::nullPtr;
        addrinfo hints;

        C(sizeof(hints)) ((char*)&hints)[i] = 0;
        hints.ai_family   = (useIpv6 ? AF_INET6 : AF_INET);
        hints.ai_flags    = AI_PASSIVE;

        // Resolve the local address and port
        int iResult = getaddrinfo(fm::nullPtr, fm::toString(port).str().c_str(), &hints, &result);
        if (iResult != 0)
        {
            // WSAGetLastError()
            return IpAddress::invalidIp();
        }

        IpAddress ret;

            if (result->ai_addr->sa_family == AF_INET)
                *((sockaddr_in*)ret.getData()) = *((sockaddr_in*)result->ai_addr);
            else
                *((sockaddr_in6*)ret.getData()) = *((sockaddr_in6*)result->ai_addr);


        freeaddrinfo(result);

        return ret;
    }

    IpAddress IpAddress::localAddress()
    {/*
        ::priv::SocketID soc = socket(PF_INET, SOCK_DGRAM, 0);

        if (soc == FRONTIER_INVALID_SOCKET)
            return IpAddress::invalidIp();

        sockaddr_in addr;
        C(sizeof(sockaddr_in)) ((char*)&addr)[i] = 0;

        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(42);

        if (connect(soc,(sockaddr*)&addr,sizeof(addr)) == -1)
        {
            ::priv::closeSocket(soc);
            return IpAddress::invalidIp();
        }

        socklen_t len = sizeof(addr);
        if (getsockname(soc,(sockaddr*)&addr,&len) == -1)
        {
            ::priv::closeSocket(soc);
            return IpAddress::invalidIp();
        }

        ::priv::closeSocket(soc);

        IpAddress ret;
        *((sockaddr_in*)ret.getData()) = addr;
        ret.setPort(0);
        return ret;

*/



        IpAddress ret;

        char name[2550];
        hostent *hostinfo;


        if (gethostname(name, sizeof(name)) == 0)
        {
            if ((hostinfo = gethostbyname(name)) != fm::nullPtr)
            {
                if (hostinfo->h_addrtype == AF_INET)
                {
                    sockaddr_in *p = ((sockaddr_in *)ret.getData());
                    p->sin_family = AF_INET;
                    p->sin_addr   = (*(in_addr *)*hostinfo->h_addr_list);
                }
                else if (hostinfo->h_addrtype == AF_INET6)
                {
                    sockaddr_in6 *p = ((sockaddr_in6 *)ret.getData());
                    p->sin6_family = AF_INET6;
                    p->sin6_addr   = (*(in6_addr *)*hostinfo->h_addr_list);
                }
            }
        }

        return ret;
    }
}

////////////////////////////////////////////////////////////
std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out, const fn::IpAddress &ip)
{
	return out << ip.toString().str();
}

////////////////////////////////////////////////////////////
std::basic_istream<char, std::char_traits<char> > &operator>>(std::basic_istream<char, std::char_traits<char> > &in, fn::IpAddress &ip)
{
	std::string str;
	in >> str;
	ip = fm::String::fromUtf8(str);
	return in;
}
