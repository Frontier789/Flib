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
#include <FRONTIER/Network/Endian.hpp>
#include <FRONTIER/System/Math.hpp>
#include <cstring>

namespace fn
{
	Message::Message() : m_data(sizeof(fm::Uint32)/sizeof(fm::Uint8),0),
						 m_readPos(0)
	{
		
	}

	void *Message::getPtr()
	{
		return &m_data[0];
	}
	
	const void *Message::getPtr() const
	{
		return &m_data[0];
	}

	fm::Uint8 *Message::getDataPtr()
	{
		return &m_data[sizeof(fm::Uint32)/sizeof(fm::Uint8)];
	}
	
	const fm::Uint8 *Message::getDataPtr() const
	{
		return &m_data[sizeof(fm::Uint32)/sizeof(fm::Uint8)];
	}
		
	fm::Uint32 Message::getSize() const
	{
		return m_data.size();
	}
		
	fm::Uint32 Message::getDataSize() const
	{
		return m_data.size() - sizeof(fm::Uint32)/sizeof(fm::Uint8);
	}
	
	void Message::prepareSend() const
	{
		fm::Uint8 *ptr = &m_data[0];
		
		fm::Uint32 *sptr = (fm::Uint32*)ptr;
		
		(*sptr) = fn::htonc(getSize());
	}
	
	void Message::clear()
	{
		m_data = std::vector<fm::Uint8>(sizeof(fm::Uint32)/sizeof(fm::Uint8),0);
	}
	
	void Message::swap(Message &msg)
	{
		m_data.swap(msg.m_data);
	}
		
	Message &Message::append(const void *ptr,fm::Uint32 byteCount)
	{
		if (byteCount)
		{
			m_data.resize(m_data.size()+byteCount);
			
			if (ptr)
				std::memcpy(&m_data[m_data.size()-byteCount],ptr,byteCount);			
		}
		
		return *this;
	}
		
	const Message &Message::read(void *ptr,fm::Uint32 byteCount) const
	{
		fm::Uint32 realRead = fm::min(getDataSize() - m_readPos,byteCount);
		
		std::memcpy(ptr,&m_data[sizeof(fm::Uint32)+m_readPos],realRead);
		
		m_readPos += realRead;
		
		return *this;	
	}
	
	Message &Message::read(void *ptr,fm::Uint32 byteCount)
	{
		((const Message *)this)->read(ptr,byteCount);
		
		return *this;
	}
	
	bool Message::eof() const
	{
		return m_readPos >= getDataSize();
	}
	
	Message &operator<<(Message &msg,char c)
	{
		return msg.append(&c,sizeof(c));
	}
	
	Message &operator<<(Message &msg,fm::Int8  num)
	{
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,fm::Int16 num)
	{
		num = fn::htonc(num);
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,fm::Int32 num)
	{
		num = fn::htonc(num);
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,fm::Int64 num)
	{
		num = fn::htonc(num);
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,fm::Uint8  num)
	{
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,fm::Uint16 num)
	{
		num = fn::htonc(num);
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,fm::Uint32 num)
	{
		num = fn::htonc(num);
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,fm::Uint64 num)
	{
		num = fn::htonc(num);
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,long double num)
	{
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,float  num)
	{
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,double num)
	{ 
		return msg.append(&num,sizeof(num));
	}
	
	Message &operator<<(Message &msg,const fm::String &str)
	{
		if (fn::littleEndian())
		{
			fm::String cpy = str;
			
			C(cpy.size()) cpy[i] = fn::htonc(cpy[i]);
			
			return msg.append(&cpy[0],sizeof(cpy[0])*cpy.size()) << fm::Uint32(0);
		}
		
		return msg.append(&str[0],sizeof(str[0])*str.size()) << fm::Uint32(0);
	}
	
	Message &operator<<(Message &msg,const char *str)
	{
		const char *ptr = str;
		while (*ptr++);
		
		return msg.append(str,(ptr-str)*sizeof(*str));
	}
	
	Message &operator<<(Message &msg,const wchar_t *str)
	{
		const wchar_t *ptr = str;
		while (*ptr++);
		
		fm::Size n = (ptr-str);
		
		if (fn::littleEndian() && sizeof(wchar_t) > 1)
		{
			wchar_t *cpy = new wchar_t[n];
			
			C(n) cpy[i] = fn::htonc(str[i]);
			
			msg.append(cpy,n*sizeof(*str));
			delete[] cpy;
			
			return msg;
		}
		
		return msg.append(str,n*sizeof(*str));
	}
	
	
	
	Message &operator>>(Message &msg,char &c)
	{
		return msg.read(&c,sizeof(c));
	}
	
	Message &operator>>(Message &msg,fm::Int8  &num)
	{
		return msg.read(&num,sizeof(num));
	}
	
	Message &operator>>(Message &msg,fm::Int16 &num)
	{
		msg.read(&num,sizeof(num));
		num = fn::ntohc(num);
		return msg;
	}
	
	Message &operator>>(Message &msg,fm::Int32 &num)
	{
		msg.read(&num,sizeof(num));
		num = fn::ntohc(num);
		return msg;
	}
	
	Message &operator>>(Message &msg,fm::Int64 &num)
	{
		msg.read(&num,sizeof(num));
		num = fn::ntohc(num);
		return msg;
	}
	
	Message &operator>>(Message &msg,fm::Uint8  &num)
	{
		return msg.read(&num,sizeof(num));
	}
	
	Message &operator>>(Message &msg,fm::Uint16 &num)
	{
		msg.read(&num,sizeof(num));
		num = fn::ntohc(num);
		return msg;
	}
	
	Message &operator>>(Message &msg,fm::Uint32 &num)
	{
		msg.read(&num,sizeof(num));
		num = fn::ntohc(num);
		return msg;
	}
	
	Message &operator>>(Message &msg,fm::Uint64 &num)
	{
		msg.read(&num,sizeof(num));
		num = fn::ntohc(num);
		return msg;
	}
	
	Message &operator>>(Message &msg,long double &num)
	{
		return msg.read(&num,sizeof(num));
	}
	
	Message &operator>>(Message &msg,float  &num)
	{
		return msg.read(&num,sizeof(num));
	}
	
	Message &operator>>(Message &msg,double &num)
	{ 
		return msg.read(&num,sizeof(num));
	}
	
	Message &operator>>(Message &msg,fm::String &str)
	{
		str.clear();
		
		while (!msg.eof())
		{
			fm::Uint32 c;
			msg >> c;
			
			if (c)
				str += c;
			else
				break;
		}
		
		return msg;
	}
}