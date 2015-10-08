////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_VECTOR3_INL_INCLUDED
#define FRONTIER_VECTOR3_INL_INCLUDED
#include <FRONTIER/System/Math.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T>::vector3() : x(T()),
	                               y(T()),
	                               z(T())
	{

	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T>::vector3(const T &XYZ) : x(XYZ),
											   y(XYZ),
											   z(XYZ)
	{
		
	}										   

	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T>::vector3(const T &X,const T &Y,const T &Z) : x(X),
											  					   y(Y),
											  					   z(Z)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T>::vector3(const T (&XYZ)[3]) : x(XYZ[0]),
												    y(XYZ[1]),
												    z(XYZ[2])
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector3<T>::vector3(const vector3<T2> &other) : x(T(other.x)),
	                                                       y(T(other.y)),
	                                                       z(T(other.z))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector3<T>::vector3(const vector2<T2> &other) : x(T(other.x)),
														   y(T(other.y)),
														   z(T(0))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2,class T3>
	inline vector3<T>::vector3(const vector2<T2> &other,const T3 &Z) : x(T(other.x)),
																	   y(T(other.y)),
																	   z(T(Z))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector3<T>::vector3(const vector4<T2> &other) : x(T(other.x)),
	                                                       y(T(other.y)),
	                                                       z(T(other.z))
	{

	}

	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::unsign()
	{
		return vector3<T>(x < T(0) ? x*T(-1) : x,
						  y < T(0) ? y*T(-1) : y,
						  z < T(0) ? z*T(-1) : z);
		
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector3<T>::length() const
	{
		return fm::sqrt3(x*x+y*y+z*z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector3<T>::LENGTH() const
	{
		return x*x+y*y+z*z;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::norm() const
	{
		T lth=LENGTH();
		return (lth==1 || lth==0) ? *this : (*this)*fm::invsqrt3(lth);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> &vector3<T>::normalize()
	{
		*this = norm();
		return  *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::sgn() const
	{
		T lth=LENGTH();
		return (lth==1 || lth==0) ? *this : (*this)*fm::invsqrt3(lth);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::cross(typename vector3<T>::const_reference other) const
	{
		return vector3<T>(y*other.z - z*other.y,
                          z*other.x - x*other.z,
                          x*other.y - y*other.x);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::projTo(typename vector3<T>::const_reference other) const
	{
		return other * this->dot(other) / other.LENGTH();
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::projToPlane(typename vector3<T>::const_reference normal) const
	{
		return *this - normal * this->dot(normal) / normal.LENGTH();
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::reflectOn(typename vector3<T>::const_reference other) const
	{
		return other * this->dot(other) / other.LENGTH() * T(2) - (*this);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::inv() const
	{
		return vector3<T>(T(1)/x,T(1)/y,T(1)/z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::byComp(typename vector3<T>::const_reference other) const
	{
		return vector3<T>(x * other.x,
						  y * other.y,
						  z * other.z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::byComp(const T &X,const T &Y,const T &Z) const
	{
		return vector3<T>(x * X,
						  y * Y,
						  z * Z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::byCompInv(typename vector3<T>::const_reference other) const
	{
		return vector3<T>(x / other.x,
						  y / other.y,
						  z / other.z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> vector3<T>::byCompInv(const T &X,const T &Y,const T &Z) const
	{
		return vector3<T>(x / X,
						  y / Y,
						  z / Z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector3<T>::dot(typename vector3<T>::const_reference other) const
	{
		return x*other.x + y*other.y + z*other.z;
	}


	template<class T>
	template<class T2>
	inline vector3<T> vector3<T>::load(const T2 &other)
	{
		return vector3<T>(other.x,other.y,other.z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector3<T> vector3<T>::loadxyz(const T2 &other)
	{
		return vector3<T>(other.x,other.y,other.z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector3<T> vector3<T>::loadrgb(const T2 &other)
	{
		return vector3<T>(other.r,other.g,other.b);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector3<T>::convert() const
	{
		return T2(x,y,z);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector3<T>::convertxyz() const
	{
		T2 ret;
		ret.x = x;
		ret.y = y;
		ret.z = z;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector3<T>::convertrgb() const
	{
		T2 ret;
		ret.r = r;
		ret.g = g;
		ret.b = b;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline typename vector3<T>::reference vector3<T>::operator()(const T &X,const T &Y,const T &Z)
	{
		x = X;
		y = Y;
		z = Z;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline typename vector3<T>::reference vector3<T>::operator()(typename vector3<T>::const_reference other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector3<T>::operator[](unsigned int index)
	{
		return *((T*)this+index);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline const T &vector3<T>::operator[](unsigned int index) const
	{
		return *((T*)this+index);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> &vector3<T>::operator*=(const T &scalar)
	{
		x*=scalar;
		y*=scalar;
		z*=scalar;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> &vector3<T>::operator/=(const T &scalar)
	{
		x/=scalar;
		y/=scalar;
		z/=scalar;
		return *this;
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
inline std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out, const fm::vector3<T> &vec)
{
	return out<<vec.x<<' '<<vec.y<<' '<<vec.z;
}

template<class T>
inline std::basic_istream<char, std::char_traits<char> > &operator>>(std::basic_istream<char, std::char_traits<char> > &in, fm::vector3<T> &vec)
{
	return in>>vec.x>>vec.y>>vec.z;
}

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<T> &operator+=(vector3<T> &left,const vector3<T2> &right)
	{
		left.x+=right.x;
		left.y+=right.y;
		left.z+=right.z;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<T> &operator-=(vector3<T> &left,const vector3<T2> &right)
	{
		left.x-=right.x;
		left.y-=right.y;
		left.z-=right.z;
		return left;
	}
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<T> &operator*=(vector3<T> &left,const vector3<T2> &right)
	{
		left.x*=right.x;
		left.y*=right.y;
		left.z*=right.z;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<T> &operator/=(vector3<T> &left,const vector3<T2> &right)
	{
		left.x/=right.x;
		left.y/=right.y;
		left.z/=right.z;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,+)> operator+(const vector3<T> &left,const vector3<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,+) retT;
		return vector3<retT>(left.x+right.x,
							 left.y+right.y,
							 left.z+right.z);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,-)> operator-(const vector3<T> &left,const vector3<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,-) retT;
		return vector3<retT>(left.x-right.x,
							 left.y-right.y,
							 left.z-right.z);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector3<T> &left,const vector3<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,*) retT;
		return vector3<retT>(left.x*right.x,
							 left.y*right.y,
							 left.z*right.z);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector3<T> &left,const vector3<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,/) retT;
		return vector3<retT>(left.x/right.x,
							 left.y/right.y,
							 left.z/right.z);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector3<T> &left,const T2 &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,*) retT;
		return vector3<retT>(left.x*right,
							 left.y*right,
							 left.z*right);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector3<T> &left,const T2 &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,/) retT;
		return vector3<retT>(left.x/right,
							 left.y/right,
							 left.z/right);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector3<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const T &left,const vector3<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,*) retT;
		return vector3<retT>(left*right.x,
							 left*right.y,
							 left*right.z);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator==(const vector3<T> &left,const vector3<T2> &right)
	{
		return left.x==right.x && left.y==right.y && left.z==right.z;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator!=(const vector3<T> &left,const vector3<T2> &right)
	{
		return left.x!=right.x || left.y!=right.y || left.z!=right.z;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> operator-(const vector3<T> &vec)
	{
		return vector3<T>(vec.x*T(-1),
						  vec.y*T(-1),
						  vec.z*T(-1));
	}
}
namespace std
{
	template<class T>
	inline T abs(const fm::vector3<T> &vec)
	{
		return vec.length();
	}

	template<class T>
	fm::vector3<T> sgn(const fm::vector3<T> &vec)
	{
		return vec.sgn();
	}
}
#endif
