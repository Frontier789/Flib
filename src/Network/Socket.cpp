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
#include <FRONTIER/Network/Message.hpp>
#include <FRONTIER/Network/Socket.hpp>
#include <FRONTIER/Network/Endian.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include "fnGetLastError.hpp"

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
    
	void setBlocking(SocketID id,bool blocking)
	{
		u_long val = blocking ? 0 : 1;
		ioctlsocket(id, FIONBIO, &val);
	}
}
#endif


#ifdef FRONTIER_OS_LINUX
namespace fn
{
	void setBlocking(SocketID id,bool blocking)
	{
		int status = fcntl(sock, F_GETFL);
		
		if (blocking)
			fcntl(id, F_SETFL, status &~ O_NONBLOCK);
		else
			fcntl(id, F_SETFL, status  | O_NONBLOCK);
	}
}
#endif

namespace fn
{
    Socket::Socket() : m_id(FRONTIER_INVALID_SOCKET),
					   m_blocking(true)
    {

    }

    Socket::~Socket()
    {
        close();
    }

    fm::Result Socket::create(bool ipv6,bool tcp)
    {
        close();

        m_id = ::socket(ipv6 ? PF_INET6 : PF_INET,
						tcp ? SOCK_STREAM : SOCK_DGRAM,
						tcp ? IPPROTO_TCP : IPPROTO_UDP);

        if (isValid())
        {
            int yes = 1;
            ::setsockopt(m_id,SOL_SOCKET,SO_REUSEADDR,(const char *)&yes,sizeof(yes));
            
            if (tcp)
			{
				setsockopt(m_id,IPPROTO_TCP,TCP_NODELAY,(const char *)&yes, sizeof(yes));
			}
			else
			{
				setsockopt(m_id,SOL_SOCKET,SO_BROADCAST,(const char *)&yes, sizeof(yes));
			}

            return fm::Result();
        }
        
		return FRONTIER_FN_GETLASTERROR("socket");
    }

    void Socket::close()
    {
        if (isValid())
        {
            FRONTIER_CLOSE_SOCKET(m_id);

            m_id = FRONTIER_INVALID_SOCKET;
        }
    }
/*
    void Socket::closeCarefully()
    {
        if (isValid())
        {
            shutdown(m_id, 2); // CHECK_FOR_ERROR

            char tmp;
            this->recv((void*)&tmp,sizeof(tmp));

            FRONTIER_CLOSE_SOCKET(m_id);

            m_id = FRONTIER_INVALID_SOCKET;
        }
    }*/

    fm::Result Socket::connect(const IpAddress &ip)
    {
		if (!ip.isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized socket","connect",__FILE__,__LINE__);
		if (!isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized address","connect",__FILE__,__LINE__);

		if (::connect(m_id,(const sockaddr *)ip.getData(),ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)))
			return FRONTIER_FN_GETLASTERROR("connect");

        return fm::Result();
    }

    fm::Result Socket::connect(const IpAddress &ip,fm::Uint16 port)
    {
		if (!ip.isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized socket","connect",__FILE__,__LINE__);
		if (!isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized address","connect",__FILE__,__LINE__);

        IpAddress cpy = ip;
        cpy.setPort(port);

		if (::connect(m_id,(const sockaddr *)cpy.getData(),ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)))
			return FRONTIER_FN_GETLASTERROR("connect");

        return fm::Result();
    }

    fm::Result Socket::bind(const IpAddress &ip)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","bind called on uninitialized socket","bind",__FILE__,__LINE__);
		if (!ip.isValid()) return fm::Error("NetError","Uninitialized","bind called on uninitialized address","bind",__FILE__,__LINE__);

        const sockaddr *addr = (const sockaddr *)ip.getData();

        if (::bind(m_id,addr,ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)) == FRONTIER_SOCKET_ERROR)
			return FRONTIER_FN_GETLASTERROR("bind");

        return fm::Result();
    }

    fm::Result Socket::listen()
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","listen called on uninitialized socket","listen",__FILE__,__LINE__);
		
		if (::listen(m_id, SOMAXCONN))
			return FRONTIER_FN_GETLASTERROR("listen");
		
        return fm::Result();
    }

    fm::Result Socket::accept(Socket &soc) const
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","accept called on uninitialized socket","accept",__FILE__,__LINE__);

        soc.close();

        soc.m_id = ::accept(m_id,fm::nullPtr,fm::nullPtr); // CHECK_FOR_ERROR
		
		if (!soc.isValid())
			return FRONTIER_FN_GETLASTERROR("accept");
		
        return fm::Result();
    }

    bool Socket::isValid() const
    {
        return m_id != FRONTIER_INVALID_SOCKET;
    }

    Socket::operator bool() const
    {
        return isValid();
    }
        
	void Socket::setBlocking(bool blocking)
	{
		fn::setBlocking(m_id,blocking);
		m_blocking = blocking;
	}
	
	bool Socket::isBlocking() const
	{
		return m_blocking;
	}

    IpAddress Socket::getRemoteAddress() const
    {
        if (!isValid()) return IpAddress::invalidIp();

        IpAddress ret;

        socklen_t len = sizeof(sockaddr_storage);

		if (getpeername(m_id,(sockaddr *)ret.getData(),&len))
			return IpAddress();
		
        return ret;
    }

	fm::Result Socket::send(const Message &msg)
    {
		msg.prepareSend();
		
		return send(msg.getPtr(),msg.getSize());
    }

    fm::Result Socket::send(const void *data,fm::Size byteCount)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","send called on uninitialized socket","send",__FILE__,__LINE__);

        const char *buf = (const char *)data;

        while (byteCount)
        {
            int sent = ::send(m_id, buf, byteCount, 0);

            if (sent == FRONTIER_SOCKET_ERROR)
                return FRONTIER_FN_GETLASTERROR("send");

            buf += sent;
            byteCount -= sent;
        }

        return fm::Result();
    }

	fm::Result Socket::sendTo(const Message &msg,const IpAddress &targetIp)
	{
		msg.prepareSend();
		
		return sendTo(msg.getPtr(),msg.getSize(),targetIp);
    }

    fm::Result Socket::sendTo(const void *data,fm::Size byteCount,const IpAddress &targetIp)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","sendTo called on uninitialized socket","sendTo",__FILE__,__LINE__);
		if (!targetIp.isValid()) return fm::Error("NetError","Uninitialized","sendTo called on uninitialized ip","sendTo",__FILE__,__LINE__);

        const char *buf = (const char *)data;

        while (byteCount)
        {
            int sent = ::sendto(m_id, buf, byteCount, 0, (const sockaddr*)targetIp.getData(), targetIp.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in));

            if (sent == FRONTIER_SOCKET_ERROR)
                return FRONTIER_FN_GETLASTERROR("sendto");

            buf += sent;
            byteCount -= sent;
        }
        
        return fm::Result();
    }
	
	fm::Result Socket::recv(Message &msg)
    {
		msg.clear();
		
		fm::Size s;
		if (fm::Error e = recv(&s,sizeof(s)))
		{
			return e;
		}
		
		*((fm::Size*)msg.getPtr()) = s;
		
		s = fn::ntohc(s);
		
		msg.append(fm::nullPtr,s-sizeof(fm::Size));
		
		return recv(((fm::Uint8*)msg.getPtr())+sizeof(fm::Size),s-sizeof(fm::Size));
    }
	
    fm::Result Socket::recv(void *data,fm::Size byteCount)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","recv called on uninitialized socket","recv",__FILE__,__LINE__);

        char *buf = (char*)data;

        while (byteCount)
        {
            int received = ::recv(m_id, buf, byteCount, 0);

            if (received == FRONTIER_SOCKET_ERROR)
                return FRONTIER_FN_GETLASTERROR("recv");

            buf += received;
            byteCount -= received;
        }

        return fm::Result();
    }
	
	fm::Result Socket::recvFrom(Message &msg,IpAddress &sourceIp)
    {
		msg.clear();
		
		fm::Size s;
		if (fm::Error e = recvFrom(&s,sizeof(s),sourceIp))
		{
			return e;
		}
		
		s = fn::ntohc(s);
		
		*((fm::Size*)msg.getPtr()) = s;
		
		return recvFrom(msg.getPtr(),msg.getSize()-sizeof(fm::Size),sourceIp);
    }

    fm::Result Socket::recvFrom(void *data,fm::Size byteCount,IpAddress &sourceIp)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","recvFrom called on uninitialized socket","recvFrom",__FILE__,__LINE__);

        char *buf = (char*)data;

        while (byteCount)
        {
            socklen_t tmp = sourceIp.isIpv4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
            int received = ::recvfrom(m_id, buf, byteCount, 0, (sockaddr*)sourceIp.getData(), &tmp);

            if (received == FRONTIER_SOCKET_ERROR)
                return FRONTIER_FN_GETLASTERROR("recvfrom");

            buf += received;
            byteCount -= received;
        }

        return fm::Result();
    }

    SocketID Socket::getID() const
    {
        return m_id;
    }
}


