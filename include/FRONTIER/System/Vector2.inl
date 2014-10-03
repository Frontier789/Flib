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
#ifndef FRONTIER_VECTOR2_INL_INCLUDED
#define FRONTIER_VECTOR2_INL_INCLUDED
#include <cmath>
namespace fm
{

	template<class T>
	inline vector2<T>::vector2() : x(T()),
	                               y(T())
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T>::vector2(const T &XY) : x(XY),
	                                      	  y(XY)
	{

	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T>::vector2(const T &X,const T &Y) : x(X),
	                                      				y(Y)
	{

	}

	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T>::vector2(const T (&XY)[2]) : x(XY[0]),
	                                               y(XY[1])
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector2<T>::vector2(const vector2<T2> &other) : x(T(other.x)),
	                                                       y(T(other.y))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector2<T>::vector2(const vector3<T2> &other) : x(T(other.x)),
														   y(T(other.y))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector2<T>::vector2(const vector4<T2> &other) : x(T(other.x)),
	                                                       y(T(other.y))
	{

	}


	template<class T>
	inline typename vector2<T>::reference vector2<T>::unsign()
	{
		x = (x < T(0) ? x*T(-1) : x);
		y = (y < T(0) ? y*T(-1) : y);
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector2<T>::length() const
	{
		return std::sqrt(x*x+y*y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector2<T>::LENGTH() const
	{
		return x*x+y*y;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::norm() const
	{
		T lth=LENGTH();
		return (lth==1 || lth==0) ? *this : *this/std::sqrt(lth);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> &vector2<T>::normalize()
	{
		*this = norm();
		return  *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::sgn() const
	{
		T lth=LENGTH();
		return (lth==1 || lth==0) ? *this : *this/std::sqrt(lth);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::projTo(typename vector2<T>::const_reference other) const
	{
		return other * this->dot(other) / other.LENGTH();
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::reflectOn(typename vector2<T>::const_reference other) const
	{
		return other * this->dot(other) / other.LENGTH() * T(2) - (*this);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::inv() const
	{
		return vector2<T>(T(1)/x,T(1)/y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::byComp(typename vector2<T>::const_reference other) const
	{
		return vector2<T>(x * other.x,
						  y * other.y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::byComp(const T &X,const T &Y) const
	{
		return vector2<T>(x * X,
						  y * Y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::byCompInv(typename vector2<T>::const_reference other) const
	{
		return vector2<T>(x / other.x,
						  y / other.y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::byCompInv(const T &X,const T &Y) const
	{
		return vector2<T>(x / X,
						  y / Y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector2<T>::dot(typename vector2<T>::const_reference other) const
	{
		return x*other.x + y*other.y;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::perp() const
	{
		return vector2<T>(y*T(-1),x);
	}


	template<class T>
	template<class T2>
	inline vector2<T> vector2<T>::load(const T2 &other)
	{
		return vector2<T>(other.x,other.y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector2<T> vector2<T>::loadxy(const T2 &other)
	{
		return vector2<T>(other.x,other.y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector2<T> vector2<T>::loadwh(const T2 &other)
	{
		return vector2<T>(other.w,other.h);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector2<T>::convert() const
	{
		return T2(x,y);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector2<T>::convertxy() const
	{
		T2 ret;
		ret.x = x;
		ret.y = y;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector2<T>::convertwh() const
	{
		T2 ret;
		ret.w = x;
		ret.h = y;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector2<T>::minxy()
	{
		return x<y ? x : y;
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline const T &vector2<T>::minxy() const
	{
		return x<y ? x : y;
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector2<T>::minwh()
	{
		return x<y ? x : y;
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline const T &vector2<T>::minwh() const
	{
		return x<y ? x : y;
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector2<T>::maxxy()
	{
		return x>y ? x : y;
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline const T &vector2<T>::maxxy() const
	{
		return x>y ? x : y;
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector2<T>::maxwh()
	{
		return x>y ? x : y;
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline const T &vector2<T>::maxwh() const
	{
		return x>y ? x : y;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline typename vector2<T>::reference vector2<T>::operator()(const T &X,const T &Y)
	{
		x = X;
		y = Y;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline typename vector2<T>::reference vector2<T>::operator()(typename vector2<T>::const_reference other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector2<T>::operator[](unsigned int index)
	{
		return *((T*)this+index);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline const T &vector2<T>::operator[](unsigned int index) const
	{
		return *((T*)this+index);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> &vector2<T>::operator*=(const T &scalar)
	{
		x*=scalar;
		y*=scalar;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> &vector2<T>::operator/=(const T &scalar)
	{
		x/=scalar;
		y/=scalar;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::operator/(const T &scalar) const
	{
		return vector2<T>(x/scalar,
						  y/scalar);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::operator*(const T &scalar) const
	{
		return vector2<T>(x*scalar,
						  y*scalar);
	}
}

namespace std
{
    template<typename,typename>
    class basic_ostream;
    template<typename,typename>
    class basic_istream;
    template<typename>
    class char_traits;
}

template<class T>
inline std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out, const fm::vector2<T> &vec)
{
	return out<<vec.x<<' '<<vec.y;
}

template<class T>
inline std::basic_istream<char, std::char_traits<char> > &operator>>(std::basic_istream<char, std::char_traits<char> > &in, fm::vector2<T> &vec)
{
	return in>>vec.x>>vec.y;
}
namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> &operator+=(vector2<T> &left,const vector2<T2> &right)
	{
		left.x+=right.x;
		left.y+=right.y;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> &operator-=(vector2<T> &left,const vector2<T2> &right)
	{
		left.x-=right.x;
		left.y-=right.y;
		return left;
	}
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> &operator*=(vector2<T> &left,const vector2<T2> &right)
	{
		left.x*=right.x;
		left.y*=right.y;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> &operator/=(vector2<T> &left,const vector2<T2> &right)
	{
		left.x/=right.x;
		left.y/=right.y;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> operator+(const vector2<T> &left,const vector2<T2> &right)
	{
		return vector2<T>(T(left.x+right.x),
						  T(left.y+right.y));
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> operator-(const vector2<T> &left,const vector2<T2> &right)
	{
		return vector2<T>(T(left.x-right.x),
						  T(left.y-right.y));
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> operator*(const vector2<T> &left,const vector2<T2> &right)
	{
		return vector2<T>(T(left.x*right.x),
						  T(left.y*right.y));
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> operator/(const vector2<T> &left,const vector2<T2> &right)
	{
		return vector2<T>(T(left.x/right.x),
						  T(left.y/right.y));
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator==(const vector2<T> &left,const vector2<T2> &right)
	{
		return left.x==right.x && left.y==right.y;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator!=(const vector2<T> &left,const vector2<T2> &right)
	{
		return left.x!=right.x || left.y!=right.y;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> operator*(const T &left,const vector2<T> &right)
	{
		return vector2<T>(left*right.x,
						  left*right.y);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> operator-(const vector2<T> &vec)
	{
		return vector2<T>(vec.x*T(-1),
						  vec.y*T(-1));
	}

}
namespace std
{
	template<class T>
	inline T abs(const fm::vector2<T> &vec)
	{
		return vec.length();
	}

	template<class T>
	fm::vector2<T> sgn(const fm::vector2<T> &vec)
	{
		return vec.sgn();
	}
}
#endif
