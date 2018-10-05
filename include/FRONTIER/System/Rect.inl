////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_RECT_INL_INCLUDED
#define FRONTIER_RECT_INL_INCLUDED
#include <cmath>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect() : 
		pos(vector2<T>()),
		size(vector2<T>())
	{

	}
	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect(const T &x,const T &y,const T &w,const T &h) :
		pos(x,y),
		size(w,h)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect(const vector2<T> &pos,const vector2<T> &size) : 
		pos(pos),
		size(size)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline rect<T>::rect(const rect<T2> &copy) : 
		pos(vector2<T>(copy.pos)),
		size(vector2<T>(copy.size))
	{

	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool rect<T>::contains(const vector2<T> &p) const
	{
		return pos.x <= p.x && pos.y <= p.y && pos.x + size.w >= p.x && pos.y + size.h >= p.y;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool rect<T>::operator==(const rect<T> &other) const
	{
		return pos==other.pos && size==other.size;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool rect<T>::operator!=(const rect<T> &other) const
	{
		return pos!=other.pos || size!=other.size;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T> &rect<T>::expand(const vector2<T> &p)
	{
		if (pos.x > p.x) size.w += pos.x - p.x, pos.x = p.x;
		if (pos.y > p.y) size.h += pos.y - p.y, pos.y = p.y;
		if (pos.x + size.w < p.x) size.w = p.x - pos.x;
		if (pos.y + size.h < p.y) size.h = p.y - pos.y;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> rect<T>::mid() const
	{
		return pos + size / T(2);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool rect<T>::intersects(const rect<T> &r) const
	{
		return pos.x < r.pos.x + r.size.w && pos.x + size.w > r.pos.x &&
    		   pos.y < r.pos.y + r.size.h && pos.y + size.h > r.pos.y;
	}
}

#include <iosfwd>

template<class T,class CharT,class CharTraitT>
inline std::basic_ostream<CharT,CharTraitT> &operator<<(std::basic_ostream<CharT,CharTraitT> &out, const fm::rect<T> &rct)
{
	return out<<rct.pos<<' '<<rct.size;
}

template<class T,class CharT,class CharTraitT>
inline std::basic_istream<CharT,CharTraitT> &operator>>(std::basic_istream<CharT,CharTraitT> &in, fm::rect<T> &rct)
{
	return in>>rct.pos>>rct.size;
}

#endif // FRONTIER_RECT_INL_INCLUDED
