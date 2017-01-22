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
#ifndef FRONTIER_POLAR2_INL_INCLUDED
#define FRONTIER_POLAR2_INL_INCLUDED
#include <cmath>

namespace fm
{
	////////////////////////////////////////////////////////////
	template<class T,class A>
	inline polar2<T,A>::polar2(const T &length,const Angle<A> &angle) : length(length),
																		angle(angle)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	inline polar2<T,A>::polar2(const polar2<T2,A2> &copy) : length(copy.length),
															angle(copy.angle)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2>
	inline polar2<T,A>::polar2(const vector2<T2> &vec) : length(vec.length()),
														 angle(fm::rad<A>(std::atan2(vec.y,vec.x)))
	{

	}


	/// operators /////////////////////////////////////////////////////////
	template<class T,class A>
	inline typename polar2<T,A>::reference polar2<T,A>::operator()(const T &length,const Angle<A> &angle)
	{
		this->length = length;
		this->angle  = angle;
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	inline polar2<T,A> polar2<T,A>::projTo(typename polar2<T,A>::const_reference target) const
	{
		return polar2<T,A>(target.length*std::cos(angle-target.angle),target.angle);
	}


	////////////////////////////////////////////////////////////
	template<class T,class A>
	inline polar2<T,A> polar2<T,A>::reflectOn(typename polar2<T,A>::const_reference pol) const
	{
		return polar2<T,A>(length,
						 pol.angle * T(2) - angle);
	}


	////////////////////////////////////////////////////////////
	template<class T,class A>
	inline T polar2<T,A>::dot(typename polar2<T,A>::const_reference pol) const
	{
		return length * pol.length * std::cos(angle-pol.angle);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class A>
	inline polar2<T,A> polar2<T,A>::sgn() const
	{
		return polar2<T,A>(T(1),angle);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	inline polar2<T,A> polar2<T,A>::perp() const
	{
		return polar2<T,A>(length,angle + deg(90));
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class A>
	auto polar2<T,A>::LENGTH() const -> decltype(T()*T())
	{
		return length*length;
	}


	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2>
	inline polar2<T,A>::operator vector2<T2>() const
	{
		return vector2<T2>(std::cos(angle),
						   std::sin(angle)) * length;
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	polar2<T,A> &polar2<T,A>::operator+=(const polar2<T2,A2> &other)
	{
		return (*this) = (*this) + other;
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	polar2<T,A> &polar2<T,A>::operator-=(const polar2<T2,A2> &other)
	{
		return (*this) = (*this) - other;
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	polar2<T,A> &polar2<T,A>::operator*=(const T &other)
	{
		length *= other;
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	polar2<T,A> &polar2<T,A>::operator/=(const T &other)
	{
		length /= other;
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	auto polar2<T,A>::operator+(const polar2<T2,A2> &other) const -> polar2<decltype(length + other.length),decltype((angle + other.angle).asRad())>
	{
		return polar2<decltype(length + other.length),decltype((angle + other.angle).asRad())>(length + other.length,
																							   angle  + other.angle);
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	auto polar2<T,A>::operator-(const polar2<T2,A2> &other) const -> polar2<decltype(length - other.length),decltype((angle - other.angle).asRad())>
	{
		return (*this) + (-other);
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2>
	auto polar2<T,A>::operator*(const T2 &scalar) const -> polar2<decltype(length * scalar),A>
	{
		return polar2<decltype(length * scalar),A>(length * scalar,angle);
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2>
	auto polar2<T,A>::operator/(const T2 &scalar) const -> polar2<decltype(length / scalar),A>
	{
		return polar2<decltype(length / scalar),A>(length / scalar,angle);
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	bool polar2<T,A>::operator==(const polar2<T2,A2> &other) const
	{
		return (length == other.length       && fmod(fmod((angle-other.angle).asDegrees(),360.f)+360.f,360.f) == 0) ||
			   (length == other.length*T(-1) && fmod(fmod((angle-other.angle).asDegrees(),360.f)+360.f,360.f) == 180);
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	bool polar2<T,A>::operator!=(const polar2<T2,A2> &other) const
	{
		return !(length == other.length       && fmod(fmod((angle-other.angle).asDegrees(),360.f)+360.f,360.f)  == 0) &&
			   !(length == other.length*T(-1) && fmod(fmod((angle-other.angle).asDegrees(),360.f)+360.f,360.f)  == 180);
	}

	////////////////////////////////////////////////////////////
	template<class T,class A>
	auto operator-(const polar2<T,A> &pol) -> polar2<decltype(-pol.length),A>
	{
		return polar2<T,A>(-pol.length,pol.angle);
	}

	////////////////////////////////////////////////////////////
	template<class T,class A,class T2>
	auto operator*(const T2 &left,const polar2<T,A> &right) -> polar2<decltype(left * right.length),A>
	{
		return polar2<T,A>(left * right.length,right.angle);
	}
}
#endif
