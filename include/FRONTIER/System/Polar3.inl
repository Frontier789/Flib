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
#ifndef FRONTIER_POLAR3_INL_INCLUDED
#define FRONTIER_POLAR3_INL_INCLUDED
#include <cmath>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T,class A>
	polar3<T,A>::polar3(const T &length,const Angle<A> &theta,const Angle<A> &phi) : length(length),
																			   theta(theta),
																			   phi(phi)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	polar3<T,A>::polar3(typename polar3<T,A>::const_reference copy) : length(copy.length),
																	  theta(copy.theta),
																	  phi(copy.phi)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2>
	polar3<T,A>::polar3(const vector3<T2> &vec) : length(vec.length()),
												  theta(fm::rad(std::atan2(vec.y,vec.x))),
												  phi( fm::rad(std::asin(vec.z/length)) )
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	typename polar3<T,A>::reference polar3<T,A>::operator+=(const polar3<T2,A2> &other)
	{
		return (*this) = (*this) + other;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	typename polar3<T,A>::reference polar3<T,A>::operator-=(const polar3<T2,A2> &other)
	{
		return (*this) = (*this) - other;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	typename polar3<T,A>::reference polar3<T,A>::operator*=(const T &scalar)
	{
		length *= scalar;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	typename polar3<T,A>::reference polar3<T,A>::operator/=(const T &scalar)
	{
		length /= scalar;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	auto polar3<T,A>::operator+(const polar3<T2,A2> &other) const -> polar3<decltype(length + other.length),decltype((theta + other.theta).asRad())>
	{
		return polar3<decltype(length + other.length),decltype((theta + other.theta).asRad())>(length + other.length,
																							   theta  + other.theta,
																							   phi    + other.phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	auto polar3<T,A>::operator-(const polar3<T2,A2> &other) const -> polar3<decltype(length - other.length),decltype((theta - other.theta).asRad())>
	{
		return polar3<decltype(length + other.length),decltype((theta + other.theta).asRad())>(length - other.length,
																							   theta  - other.theta,
																							   phi    - other.phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2>
	auto polar3<T,A>::operator*(const T2 &scalar) const -> polar3<decltype(length * scalar),A>
	{
		return polar3<decltype(length * scalar),A>(length * scalar,theta,phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2>
	auto polar3<T,A>::operator/(const T2 &scalar) const -> polar3<decltype(length / scalar),A>
	{
		return polar3<decltype(length / scalar),A>(length / scalar,theta,phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	bool polar3<T,A>::operator==(const polar3<T2,A2> &other) const
	{
		return (vector3<T>(*this) == vector3<T2>(other));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	template<class T2,class A2>
	bool polar3<T,A>::operator!=(const polar3<T2,A2> &other) const
	{
		return (vector3<T>(*this) != vector3<T2>(other));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	polar3<T,A> polar3<T,A>::cross(const polar3<T,A> &other) const
	{
		return polar3<T,A>(vector3<T>(*this).cross(vector3<T>(other)));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	polar3<T,A> polar3<T,A>::projTo(const polar3<T,A> &target) const
	{
		target * this->dot(target) / target.length / target.length;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	polar3<T,A> polar3<T,A>::projToPlane(const polar3<T,A> &normal) const
	{
		return polar3<T,A>(vector3<T>(*this).projToPlane(vector3<T>(normal)));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	polar3<T,A> polar3<T,A>::reflectOn(const polar3<T,A> &pol) const
	{
		return polar3<T,A>(vector3<T>(*this).reflectOn(vector3<T>(pol)));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	T polar3<T,A>::dot(const polar3<T,A> &pol) const
	{
		return length * pol.length * (std::cos(phi)*std::cos(pol.phi)*std::cos(theta - pol.theta) + std::sin(phi)*std::sin(pol.phi));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	polar3<T,A>::operator vector3<T>() const
	{
		return vector3<T>(length * std::cos(phi) * std::cos(theta),
						  length * std::cos(phi) * std::sin(theta),
						  length * std::sin(phi));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A>
	auto operator-(const polar3<T,A> &pol) -> polar3<decltype(-pol.length),A>
	{
		return polar3<decltype(-pol.length),A>(-pol.length,pol.theta,pol.phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class A,class T2>
	auto operator*(const T2 &left,const polar3<T,A> &right) -> polar3<decltype(left*right.length),A>
	{
		return polar3<decltype(left*right.length),A>(left * right.length,right.theta,right.phi);
	}
}
#endif
