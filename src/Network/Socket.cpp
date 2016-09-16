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
#include <FRONTIER/Network/Message.hpp>
#include <FRONTIER/Network/Socket.hpp>
#include <FRONTIER/Network/Endian.hpp>
#include <FRONTIER/System/NullPtr.hpp>

namespace fn
{
    fm::Result Socket::create(bool ipv6,bool tcp)
    {
		return getImpl().create(ipv6,tcp);
    }

    void Socket::close()
    {
        getImpl().close();
    }

    void Socket::shutdown(bool read,bool write)
    {
		getImpl().shutdown(read,write);
    }

    bool Socket::isValid() const
    {
        return getImpl().isValid();
    }

    Socket::operator bool() const
    {
        return isValid();
    }
        
	void Socket::setBlocking(bool blocking)
	{
		getImpl().setBlocking(blocking);
	}
	
	bool Socket::isBlocking() const
	{
		return getImpl().isBlocking();;
	}

    IpAddress Socket::getRemoteAddress() const
    {
		return getImpl().getRemoteAddress();
    }

    IpAddress Socket::getLocalAddress() const
    {
		return getImpl().getLocalAddress();
    }
        
	bool Socket::isReady() const
	{
		return getImpl().isReady();
	}
	
	void Socket::setReady(bool ready)
	{
		getImpl().setReady(ready);
	}
	
    SocketID Socket::getID() const
    {
        return getImpl().getID();
    }
        
	BasicSocket &Socket::getImpl()
	{
		return m_impl;
	}
	
	const BasicSocket &Socket::getImpl() const
	{
		return m_impl;
	}
}


