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
#ifndef FRONTIER_POLAR3_INL_INCLUDED
#define FRONTIER_POLAR3_INL_INCLUDED
#include <cmath>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T>::polar3(const T &length,const Angle &theta,const Angle &phi) : length(length),
																			 theta(theta),
																			 phi(phi)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T>::polar3(typename polar3<T>::const_reference copy) : length(copy.length),
																  theta(copy.theta),
																  phi(copy.phi)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	polar3<T>::polar3(const vector3<T2> &vec) : length(vec.length()),
												theta(fm::rad(std::atan2(vec.y,vec.x))),
												phi( fm::rad(std::asin(vec.z/length)) )
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T>
	typename polar3<T>::reference polar3<T>::operator()(const T &Length,const Angle &Theta,const Angle &Phi)
	{
		length = Length;
		Theta  = theta;
		Phi    = phi;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	typename polar3<T>::reference polar3<T>::operator()(const vector3<T2> &vec)
	{
		(*this) = polar3<T>(vec);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	typename polar3<T>::reference polar3<T>::operator+=(const polar3<T> &other)
	{
		length += other.length;
		theta  += other.theta;
		phi    += other.phi;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	typename polar3<T>::reference polar3<T>::operator-=(const polar3<T> &other)
	{
		length -= other.length;
		theta  -= other.theta;
		phi    -= other.phi;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	typename polar3<T>::reference polar3<T>::operator*=(const T &scalar)
	{
		length *= scalar;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	typename polar3<T>::reference polar3<T>::operator/=(const T &scalar)
	{
		length /= scalar;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::operator+(const polar3<T> &other) const
	{
		return polar3<T>(length + other.length,
						 theta  + other.theta,
						 phi    + other.phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::operator-(const polar3<T> &other) const
	{
		return polar3<T>(length - other.length,
						 theta  - other.theta,
						 phi    - other.phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::operator*(const T &scalar) const
	{
		return polar3<T>(length * scalar,theta,phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::operator/(const T &scalar) const
	{
		return polar3<T>(length / scalar,theta,phi);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	bool polar3<T>::operator==(const polar3<T> &other) const
	{
		return (vector3<T>(*this) == vector3<T>(other));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	bool polar3<T>::operator!=(const polar3<T> &other) const
	{
		return (vector3<T>(*this) != vector3<T>(other));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::cross(const polar3<T> &other) const
	{
		return polar3<T>(vector3<T>(*this).cross(vector3<T>(other)));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::projTo(const polar3<T> &target) const
	{
		target * this->dot(target) / target.length / target.length;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::projToPlane(const polar3<T> &normal) const
	{
		return polar3<T>(vector3<T>(*this).projToPlane(vector3<T>(normal)));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> polar3<T>::reflectOn(const polar3<T> &pol) const
	{
		return polar3<T>(vector3<T>(*this).reflectOn(vector3<T>(pol)));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	T polar3<T>::dot(const polar3<T> &pol) const
	{
		return length * pol.length * (std::cos(phi)*std::cos(pol.phi)*std::cos(theta - pol.theta) + std::sin(phi)*std::sin(pol.phi));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T>::operator vector3<T>() const
	{
		return vector3<T>(length * std::cos(phi) * std::cos(theta),
						  length * std::cos(phi) * std::sin(theta),
						  length * std::sin(phi));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> operator-(const polar3<T> &pol)
	{
		return polar3<T>(pol.length * T(-1),pol.phi,pol.theta);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	polar3<T> operator*(const T &left,const polar3<T> &right)
	{
		return polar3<T>(right.length * left,right.phi,right.theta);
	}
}
#endif
