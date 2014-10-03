////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_RECT_INL_INCLUDED
#define FRONTIER_RECT_INL_INCLUDED
namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect() : pos(vector2<T>()),
							 size(vector2<T>())
	{
		
	}
	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect(const vector2<T> &pos) : pos(pos),
												  size(vector2<T>())
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect(const vector2<T> &pos,const vector2<T> &size) : pos(pos),
																		 size(size)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect(const T &x,const T &y,const vector2<T> &size) : pos(x,y),
																		 size(size)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect(const T &x,const T &y,const T &w,const T &h) : pos(x,y),
																 size(w,h)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T>::rect(const vector2<T> &pos,const T &w,const T &h) : pos(pos),
																		size(w,h)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline rect<T>::rect(const rect<T2> &copy) : pos(vector2<T>(copy.pos)),
												 size(vector2<T>(copy.size))
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool rect<T>::contains(const vector2<T> &p) const
	{
		return ((pos.x<=p.x && pos.x+size.w>=p.x) || (pos.x>=p.x && pos.x+size.w<=p.x)) && 
			   ((pos.y<=p.y && pos.y+size.h>=p.y) || (pos.y>=p.y && pos.y+size.h<=p.y));
	}

	/////////////////////////////////////////////////////////////
	
	namespace priv
	{
		template<class T>
		inline void makeRectSizeUnsigned(rect<T> &rct)
		{
			if (rct.size.w < T(0.0))
				rct.size.w*=T(-1.0),
				rct.pos.x-=rct.size.w;
			if (rct.size.h < T(0.0))
				rct.size.h*=T(-1.0),
				rct.pos.y-=rct.size.h;
		}
		template<class T>
		inline const T &max(const T &left,const T &right)
		{
			return left > right ? left : right;
		}
		template<class T>
		inline const T &min(const T &left,const T &right)
		{
			return left < right ? left : right;
		}
		template<class T>
		inline T abs(const T &val)
		{
			return val < T(0.0) ? val*T(-1.0) : val;
		}
	}
	template<class T>
	inline bool rect<T>::intersects(const rect<T> &other) const
	{
		rect<T> tmp1=*this,tmp2=other;
		makeRectSizeUnsigned(tmp1);
		makeRectSizeUnsigned(tmp2);
		return (priv::max(tmp1.pos.x,tmp2.pos.x) <= priv::min(tmp1.pos.x+tmp1.size.w,tmp2.pos.x+tmp2.size.w) &&
				priv::max(tmp1.pos.y,tmp2.pos.y) <= priv::min(tmp1.pos.y+tmp1.size.h,tmp2.pos.y+tmp2.size.h));
	}
	

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool rect<T>::intersects(const rect<T> &other,rect<T> &intersection) const
	{
		rect<T> tmp1=*this,tmp2=other;
		makeRectSizeUnsigned(tmp1);
		makeRectSizeUnsigned(tmp2);
		rect<T> ret(vec2(priv::max(tmp1.pos.x,tmp2.pos.x),
						 priv::max(tmp1.pos.y,tmp2.pos.y)),vec2(priv::min(tmp1.pos.x+tmp1.size.w,tmp2.pos.x+tmp2.size.w) - priv::max(tmp1.pos.x,tmp2.pos.x),
																priv::min(tmp1.pos.y+tmp1.size.h,tmp2.pos.y+tmp2.size.h) - priv::max(tmp1.pos.y,tmp2.pos.y)));
		if (ret.size.w >= 0 &&
			ret.size.h >= 0)
		{
			intersection = ret;
			return true;
		}
		return false;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline rect<T> &rect<T>::extend(const vector2<T> &p)
	{
		if (size.w > T(0.0))
		{
			if (p.x<pos.x)
				size.w += pos.x-p.x,
				pos.x=p.x;
			if (pos.x+size.w<p.x)
				size.w=p.x-pos.x;
		}
		else
		{
			if (p.x>pos.x)
				size.w += pos.x-p.x,
				pos.x=p.x;
			if (pos.x+size.w>p.x)
				size.w=p.x-pos.x;
		}
		if (size.h > T(0.0))
		{
			if (p.y<pos.y)
				size.h += pos.y-p.y,
				pos.y=p.y;
			if (pos.y+size.h<p.y)
				size.h=p.y-pos.y;
		}
		else
		{
			if (p.y>pos.y)
				size.h += pos.y-p.y,
				pos.y=p.y;
			if (pos.y+size.h>p.y)
				size.h=p.y-pos.y;
		}
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T rect<T>::area() const
	{
		return priv::abs(size.w*size.h);
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
}
#endif // FRONTIER_RECT_INL_INCLUDED
