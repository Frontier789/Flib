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
#ifndef FRONTIER_BOX_INL_INCLUDED
#define FRONTIER_BOX_INL_INCLUDED
#include <cmath>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline box<T>::box() : 
		pos(vector3<T>()),
		size(vector3<T>())
	{

	}
	/////////////////////////////////////////////////////////////
	template<class T>
	inline box<T>::box(const T &x,const T &y,const T &z,const T &w,const T &h,const T &d) :
		pos(x,y,z),
		size(w,h,d)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline box<T>::box(const vector3<T> &pos,const vector3<T> &size) : 
		pos(pos),
		size(size)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline box<T>::box(const box<T2> &copy) : 
		pos(vector3<T>(copy.pos)),
		size(vector3<T>(copy.size))
	{

	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool box<T>::contains(const vector3<T> &p) const
	{
		return pos.x <= p.x && pos.y <= p.y && pos.z <= p.z && 
		       pos.x + size.w >= p.x && pos.y + size.h >= p.y && pos.z + size.d >= p.z;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool box<T>::operator==(const box<T> &other) const
	{
		return pos==other.pos && size==other.size;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool box<T>::operator!=(const box<T> &other) const
	{
		return pos!=other.pos || size!=other.size;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline box<T> &box<T>::expand(const vector3<T> &p)
	{
		if (pos.x > p.x) size.w += pos.x - p.x, pos.x = p.x;
		if (pos.y > p.y) size.h += pos.y - p.y, pos.y = p.y;
		if (pos.z > p.z) size.d += pos.z - p.z, pos.z = p.z;
		
		if (pos.x + size.w < p.x) size.w = p.x - pos.x;
		if (pos.y + size.h < p.y) size.h = p.y - pos.y;
		if (pos.z + size.d < p.z) size.d = p.z - pos.z;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> box<T>::mid() const
	{
		return pos + size / T(2);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool box<T>::intersects(const box<T> &r) const
	{
		return pos.x < r.pos.x + r.size.w && pos.x + size.w > r.pos.x &&
    		   pos.y < r.pos.y + r.size.h && pos.y + size.h > r.pos.y &&
    		   pos.z < r.pos.z + r.size.d && pos.z + size.d > r.pos.z;
	}
}

#include <iosfwd>

template<class T,class CharT,class CharTraitT>
inline std::basic_ostream<CharT,CharTraitT> &operator<<(std::basic_ostream<CharT,CharTraitT> &out, const fm::box<T> &rct)
{
	return out<<rct.pos<<' '<<rct.size;
}

template<class T,class CharT,class CharTraitT>
inline std::basic_istream<CharT,CharTraitT> &operator>>(std::basic_istream<CharT,CharTraitT> &in, fm::box<T> &rct)
{
	return in>>rct.pos>>rct.size;
}

#endif // FRONTIER_RECT_INL_INCLUDED
