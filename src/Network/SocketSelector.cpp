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
#include <FRONTIER/Network/SocketSelector.hpp>
#include <FRONTIER/Network/SocketPredef.hpp>
#include <FRONTIER/Network/Socket.hpp>
#include <FRONTIER/System/Math.hpp>
#include "fnGetLastError.hpp"

namespace fn
{
	SocketSelector::SocketSelector()
	{
		m_sockets.push_back(&m_stopperSoc);
	}
	
	//////////////////////////////////////////////////////////////////////////
	fm::Size SocketSelector::getMaxSize()
	{
		return FD_SETSIZE-1;
	}
	
	//////////////////////////////////////////////////////////////////////////
	SocketSelector &SocketSelector::addSocket(Socket &socket)
	{
		if (m_sockets.size() < FD_SETSIZE)
			m_sockets.push_back(&socket);
		
		return *this;
	}
		
	//////////////////////////////////////////////////////////////////////////
	SocketSelector &operator<<(SocketSelector &selector,Socket &socket)
	{
		return selector.addSocket(socket);
	}

	//////////////////////////////////////////////////////////////////////////
	SocketSelector &SocketSelector::remSocket(Socket &socket)
	{
		for (std::vector<Socket *>::iterator it = m_sockets.begin();it != m_sockets.end();++it)
		{
			if ((*it)->getID() == socket.getID())
			{
				m_sockets.erase(it);
				break;
			}
		}
		
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	fm::Result SocketSelector::select(const fm::Time &timeout,fm::Ref<fm::Size> readyCount)
	{
		fn::SocketID maxID = 0;
		fd_set fdSet;
		FD_ZERO(&fdSet);
		
		timeval *timeVal = fm::nullPtr;
		if (timeout >= fm::Time::Zero)
		{
			timeVal = new timeval;
			
			if (timeout == fm::Time::Zero)
			{
				timeVal->tv_sec = 0;
				timeVal->tv_usec = 0;
			}
			else
			{
				timeVal->tv_sec = fm::Size(timeout.s());
				timeVal->tv_usec = fm::Size(timeout.Ms() - timeout.s() * 1000 * 1000);
			}
		}
		
		if (fm::Error err = m_stopperSoc.bind(IpAddress::localHost()))
		{
			return err;
		}
		
		for (std::vector<Socket *>::const_iterator it = m_sockets.begin();it != m_sockets.end();++it)
		{
			(*it)->setReady(false);
			
			maxID = fm::max(maxID,(*it)->getID());
			
			FD_SET((*it)->getID(),&fdSet);
		}
		
		int readyNum = ::select(maxID + 1, &fdSet, fm::nullPtr, fm::nullPtr, timeVal);
		
		delete timeVal;
		
		if (readyNum < 0) 
		{
			return FRONTIER_FN_GETLASTERROR("select");
		}

		for (std::vector<Socket *>::const_iterator it = m_sockets.begin();it != m_sockets.end();++it)
		{
			if (FD_ISSET((*it)->getID(), &fdSet))
				(*it)->setReady(true);
		}
		
		if (FD_ISSET(m_stopperSoc.getID(), &fdSet))
		{
			readyNum--;
			if (readyCount) *readyCount = readyNum;
			
			char tmp;
			m_stopperSoc.recvFrom(&tmp,1);
			
			return fm::Error("NetError","SelectStopped","select was stopped by stop()","select",__FILE__,__LINE__);
		}
		else
		{
			if (readyCount) *readyCount = readyNum;
		}
		
		return fm::Result();
	}
	
	//////////////////////////////////////////////////////////////////////////
	void SocketSelector::stop()
	{
		IpAddress ip = IpAddress::localAddress();
		ip.setPort(m_stopperSoc.getLocalAddress().getPort());
		
		m_stopperSoc.sendTo("1",1,ip);
	}
	
	//////////////////////////////////////////////////////////////////////////
	void SocketSelector::clear()
	{
		m_sockets.resize(1);
	}
	
	//////////////////////////////////////////////////////////////////////////
	fm::Size SocketSelector::getSize()
	{
		return m_sockets.size()-1;
	}
}
