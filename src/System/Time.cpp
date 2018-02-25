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
#include <FRONTIER/System/Time.hpp>
namespace fm
{
	/////////////////////////////////////////////////////////////
	Time::Time(double secs) : m_amount(secs * 1000000.0)
	{

	}

	/////////////////////////////////////////////////////////////
	double Time::asMicroseconds() const
	{
		return m_amount;
	}

	/////////////////////////////////////////////////////////////
	double Time::asMilliseconds() const
	{
		return m_amount/1000.0;
	}

	/////////////////////////////////////////////////////////////
	double Time::asSeconds() const
	{
		return m_amount/1000.0/1000.0;
	}


	/////////////////////////////////////////////////////////////
	double Time::asMicsecs() const
	{
		return m_amount;
	}

	/////////////////////////////////////////////////////////////
	double Time::asMilsecs() const
	{
		return m_amount/1000.0;
	}

	/////////////////////////////////////////////////////////////
	double Time::asSecs() const
	{
		return m_amount/1000.0/1000.0;
	}


	/////////////////////////////////////////////////////////////
	double Time::Ms() const
	{
		return m_amount;
	}

	/////////////////////////////////////////////////////////////
	double Time::ms() const
	{
		return m_amount/1000.0;
	}

	/////////////////////////////////////////////////////////////
	double Time::s() const
	{
		return m_amount/1000.0/1000.0;
	}

	/////////////////////////////////////////////////////////////
	const Time Time::Zero = Time(0);

	/////////////////////////////////////////////////////////////
	const Time Time::Inf = Time(-1);

	/////////////////////////////////////////////////////////////
	Time operator+(const Time &left,const Time &right)
	{
		return microseconds(left.asMicroseconds()+right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	Time operator-(const Time &left,const Time &right)
	{
		return microseconds(left.asMicroseconds()-right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	Time operator-(const Time &time)
	{
		return microseconds(time.asMicroseconds()*-1.0);
	}

	/////////////////////////////////////////////////////////////
	Time operator*(const Time &left,const double &right)
	{
		return microseconds(left.asMicroseconds()*right);
	}

	/////////////////////////////////////////////////////////////
	Time operator*(const double &left,const Time &right)
	{
		return microseconds(left*right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	Time operator/(const Time &left,const double &right)
	{
		return microseconds(left.asMicroseconds()/right);
	}

	/////////////////////////////////////////////////////////////
	double operator/(const Time &left,const Time &right)
	{
		return left.asMicroseconds()/right.asMicroseconds();
	}

	/////////////////////////////////////////////////////////////
	Time &operator+=(Time &left,const Time &right)
	{
		return left = left + right;
	}

	/////////////////////////////////////////////////////////////
	Time &operator-=(Time &left,const Time &right)
	{
		return left = left - right;
	}

	/////////////////////////////////////////////////////////////
	Time &operator*=(Time &left,const double &right)
	{
		return left = left * right;
	}

	/////////////////////////////////////////////////////////////
	Time &operator/=(Time &left,const double &right)
	{
		return left = left / right;
	}

	/////////////////////////////////////////////////////////////
	bool operator==(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()==right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	bool operator!=(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()!=right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	bool operator< (const Time &left,const Time &right)
	{
		return (left.asMicroseconds()<right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	bool operator<=(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()<=right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	bool operator> (const Time &left,const Time &right)
	{
		return (left.asMicroseconds()>right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	bool operator>=(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()>=right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	Time microseconds(double Msecs)
	{
		return Time(Msecs / 1000.0 / 1000.0);
	}

	/////////////////////////////////////////////////////////////
	Time milliseconds(double msecs)
	{
		return Time(msecs / 1000.0);
	}

	/////////////////////////////////////////////////////////////
	Time seconds(double secs)
	{
		return Time(secs);
	}
}
