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
#ifndef FRONTIER_MESSAGE_HPP_INCLUDED
#define FRONTIER_MESSAGE_HPP_INCLUDED

#include <FRONTIER/System/type_traits/Underlying_type.hpp>
#include <FRONTIER/System/type_traits/Is_container.hpp>
#include <FRONTIER/System/type_traits/Enable_if.hpp>
#include <FRONTIER/System/type_traits/Identity.hpp>
#include <FRONTIER/System/type_traits/Is_enum.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/String.hpp>
#include <utility>
#include <vector>
#include <string>

namespace fn
{
	class FRONTIER_API Message
	{
		mutable std::vector<fm::Uint8> m_data;
		mutable fm::Uint32 m_readPos;
	public:
		
		Message();
		
		void *getPtr();
		const void *getPtr() const;
		fm::Uint8 *getDataPtr();
		const fm::Uint8 *getDataPtr() const;
		
		fm::Uint32 getSize() const;
		fm::Uint32 getDataSize() const;
		
		void prepareSend() const;
		
		void clear();
		void swap(Message &msg);
		
		Message &append(const void *ptr,fm::Uint32 byteCount);
		
		const Message &read(void *ptr,fm::Uint32 byteCount) const;
		Message &read(void *ptr,fm::Uint32 byteCount);
		
		bool eof() const;
	};
	
	Message &operator<<(Message &msg,char c);
	Message &operator<<(Message &msg,fm::Int8  num);
	Message &operator<<(Message &msg,fm::Int16 num);
	Message &operator<<(Message &msg,fm::Int32 num);
	Message &operator<<(Message &msg,fm::Int64 num);
	Message &operator<<(Message &msg,fm::Uint8  num);
	Message &operator<<(Message &msg,fm::Uint16 num);
	Message &operator<<(Message &msg,fm::Uint32 num);
	Message &operator<<(Message &msg,fm::Uint64 num);

	Message &operator<<(Message &msg,long double num);
	Message &operator<<(Message &msg,float  num);
	Message &operator<<(Message &msg,double num);
	
	
	template<class charT,class traits,class Alloc>
	inline Message &operator<<(Message &msg,const std::basic_string<charT,traits,Alloc> &str);
	
	Message &operator<<(Message &msg,const fm::String &str);
	Message &operator<<(Message &msg,const char *str);
	Message &operator<<(Message &msg,const wchar_t *str);
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_enum<T>::value,Message &>::type operator<<(Message &msg,const T &t);
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_container<T>::value,Message &>::type operator<<(Message &msg,const T &t);
	
	template<class A,class B>
	inline Message &operator<<(Message &msg,const std::pair<A,B> &pr);
	
	template<class T,fm::Uint32 N>
	inline Message &operator<<(Message &msg,const T (&t)[N]);
	
	
	template<class T>
	inline Message &operator>>(const typename fm::Identity<T>::type &t,Message &msg);
	
	
	
	
	
	
	
	
	Message &operator>>(Message &msg,char &c);
	Message &operator>>(Message &msg,fm::Int8  &num);
	Message &operator>>(Message &msg,fm::Int16 &num);
	Message &operator>>(Message &msg,fm::Int32 &num);
	Message &operator>>(Message &msg,fm::Int64 &num);
	Message &operator>>(Message &msg,fm::Uint8  &num);
	Message &operator>>(Message &msg,fm::Uint16 &num);
	Message &operator>>(Message &msg,fm::Uint32 &num);
	Message &operator>>(Message &msg,fm::Uint64 &num);

	Message &operator>>(Message &msg,long double &num);
	Message &operator>>(Message &msg,float  &num);
	Message &operator>>(Message &msg,double &num);
	
	
	template<class charT,class traits,class Alloc>
	inline Message &operator>>(Message &msg,std::basic_string<charT,traits,Alloc> &str);
	
	Message &operator>>(Message &msg,fm::String &str);
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_enum<T>::value,Message &>::type operator>>(Message &msg,T &t);
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_container<T>::value,Message &>::type operator>>(Message &msg,T &t);
	
	template<class A,class B>
	inline Message &operator>>(Message &msg,std::pair<A,B> &pr);
	
	template<class T,fm::Uint32 N>
	inline Message &operator>>(Message &msg,T (&t)[N]);
}

#endif // FRONTIER_MESSAGE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Network/Message.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
