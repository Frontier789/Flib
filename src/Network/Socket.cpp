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
#include <FRONTIER/Network/Socket.hpp>
#include <FRONTIER/Network/SocketPredef.hpp>

#ifdef FRONTIER_OS_WINDOWS
namespace fn
{
    namespace priv
    {
        class WSAInitializer
        {
        public:
            WSAInitializer()
            {
                WSADATA dat;
                WSAStartup(MAKEWORD(2,2),&dat);
            }

            ~WSAInitializer()
            {
                WSACleanup();
            }

            static WSAInitializer wsainitializer;
        };

        WSAInitializer WSAInitializer::wsainitializer;
    }
}
#endif

namespace fn
{
    Socket::Socket() : m_id(FRONTIER_INVALID_SOCKET)
    {

    }

    Socket::~Socket()
    {
        close();
    }

    bool Socket::create(bool ipv6,bool tcp)
    {
        close();

        m_id = socket(ipv6 ? PF_INET6 : PF_INET,
                      tcp ? SOCK_STREAM : SOCK_DGRAM,
                      tcp ? IPPROTO_TCP : IPPROTO_UDP);

        if (isValid())
        {
            int yes = 1;
            setsockopt(m_id,SOL_SOCKET,SO_REUSEADDR,(const char *)&yes,sizeof(int));

            return true;
        }

        // CHECK_FOR_ERROR

        return false;
    }

    void Socket::destroy()
    {
        if (isValid())
        {
            FRONTIER_CLOSE_SOCKET(m_id);

            m_id = FRONTIER_INVALID_SOCKET;
        }
    }

    void Socket::close()
    {
        if (isValid())
        {
            shutdown(m_id, 2); // CHECK_FOR_ERROR

            char tmp;
            this->recv((void*)&tmp,sizeof(tmp));

            FRONTIER_CLOSE_SOCKET(m_id);

            m_id = FRONTIER_INVALID_SOCKET;
        }
    }

    bool Socket::connect(const IpAddress &ip)
    {
        if (!ip.isValid() || !isValid()) return false;

        int res = ::connect(m_id,(const sockaddr *)ip.getData(),ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in));

        // CHECK_FOR_ERROR

        return res == 0;
    }

    bool Socket::connect(const IpAddress &ip,fm::Uint16 port)
    {
        if (!ip.isValid() || !isValid()) return false;

        IpAddress cpy = ip;
        cpy.setPort(port);

        int res = ::connect(m_id,(const sockaddr *)cpy.getData(),ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in));

        // CHECK_FOR_ERROR

        return res == 0;
    }

    bool Socket::bind(const IpAddress &ip)
    {
        if (!isValid() || !ip.isValid()) return false;

        const sockaddr *addr = (const sockaddr *)ip.getData();

        if (::bind(m_id,addr,ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)) == FRONTIER_SOCKET_ERROR)
        {
            // CHECK_FOR_ERROR

            return false;
        }

        return true;
    }

    bool Socket::listen()
    {
        if (!isValid()) return false;

        return (::listen(m_id, SOMAXCONN) != FRONTIER_SOCKET_ERROR); // CHECK_FOR_ERROR
    }

    bool Socket::accept(Socket &soc) const
    {
        if (!isValid()) return false;

        soc.close();

        soc.m_id = ::accept(m_id,fm::nullPtr,fm::nullPtr); // CHECK_FOR_ERROR

        return soc.isValid();
    }

    bool Socket::isValid() const
    {
        return m_id != FRONTIER_INVALID_SOCKET;
    }

    Socket::operator bool() const
    {
        return isValid();
    }

    IpAddress Socket::getRemoteAddress() const
    {
        if (!isValid()) return IpAddress::invalidIp();

        IpAddress ret;

        socklen_t len = sizeof(sockaddr_storage);

        /*int result =*/ getpeername(m_id,(sockaddr *)ret.getData(),&len);

        // CHECK_FOR_ERROR

        return ret;
    }

    bool Socket::send(const void *data,fm::Size byteCount)
    {
        if (!isValid()) return false;

        const char *buf = (const char *)data;

        while (byteCount)
        {
            int sent = ::send(m_id, buf, byteCount, 0);

            if (sent == FRONTIER_SOCKET_ERROR)
            {
                // CHECK_FOR_ERROR
                return false;
            }

            buf += sent;
            byteCount -= sent;
        }

        return true;
    }

    bool Socket::sendTo(const void *data,fm::Size byteCount,const IpAddress &targetIp)
    {
        if (!isValid() || !targetIp.isValid()) return false;

        const char *buf = (const char *)data;

        while (byteCount)
        {
            int sent = ::sendto(m_id, buf, byteCount, 0, (const sockaddr*)targetIp.getData(), targetIp.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in));

            if (sent == FRONTIER_SOCKET_ERROR)
            {
                // CHECK_FOR_ERROR
                return false;
            }

            buf += sent;
            byteCount -= sent;
        }

        return true;
    }

    bool Socket::recv(void *data,fm::Size byteCount)
    {
        if (!isValid()) return false;

        char *buf = (char*)data;

        while (byteCount)
        {
            int received = ::recv(m_id, buf, byteCount, 0);

            if (received == FRONTIER_SOCKET_ERROR)
            {
                // CHECK_FOR_ERROR
                return false;
            }

            buf += received;
            byteCount -= received;
        }

        return true;
    }

    bool Socket::recvFrom(void *data,fm::Size byteCount,IpAddress &sourceIp)
    {
        if (!isValid()) return false;

        char *buf = (char*)data;

        while (byteCount)
        {
            socklen_t tmp = sourceIp.isIpv4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
            int received = ::recvfrom(m_id, buf, byteCount, 0, (sockaddr*)sourceIp.getData(), &tmp);

            if (received == FRONTIER_SOCKET_ERROR)
            {
                // CHECK_FOR_ERROR
                return false;
            }

            buf += received;
            byteCount -= received;
        }

        return true;
    }

    SocketID Socket::getID() const
    {
        return m_id;
    }
}


