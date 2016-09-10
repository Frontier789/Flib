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
#ifndef FRONTIER_MESSAGE_INL_INCLUDED
#define FRONTIER_MESSAGE_INL_INCLUDED
#include <FRONTIER/System/type_traits/Remove_cv.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/Network/Endian.hpp>

namespace fn
{
	template<class T>
	inline Message &operator>>(const typename fm::Identity<T>::type &t,Message &msg)
	{
		Message cpy;
		
		cpy.append(&t,sizeof(t));
		cpy.append(msg.getDataPtr(),msg.getDataSize());
		
		msg.swap(cpy);
		
		return msg;
	}
	
	
	template<class charT,class traits,class Alloc>
	inline Message &operator<<(Message &msg,const std::basic_string<charT,traits,Alloc> &str)
	{
		if (fn::littleEndian() && sizeof(charT) > 1)
		{
			std::basic_string<charT,traits,Alloc> cpy = str;
			
			C(cpy.size()) cpy[i] = fn::htonc(cpy[i]);
			
			return msg.append(&cpy[0],sizeof(cpy[0])*cpy.size()) << charT(0);
		}
		
		return msg.append(&str[0],sizeof(str[0])*str.size()) << charT(0);
	}
	
	template<class T,fm::Size N>
	inline Message &operator<<(Message &msg,const T (&t)[N])
	{
		C(N) msg << t[i];
		
		return msg;
	}
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_enum<T>::value,Message &>::type operator<<(Message &msg,const T &t)
	{
		return msg << (typename fm::Underlying_type<T>::type)t;
	}
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_container<T>::value,Message &>::type operator<<(Message &msg,const T &t)
	{
		msg << fm::Size(0);
		fm::Size sizeOffset = msg.getDataSize()-sizeof(fm::Size);
		
		fm::Size n = 0;
		
		for (typename T::const_iterator it = t.begin(); it != t.end(); ++it)
			msg << *it,
			++n;
		
		*(fm::Size*)(msg.getDataPtr()+sizeOffset) = fn::htonc(n);
		
		return msg;
	}
	
	template<class A,class B>
	inline Message &operator<<(Message &msg,const std::pair<A,B> &pr)
	{
		return msg << pr.first << pr.second;
	}
	
	
	template<class charT,class traits,class Alloc>
	inline Message &operator>>(Message &msg,std::basic_string<charT,traits,Alloc> &str)
	{
		str.clear();
		
		while (!msg.eof())
		{
			charT c;
			msg >> c;
			
			if (c)
				str += c;
			else
				break;
		}
		
		return msg;
	}
	
	template<class T,fm::Size N>
	inline Message &operator>>(Message &msg,T (&t)[N])
	{
		C(N) msg >> t[i];
		
		return msg;
	}
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_enum<T>::value,Message &>::type operator>>(Message &msg,T &t)
	{
		typename fm::Underlying_type<T>::type tmp;
		msg >> tmp;
		t = T(tmp);
		
		return msg;
	}
	
	template<class T>
	inline typename fm::Enable_if<fm::Is_container<T>::value,Message &>::type operator>>(Message &msg,T &t)
	{
		fm::Size n;
		msg >> n;
		
		t.clear();
		
		C(n) 
		{
			typename T::value_type val;
			msg >> val;
			
			t.insert(t.end(),val);
		}
		
		return msg;
	}
	
	template<class A,class B>
	inline Message &operator>>(Message &msg,std::pair<A,B> &pr)
	{
		return msg >> (typename fm::Remove_cv<A>::type &)pr.first >> (typename fm::Remove_cv<B>::type &)pr.second;
	}
}

#endif // FRONTIER_MESSAGE_INL_INCLUDED
