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
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/Config.hpp>
#include <cmath>

namespace fm
{
    Angle::MeasureType Angle::globalMeasureType = Angle::Degrees;


    ////////////////////////////////////////////////////////////
	Angle::Angle() : m_amount(0)
    {

    }

    ////////////////////////////////////////////////////////////
	Angle::Angle(float angle) : m_amount(globalMeasureType ? angle : angle/180.f*fm::PI)
    {

    }

    ////////////////////////////////////////////////////////////
	Angle::Angle(const Angle &copy) : m_amount(copy.asRadians())
    {

    }

    ////////////////////////////////////////////////////////////
	Angle::Angle(float angle,MeasureType type) : m_amount(type ? angle : angle/180.f*fm::PI)
    {

    }


    float Angle::asDegrees() const
    {
        return m_amount * 180.f / fm::PI;
    }


    ////////////////////////////////////////////////////////////
    float Angle::asRadians() const
    {
        return m_amount;
    }


    ////////////////////////////////////////////////////////////
    float Angle::asDegs() const
    {
        return asDegrees();
    }


    ////////////////////////////////////////////////////////////
    float Angle::asRads() const
    {
        return asRadians();
    }


    ////////////////////////////////////////////////////////////
    float Angle::asDeg() const
    {
        return asDegrees();
    }


    ////////////////////////////////////////////////////////////
    float Angle::asRad() const
    {
        return asRadians();
    }


	/// operators //////////////////////////////////////////////////////////
	Angle operator+(const Angle &left,const Angle &right)
	{
		return radians(left.asRadians()+right.asRadians());
	}

	/////////////////////////////////////////////////////////////
	Angle operator-(const Angle &left,const Angle &right)
	{
		return radians(left.asRadians()-right.asRadians());
	}
	
	/////////////////////////////////////////////////////////////
	Angle operator-(const Angle &angle)
	{
		return radians(-angle.asRadians());
	}

	/////////////////////////////////////////////////////////////
	Angle operator*(const Angle &left,const float &right)
	{
		return radians(left.asRadians()*right);
	}

	/////////////////////////////////////////////////////////////
    Angle operator*(const float &left,const Angle &right)
	{
		return radians(left*right.asRadians());
	}

	/////////////////////////////////////////////////////////////
	Angle operator/(const Angle &left,const float &right)
	{
		return radians(left.asRadians()/right);
	}

	/////////////////////////////////////////////////////////////
	Angle &operator+=(Angle &left,const Angle &right)
	{
		*((float*)&left) += right.asRadians();
		return left;
	}

	/////////////////////////////////////////////////////////////
	Angle &operator-=(Angle &left,const Angle &right)
	{
		*((float*)&left) -= right.asRadians();
		return left;
	}

	/////////////////////////////////////////////////////////////
	Angle &operator*=(Angle &left,const float &right)
	{
		*((float*)&left) *= right;
		return left;
	}

	/////////////////////////////////////////////////////////////
	Angle &operator/=(Angle &left,const float &right)
	{
		*((float*)&left) /= right;
		return left;
	}

	/////////////////////////////////////////////////////////////
	bool operator==(const Angle &left,const Angle &right)
	{
		return left.asRadians() == right.asRadians();
	}

	/////////////////////////////////////////////////////////////
	bool operator!=(const Angle &left,const Angle &right)
	{
		return left.asRadians() != right.asRadians();
	}

	/////////////////////////////////////////////////////////////
	bool operator< (const Angle &left,const Angle &right)
	{
		return left.asRadians() < right.asRadians();
	}

	/////////////////////////////////////////////////////////////
	bool operator<=(const Angle &left,const Angle &right)
	{
		return left.asRadians() <= right.asRadians();
	}

	/////////////////////////////////////////////////////////////
	bool operator> (const Angle &left,const Angle &right)
	{
		return left.asRadians() > right.asRadians();
	}

	/////////////////////////////////////////////////////////////
	bool operator>=(const Angle &left,const Angle &right)
	{
		return left.asRadians() >= right.asRadians();
	}

    Angle degrees(float amount)
    {
    	Angle ret;
    	*((float*)&ret) = amount/180.f*fm::PI;
        return ret;
    }

    ////////////////////////////////////////////////////////////
    Angle radians(float amount)
    {
    	Angle ret;
    	*((float*)&ret) = amount;
        return ret;
    }


    ////////////////////////////////////////////////////////////
    Angle deg(float amount)
    {
        return degrees(amount);
    }


    ////////////////////////////////////////////////////////////
    Angle rad(float amount)
    {
        return radians(amount);
    }
}
namespace std
{
    float sin(const fm::Angle &angle)
    {
        return std::sin(angle.asRad());
    }


    ////////////////////////////////////////////////////////////
    float cos(const fm::Angle &angle)
    {
        return std::cos(angle.asRad());
    }


    ////////////////////////////////////////////////////////////
    float tan(const fm::Angle &angle)
    {
        return std::tan(angle.asRad());
    }
}

