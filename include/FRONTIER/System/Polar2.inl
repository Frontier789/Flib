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
#ifndef FRONTIER_POLAR_INL_INCLUDED
#define FRONTIER_POLAR_INL_INCLUDED
#include <cmath>
namespace fm
{
	/// constructors /////////////////////////////////////////////////////////
	template<class T>
	inline polar2<T>::polar2() : length(0),
								 angle(Angle())
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline polar2<T>::polar2(const T &length) : length(length),
												angle(Angle())
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline polar2<T>::polar2(const T &length,const Angle &angle) : length(length),
	                                                               angle(angle)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline polar2<T>::polar2(const polar2<T> &copy) : length(copy.length),
													  angle(copy.angle)
	{

	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline polar2<T>::polar2(const vector2<T2> &vec) : length(vec.length()),
													   angle(rad(std::atan2(vec.y,vec.x)))
	{

	}


	/// operators /////////////////////////////////////////////////////////
	template<class T>
	inline typename polar2<T>::reference polar2<T>::operator()(const T &Length,const Angle &Alpha)
	{
		length=Length;
		angle =Alpha;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	inline typename polar2<T>::reference polar2<T>::operator()(const vector2<T2> &vec)
	{
		polar2<T> pol(vec);
		length=pol.length;
		angle =pol.angle;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline polar2<T> polar2<T>::projTo(typename polar2<T>::const_reference target) const
	{
		return polar2<T>(target.length*std::cos(angle-target.angle),target.angle);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline polar2<T> polar2<T>::reflectOn(typename polar2<T>::const_reference pol) const
	{
		return polar2<T>(length,
						 pol.angle * T(2) - angle);
	}


	////////////////////////////////////////////////////////////
	template<class T>
	inline T polar2<T>::dot(typename polar2<T>::const_reference pol) const
	{
		return length * pol.length * std::cos(angle-pol.angle);
	}


	/// conversions /////////////////////////////////////////////////////////
	template<class T>
	inline polar2<T>::operator vector2<T>() const
	{
		return vector2<T>(std::cos(angle),
						  std::sin(angle)) * length;
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &polar2<T>::operator+=(const polar2<T> &other)
	{
		*this = polar2<T>(vector2<T>(*this)+vector2<T>(other));
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &polar2<T>::operator-=(const polar2<T> &other)
	{
		*this = polar2<T>(vector2<T>(*this)-vector2<T>(other));
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &polar2<T>::operator*=(const T &other)
	{
		length*=other;
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> &polar2<T>::operator/=(const T &other)
	{
		length/=other;
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> polar2<T>::operator+(const polar2<T> &other) const
	{
		return polar2<T>(vector2<T>(*this)+vector2<T>(other));
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> polar2<T>::operator-(const polar2<T> &other) const
	{
		return polar2<T>(vector2<T>(*this)-vector2<T>(other));
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> polar2<T>::operator*(const T &scalar) const
	{
		return polar2<T>(length*scalar,angle);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> polar2<T>::operator/(const T &scalar) const
	{
		return polar2<T>(length/scalar,angle);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	bool polar2<T>::operator==(const polar2<T> &other) const 
	{
		return (length==other.length	    && (angle-other.angle).asDegrees()%360==0) ||
			   (length==other.length*T(-1) && (angle-other.angle).asDegrees()%360==180);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	bool polar2<T>::operator!=(const polar2<T> &other) const
	{
		return !(length==other.length	     && (angle-other.angle).asDegrees()%360==0) &&
			   !(length==other.length*T(-1) && (angle-other.angle).asDegrees()%360==180);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator-(const polar2<T> &pol)
	{
		return polar2<T>(pol.length*T(-1),pol.angle);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	polar2<T> operator*(const T &left,const polar2<T> &right)
	{
		return polar2<T>(right.length*left,right.angle);
	}
}
#endif
