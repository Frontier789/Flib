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
#include <FRONTIER/Network/BasicSocket.hpp>
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
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

namespace fn
{
	void setBlocking(SocketID id,bool blocking)
	{
		int status = fcntl(id, F_GETFL);
		
		if (blocking)
			fcntl(id, F_SETFL, status &~ O_NONBLOCK);
		else
			fcntl(id, F_SETFL, status  | O_NONBLOCK);
	}
}
#endif

namespace fn
{
    BasicSocket::BasicSocket() : m_id(FRONTIER_INVALID_SOCKET),
								 m_blocking(true),
								 m_ready(false)
    {

    }

    BasicSocket::~BasicSocket()
    {
        close();
    }

    fm::Result BasicSocket::create(bool ipv6,bool tcp)
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

    void BasicSocket::close()
    {
        if (isValid())
        {
            FRONTIER_CLOSE_SOCKET(m_id);

            m_id = FRONTIER_INVALID_SOCKET;
        }
    }

    void BasicSocket::shutdown(bool read,bool write)
    {
		if (isValid() && (read || write))
        {
			shutdown(m_id, read ? (write ? 2 : 0) : 1);
        }
    }

    fm::Result BasicSocket::connect(const IpAddress &ip)
    {
		if (!ip.isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized socket","connect",__FILE__,__LINE__);
		if (!isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized address","connect",__FILE__,__LINE__);

		if (::connect(m_id,(const sockaddr *)ip.getData(),ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)))
			return FRONTIER_FN_GETLASTERROR("connect");

        return fm::Result();
    }

    fm::Result BasicSocket::connect(const IpAddress &ip,fm::Uint16 port)
    {
		if (!ip.isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized socket","connect",__FILE__,__LINE__);
		if (!isValid()) return fm::Error("NetError","Uninitialized","connect called on uninitialized address","connect",__FILE__,__LINE__);

        IpAddress cpy = ip;
        cpy.setPort(port);

		if (::connect(m_id,(const sockaddr *)cpy.getData(),ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)))
			return FRONTIER_FN_GETLASTERROR("connect");

        return fm::Result();
    }

    fm::Result BasicSocket::bind(const IpAddress &ip)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","bind called on uninitialized socket","bind",__FILE__,__LINE__);
		if (!ip.isValid()) return fm::Error("NetError","Uninitialized","bind called on uninitialized address","bind",__FILE__,__LINE__);

        const sockaddr *addr = (const sockaddr *)ip.getData();

        if (::bind(m_id,addr,ip.isIpv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)) == FRONTIER_SOCKET_ERROR)
			return FRONTIER_FN_GETLASTERROR("bind");

        return fm::Result();
    }

    fm::Result BasicSocket::listen()
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","listen called on uninitialized socket","listen",__FILE__,__LINE__);
		
		if (::listen(m_id, SOMAXCONN))
			return FRONTIER_FN_GETLASTERROR("listen");
		
        return fm::Result();
    }

    fm::Result BasicSocket::accept(BasicSocket &soc) const
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","accept called on uninitialized socket","accept",__FILE__,__LINE__);

        soc.close();

        soc.m_id = ::accept(m_id,fm::nullPtr,fm::nullPtr); // CHECK_FOR_ERROR
		
		if (!soc.isValid())
			return FRONTIER_FN_GETLASTERROR("accept");
		
        return fm::Result();
    }

    bool BasicSocket::isValid() const
    {
        return m_id != FRONTIER_INVALID_SOCKET;
    }

    BasicSocket::operator bool() const
    {
        return isValid();
    }
        
	void BasicSocket::setBlocking(bool blocking)
	{
		fn::setBlocking(m_id,blocking);
		m_blocking = blocking;
	}
	
	bool BasicSocket::isBlocking() const
	{
		return m_blocking;
	}

    IpAddress BasicSocket::getRemoteAddress() const
    {
        if (!isValid()) return IpAddress::invalidIp();

        IpAddress ret;

        socklen_t len = sizeof(sockaddr_storage);

		if (getpeername(m_id,(sockaddr *)ret.getData(),&len))
			return IpAddress();
		
        return ret;
    }

    IpAddress BasicSocket::getLocalAddress() const
    {
        if (!isValid()) return IpAddress::invalidIp();

        IpAddress ret;

        socklen_t len = sizeof(sockaddr_storage);

		if (getsockname(m_id,(sockaddr *)ret.getData(),&len))
			return IpAddress();
		
        return ret;
    }

	fm::Result BasicSocket::send(const Message &msg)
    {
		msg.prepareSend();
		
		return send(msg.getPtr(),msg.getSize());
    }

    fm::Result BasicSocket::send(const void *data,fm::Uint32 byteCount)
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

	fm::Result BasicSocket::sendTo(const Message &msg,const IpAddress &targetIp)
	{
		msg.prepareSend();
		
		if (fm::Error err = sendTo(msg.getPtr(),sizeof(fm::Uint32),targetIp)) return err;
		
		return sendTo(msg.getDataPtr(),msg.getDataSize(),targetIp);
    }

    fm::Result BasicSocket::sendTo(const void *data,fm::Uint32 byteCount,const IpAddress &targetIp)
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
	
	fm::Result BasicSocket::recv(Message &msg)
    {
		msg.clear();
		
		fm::Uint32 s;
		if (fm::Error e = recv(&s,sizeof(s)))
		{
			return e;
		}
		
		*((fm::Uint32*)msg.getPtr()) = s;
		
		s = fn::ntohc(s);
		
		msg.append(fm::nullPtr,s-sizeof(s));
		
		return recv(msg.getDataPtr(),s-sizeof(s));
    }
	
    fm::Result BasicSocket::recv(void *data,fm::Uint32 byteCount)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","recv called on uninitialized socket","recv",__FILE__,__LINE__);

        char *buf = (char*)data;

        while (byteCount)
        {
            int received = ::recv(m_id, buf, byteCount, 0);

            if (received == FRONTIER_SOCKET_ERROR)
                return FRONTIER_FN_GETLASTERROR("recv");
               
			if (received == 0)
				return fm::Error("NetError","Closed","BasicSocket was closed on the other end","recv",__FILE__,__LINE__);

            buf += received;
            byteCount -= received;
        }

        return fm::Result();
    }
	
	fm::Result BasicSocket::recvFrom(Message &msg,fm::Ref<IpAddress> sourceIp)
    {
		msg.clear();
		
		fm::Uint32 s;
		if (fm::Error e = recvFrom(&s,sizeof(s),sourceIp))
		{
			return e;
		}
		
		s = fn::ntohc(s);
		
		*((fm::Uint32*)msg.getPtr()) = s;
		
		msg.append(fm::nullPtr,s-sizeof(s));
		
		return recvFrom(msg.getDataPtr(),s-sizeof(s),sourceIp);
    }

    fm::Result BasicSocket::recvFrom(void *data,fm::Uint32 byteCount,fm::Ref<IpAddress> sourceIp)
    {
		if (!isValid()) return fm::Error("NetError","Uninitialized","recvFrom called on uninitialized socket","recvFrom",__FILE__,__LINE__);

        char *buf = (char*)data;
        
		sockaddr_in tmpaddr;
		socklen_t tmpsize = 0;
		sockaddr *ptr = fm::nullPtr;
		
		if (sourceIp)
		{
			tmpsize = sourceIp->isIpv4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
			ptr = (sockaddr*)sourceIp->getData();
		}
		else
		{
			tmpsize = sizeof(sockaddr_in);
			ptr = (sockaddr*)&tmpaddr;
		}

		int received = ::recvfrom(m_id, buf, byteCount, 0, ptr, &tmpsize);

		if (received == FRONTIER_SOCKET_ERROR)
			return FRONTIER_FN_GETLASTERROR("recvfrom");

        return fm::Result();
    }
        
	bool BasicSocket::isReady() const
	{
		return m_ready;
	}
	
	void BasicSocket::setReady(bool ready)
	{
		m_ready = ready;
	}

    SocketID BasicSocket::getID() const
    {
        return m_id;
    }
}


