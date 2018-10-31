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

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> vector2<T>::unsign() const
	{
		return vector2<T>(x < T(0) ? x*T(-1) : x,
						  y < T(0) ? y*T(-1) : y);
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
		T lth = LENGTH();
		return (lth==T(1) || lth==T(0)) ? *this : (*this)/std::sqrt(lth);
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
		T lth = LENGTH();
		return (lth==1 || lth==0) ? *this : (*this)/std::sqrt(lth);
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
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline T vector2<T>::area() const
	{
		return w*h;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline T vector2<T>::aspect() const
	{
		return w/h;
	}
	
	////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector2<T>::min()
	{
		return x<y ? x : y;
	}

	////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector2<T>::max()
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
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator+(const vector2<T> &left,const vector2<T2> &right) -> vector2<RETTYPE(T,T2,+)>
	{
		return vector2<RETTYPE(T,T2,+)>(left.x+right.x,
												 left.y+right.y);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator+(const vector2<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,+)>
	{
		return vector3<RETTYPE(T,T2,+)>(left.x+right.x,
												 left.y+right.y,
												 right.z);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator-(const vector2<T> &left,const vector2<T2> &right) -> vector2<RETTYPE(T,T2,-)>
	{
		return vector2<RETTYPE(T,T2,-)>(left.x-right.x,
												 left.y-right.y);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator-(const vector2<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,-)>
	{
		return vector3<RETTYPE(T,T2,-)>(left.x-right.x,
												 left.y-right.y,
												 right.z);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator*(const vector2<T> &left,const vector2<T2> &right) -> vector2<RETTYPE(T,T2,*)>
	{
		return vector2<RETTYPE(T,T2,*)>(left.x*right.x,
												 left.y*right.y);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator*(const vector2<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,*)>
	{
		return vector3<RETTYPE(T,T2,*)>(left.x*right.x,
												 left.y*right.y,
												 right.z);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator/(const vector2<T> &left,const vector2<T2> &right) -> vector2<RETTYPE(T,T2,/)>
	{
		return vector2<RETTYPE(T,T2,/)>(left.x/right.x,
												 left.y/right.y);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator/(const vector2<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,/)>
	{
		return vector3<RETTYPE(T,T2,/)>(left.x/right.x,
												 left.y/right.y,
												 right.z);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator%(const vector2<T> &left,const vector2<T2> &right) -> vector2<RETTYPE(T,T2,+)>
	{
		return vector2<RETTYPE(T,T2,+)>(std::fmod(left.x,right.x),
												 std::fmod(left.y,right.y));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline auto operator%(const vector2<T> &left,const vector3<T2> &right) -> vector3<RETTYPE(T,T2,+)>
	{
		return vector3<RETTYPE(T,T2,+)>(std::fmod(left.x,right.x),
												 std::fmod(left.y,right.y),
												 right.z);
	}
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector2<T> &left,const T2 &right) -> vector2<RETTYPE(T,T2,*)>
	{
		return vector2<RETTYPE(T,T2,*)>(left.x*right,
									    left.y*right);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const T2 &left,const vector2<T> &right) -> vector2<RETTYPE(T,T2,*)>
	{
		return vector2<RETTYPE(T,T2,*)>(left*right.x,
									    left*right.y);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const vector2<T> &left,const T2 &right) -> vector2<RETTYPE(T,T2,/)>
	{
		return vector2<RETTYPE(T,T2,/)>(left.x/right,
									    left.y/right);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator%(const vector2<T> &left,const T2 &right) -> vector2<RETTYPE(T,T2,%)>
	{
		return vector2<RETTYPE(T,T2,%)>(std::fmod(left.x,right),
									    std::fmod(left.y,right));
	}
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */
	
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
	inline vector2<T> &operator+=(vector2<T> &left,const vector3<T2> &right)
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
	inline vector2<T> &operator-=(vector2<T> &left,const vector3<T2> &right)
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
	inline vector2<T> &operator*=(vector2<T> &left,const vector3<T2> &right)
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
	inline vector2<T> &operator/=(vector2<T> &left,const vector3<T2> &right)
	{
		left.x/=right.x;
		left.y/=right.y;
		return left;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector2<T> &operator%=(vector2<T> &left,const vector2<T2> &right)
	{
		left.x=std::fmod(left.x,right.x);
		left.y=std::fmod(left.y,right.y);
		return left;
	}
	
	
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A x= b                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator*=(vector2<T> &left,const T2 &right)
	{
		left.x*=right;
		left.y*=right;
		
		return left;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator/=(vector2<T> &left,const T2 &right)
	{
		left.x/=right;
		left.y/=right;
		
		return left;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	vector2<T> &operator%=(vector2<T> &left,const T2 &right)
	{
		left.x=std::fmod(left.x,right);
		left.y=std::fmod(left.y,right);
		
		return left;
	}

	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
					   A eq B                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator==(const vector2<T> &left,const vector2<T2> &right)
	{
		return left.x==right.x && left.y==right.y;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator==(const vector2<T> &left,const vector3<T2> &right)
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
	template<class T,class T2>
	inline bool operator!=(const vector2<T> &left,const vector3<T2> &right)
	{
		return left.x!=right.x || left.y!=right.y;
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * *\
					                          
						 x A                  
					                          
	\* * * * * * * * * * * * * * * * * * * * */

	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator-(const vector2<T> &vec) -> vector2<RETTYPE_UN(-,T)>
	{
		return vector2<RETTYPE_UN(-,T)>(-vec.x,
										-vec.y);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator+(const vector2<T> &vec) -> vector2<RETTYPE_UN(+,T)>
	{
		return vector2<RETTYPE_UN(+,T)>(+vec.x,
										+vec.y);
	}
}

#include <iosfwd>

template<class T,class CharT,class CharTraitT>
inline std::basic_ostream<CharT,CharTraitT> &operator<<(std::basic_ostream<CharT,CharTraitT> &out, const fm::vector2<T> &vec)
{
	return out<<vec.x<<' '<<vec.y;
}
template<class T,class CharT,class CharTraitT>
inline std::basic_istream<CharT,CharTraitT> &operator>>(std::basic_istream<CharT,CharTraitT> &in, fm::vector2<T> &vec)
{
	return in>>vec.x>>vec.y;
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
