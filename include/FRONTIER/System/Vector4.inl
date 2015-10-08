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
#ifndef FRONTIER_VECTOR4_INL_INCLUDED
#define FRONTIER_VECTOR4_INL_INCLUDED
#include <FRONTIER/System/Math.hpp>

namespace fm
{
	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T>::vector4() : x(T()),
	                               y(T()),
	                               z(T()),
	                               w(T(1))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T>::vector4(const T &XYZ,const T &W) : x(XYZ),
														  y(XYZ),
														  z(XYZ),
														  w(W)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T>::vector4(const T &X,const T &Y,const T &Z,const T &W) : x(X),
												  							  y(Y),
												  							  z(Z),
												  							  w(W)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T>::vector4(const T (&XYZW)[4]) : x(XYZW[0]),
													 y(XYZW[1]),
												     z(XYZW[2]),
												     w(XYZW[3])
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T>::vector4(const T (&XYZ)[3],const T &W) : x(XYZ[0]),
															   y(XYZ[1]),
															   z(XYZ[2]),
															   w(W)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector4<T>::vector4(const vector2<T2> &vec) : x(T(vec.x)),
														 y(T(vec.y)),
														 z(T(0)),
														 w(T(1))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2,class T3>
	inline vector4<T>::vector4(const vector2<T2> &vec,const T3 &Z) : x(T(vec.x)),
														             y(T(vec.y)),
														             z(T(Z)),
														             w(T(1))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2,class T3,class T4>
	inline vector4<T>::vector4(const vector2<T2> &vec,const T3 &Z,const T4 &W) : x(T(vec.x)),
														                         y(T(vec.y)),
														                         z(T(Z)),
														                         w(T(W))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector4<T>::vector4(const vector3<T2> &vec) : x(T(vec.x)),
														 y(T(vec.y)),
														 z(T(vec.z)),
														 w(T(1))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2,class T3>
	inline vector4<T>::vector4(const vector3<T2> &vec,const T3 &W) : x(T(vec.x)),
																	 y(T(vec.y)),
																	 z(T(vec.z)),
																	 w(T(W))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector4<T>::vector4(const vector4<T2> &vec) : x(T(vec.x)),
	                                                     y(T(vec.y)),
	                                                     z(T(vec.z)),
	                                                     w(T(vec.w))
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::unsign()
	{
		return vector4<T>(x < T(0) ? x*T(-1) : x,
						  y < T(0) ? y*T(-1) : y,
						  z < T(0) ? z*T(-1) : z,
						  w < T(0) ? w*T(-1) : w);
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector4<T>::length() const
	{
		return fm::sqrt3(x*x+y*y+z*z+w*w);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T vector4<T>::LENGTH() const
	{
		return x*x+y*y+z*z+w*w;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::norm() const
	{
		T lth=LENGTH();
		return (lth==1 || lth==0) ? *this : (*this)*fm::invsqrt3(lth);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> &vector4<T>::normalize()
	{
		*this = norm();
		return  *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::sgn() const
	{
		T lth=LENGTH();
		return (lth==1 || lth==0) ? *this : (*this)*fm::invsqrt3(lth);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::inv() const
	{
		return vector4<T>(T(1)/x,T(1)/y,T(1)/z,T(1)/w);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::byComp(typename vector4<T>::const_reference vec) const
	{
		return vector4<T>(x * vec.x,
						  y * vec.y,
						  z * vec.z,
						  w * vec.w);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::byComp(const T &X,const T &Y,const T &Z,const T &W) const
	{
		return vector4<T>(x * X,
						  y * Y,
						  z * Z,
						  w * W);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::byCompInv(typename vector4<T>::const_reference vec) const
	{
		return vector4<T>(x / vec.x,
						  y / vec.y,
						  z / vec.z,
						  w / vec.w);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::byCompInv(const T &X,const T &Y,const T &Z,const T &W) const
	{
		return vector4<T>(x / X,
						  y / Y,
						  z / Z,
						  w / W);
	}


	template<class T>
	template<class T2>
	inline vector4<T> vector4<T>::load(const T2 &vec)
	{
		return vector4<T>(vec.x,vec.y,vec.z,vec.w);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector4<T> vector4<T>::loadxyzw(const T2 &vec)
	{
		return vector4<T>(vec.x,vec.y,vec.z,vec.w);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline vector4<T> vector4<T>::loadrgba(const T2 &vec)
	{
		return vector4<T>(vec.r,vec.g,vec.b,vec.a);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector4<T>::convert() const
	{
		return T2(x,y,z,w);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector4<T>::convertxyzw() const
	{
		T2 ret;
		ret.x = x;
		ret.y = y;
		ret.z = z;
		ret.w = w;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline T2 vector4<T>::convertrgba() const
	{
		T2 ret;
		ret.r = r;
		ret.g = g;
		ret.b = b;
		ret.a = a;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> vector4<T>::fromHex(unsigned int hex)
	{
		return vector4<T>( *(((unsigned char*)&hex)+3) / 255.f,
						   *(((unsigned char*)&hex)+2) / 255.f,
						   *(((unsigned char*)&hex)+1) / 255.f,
						   *(((unsigned char*)&hex)+0) / 255.f );
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline unsigned int vector4<T>::toHex() const
	{
		unsigned int ret;
		(((unsigned char*)&ret)+3) = x*255.f;
		(((unsigned char*)&ret)+2) = y*255.f;
		(((unsigned char*)&ret)+1) = z*255.f;
		(((unsigned char*)&ret)+0) = w*255.f;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline typename vector4<T>::reference vector4<T>::operator()(const T &X,const T &Y,const T &Z)
	{
		x = X;
		y = Y;
		z = Z;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline typename vector4<T>::reference vector4<T>::operator()(const T &X,const T &Y,const T &Z,const T &W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline typename vector4<T>::reference vector4<T>::operator()(typename vector4<T>::const_reference vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T &vector4<T>::operator[](unsigned int index)
	{
		return *((T*)this+index);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline const T &vector4<T>::operator[](unsigned int index) const
	{
		return *((T*)this+index);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> &vector4<T>::operator*=(const T &scalar)
	{
		x*=scalar;
		y*=scalar;
		z*=scalar;
		w*=scalar;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> &vector4<T>::operator/=(const T &scalar)
	{
		x/=scalar;
		y/=scalar;
		z/=scalar;
		w/=scalar;
		return *this;
	}
	
	template<class T> vector4<T> vector4<T>::White     = vector4<T>::fromHex(0xFFFFFFFF);
	template<class T> vector4<T> vector4<T>::Silver    = vector4<T>::fromHex(0xBFBFBFFF);
	template<class T> vector4<T> vector4<T>::Gray      = vector4<T>::fromHex(0x7B7B7BFF);
	template<class T> vector4<T> vector4<T>::Grey      = vector4<T>::fromHex(0x7B7B7BFF);
	template<class T> vector4<T> vector4<T>::Black     = vector4<T>::fromHex(0x000000FF);
	template<class T> vector4<T> vector4<T>::Red       = vector4<T>::fromHex(0xFF0000FF);
	template<class T> vector4<T> vector4<T>::Brown     = vector4<T>::fromHex(0x7B4000FF);
	template<class T> vector4<T> vector4<T>::Yellow    = vector4<T>::fromHex(0xFFFF00FF);
	template<class T> vector4<T> vector4<T>::Lime      = vector4<T>::fromHex(0x00FF00FF);
	template<class T> vector4<T> vector4<T>::Green     = vector4<T>::fromHex(0x218C21FF);
	template<class T> vector4<T> vector4<T>::Teal      = vector4<T>::fromHex(0x007B7BFF);
	template<class T> vector4<T> vector4<T>::Blue      = vector4<T>::fromHex(0x0000FFFF);
	template<class T> vector4<T> vector4<T>::Navy      = vector4<T>::fromHex(0x00007BFF);
	template<class T> vector4<T> vector4<T>::Purple    = vector4<T>::fromHex(0x8F00D6FF);
	template<class T> vector4<T> vector4<T>::Maroon    = vector4<T>::fromHex(0x7B0000FF);
	template<class T> vector4<T> vector4<T>::Pink      = vector4<T>::fromHex(0xFF00FFFF);
	template<class T> vector4<T> vector4<T>::Olive     = vector4<T>::fromHex(0x7B7B00FF);
	template<class T> vector4<T> vector4<T>::Orange    = vector4<T>::fromHex(0xFFA300FF);
	template<class T> vector4<T> vector4<T>::Aqua      = vector4<T>::fromHex(0x00FFFFFF);
	template<class T> vector4<T> vector4<T>::Bone      = vector4<T>::fromHex(0xF2F2E6FF);
	template<class T> vector4<T> vector4<T>::Tan       = vector4<T>::fromHex(0xD1B28CFF);
	template<class T> vector4<T> vector4<T>::Charcoal  = vector4<T>::fromHex(0x444444FF);
	template<class T> vector4<T> vector4<T>::Royal     = vector4<T>::fromHex(0x074C9EFF);
	template<class T> vector4<T> vector4<T>::Azure     = vector4<T>::fromHex(0x007FFFFF);
	template<class T> vector4<T> vector4<T>::Aquamarine= vector4<T>::fromHex(0x7FFFD2FF);
	template<class T> vector4<T> vector4<T>::Forest    = vector4<T>::fromHex(0x218921FF);
	template<class T> vector4<T> vector4<T>::Gold      = vector4<T>::fromHex(0xFFD600FF);
	template<class T> vector4<T> vector4<T>::Coral     = vector4<T>::fromHex(0xFF7F4FFF);
	template<class T> vector4<T> vector4<T>::Lavender  = vector4<T>::fromHex(0xB57FDBFF);
	template<class T> vector4<T> vector4<T>::Indigo    = vector4<T>::fromHex(0x490082FF);
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
inline std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out, const fm::vector4<T> &vec)
{
	return out<<vec.x<<' '<<vec.y<<' '<<vec.z<<' '<<vec.w;
}

template<class T>
inline std::basic_istream<char, std::char_traits<char> > &operator>>(std::basic_istream<char, std::char_traits<char> > &in, fm::vector4<T> &vec)
{
	return in>>vec.x>>vec.y>>vec.z>>vec.w;
}

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<T> &operator+=(vector4<T> &left,const vector4<T2> &right)
	{
		left.x+=right.x;
		left.y+=right.y;
		left.z+=right.z;
		left.w+=right.w;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<T> &operator-=(vector4<T> &left,const vector4<T2> &right)
	{
		left.x-=right.x;
		left.y-=right.y;
		left.z-=right.z;
		left.w-=right.w;
		return left;
	}
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<T> &operator*=(vector4<T> &left,const vector4<T2> &right)
	{
		left.x*=right.x;
		left.y*=right.y;
		left.z*=right.z;
		left.w*=right.w;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<T> &operator/=(vector4<T> &left,const vector4<T2> &right)
	{
		left.x/=right.x;
		left.y/=right.y;
		left.z/=right.z;
		left.w/=right.w;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,+)> operator+(const vector4<T> &left,const vector4<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,+) retT;
		return vector4<retT>(left.x+right.x,
						     left.y+right.y,
						     left.z+right.z,
						     left.w+right.w);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,-)> operator-(const vector4<T> &left,const vector4<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,-) retT;
		return vector4<retT>(left.x-right.x,
						     left.y-right.y,
						     left.z-right.z,
						     left.w-right.w);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector4<T> &left,const vector4<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,*) retT;
		return vector4<retT>(left.x*right.x,
						     left.y*right.y,
						     left.z*right.z,
						     left.w*right.w);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector4<T> &left,const vector4<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,/) retT;
		return vector4<retT>(left.x/right.x,
						     left.y/right.y,
						     left.z/right.z,
						     left.w/right.w);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const vector4<T> &left,const T2 &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,*) retT;
		return vector4<retT>(left.x*right,
						     left.y*right,
						     left.z*right,
						     left.w*right);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,/)> operator/(const vector4<T> &left,const T2 &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,/) retT;
		return vector4<retT>(left.x/right,
						     left.y/right,
						     left.z/right,
						     left.w/right);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline vector4<FRONTIER_FIND_RETURN_TYPE(T,T2,*)> operator*(const T &left,const vector4<T2> &right)
	{
		typedef FRONTIER_FIND_RETURN_TYPE(T,T2,*) retT;
		return vector4<retT>(left*right.x,
						     left*right.y,
						     left*right.z,
							 left*right.w);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator==(const vector4<T> &left,const vector4<T2> &right)
	{
		return left.x==right.x && left.y==right.y && left.z==right.z && left.w==right.w;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	inline bool operator!=(const vector4<T> &left,const vector4<T2> &right)
	{
		return left.x!=right.x || left.y!=right.y || left.z!=right.z || left.w!=right.w;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> operator-(const vector4<T> &vec)
	{
		return vector4<T>(vec.x*T(-1),
						  vec.y*T(-1),
						  vec.z*T(-1),
						  vec.w*T(-1));
	}
}
#endif
