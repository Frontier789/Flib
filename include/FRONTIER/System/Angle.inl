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
#ifndef FRONTIER_ANGLE_INL_INCLUDED
#define FRONTIER_ANGLE_INL_INCLUDED

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T>::Angle() : m_amount(T())
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T>::Angle(const T &angle) : m_amount(angle)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	Angle<T>::Angle(const Angle<T2> &copy) : m_amount(copy.asRad())
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	T Angle<T>::asDeg() const
	{
		return T(180.0) * m_amount / T(3.14159265358979);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	T Angle<T>::asRad() const
	{
		return m_amount;
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	template<class T2>
	Angle<T>::operator Angle<T2>() const
	{
		return Angle<T2>(T2(m_amount));
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator+(const Angle<T> &left,const Angle<T2> &right) -> Angle<decltype(left.asRad()+right.asRad())>
	{
		return Angle<decltype(left.asRad()+right.asRad())>(left.asRad() + right.asRad());
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator-(const Angle<T> &left,const Angle<T2> &right) -> Angle<decltype(left.asRad()-right.asRad())>
	{
		return Angle<decltype(left.asRad()-right.asRad())>(left.asRad() - right.asRad());
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator-(const Angle<T> &angle) -> Angle<decltype(-angle.asRad())>
	{
		return Angle<decltype(-angle.asRad())>(-angle.asRad());
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const Angle<T> &left,const T2 &right) -> Angle<decltype(left.asRad()*right)>
	{
		return Angle<decltype(left.asRad()*right)>(left.asRad() * right);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const T2 &left,const Angle<T> &right) -> Angle<decltype(left*right.asRad())>
	{
		return Angle<decltype(left+right.asRad())>(left * right.asRad());
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	auto operator/(const Angle<T> &left,const T &right) -> Angle<decltype(left.asRad()/right)>
	{
		return Angle<decltype(left.asRad()+right)>(left.asRad() + right);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator/(const Angle<T> &left,const Angle<T2> &right) -> Angle<decltype(left.asRad()/right.asRad())>
	{
		return Angle<decltype(left.asRad()/right.asRad())>(left.asRad() / right.asRad());
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator+=(Angle<T> &left,const Angle<T> &right)
	{
		return left = left + right;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator-=(Angle<T> &left,const Angle<T> &right)
	{
		return left = left - right;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator*=(Angle<T> &left,const T &right)
	{
		return left = left * right;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	Angle<T> &operator/=(Angle<T> &left,const T &right)
	{
		return left = left / right;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator==(const Angle<T> &left,const Angle<T2> &right)
	{
		return left.asRad() == right.asRad();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator!=(const Angle<T> &left,const Angle<T2> &right)
	{
		return left.asRad() != right.asRad();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator< (const Angle<T> &left,const Angle<T2> &right)
	{
		return left.asRad() < right.asRad();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator<=(const Angle<T> &left,const Angle<T2> &right)
	{
		return left.asRad() <= right.asRad();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator> (const Angle<T> &left,const Angle<T2> &right)
	{
		return left.asRad() > right.asRad();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	bool operator>=(const Angle<T> &left,const Angle<T2> &right)
	{
		return left.asRad() >= right.asRad();
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	auto deg(const T &amount) -> Angle<decltype(amount / 180.f * F_PI)>
	{
		return Angle<decltype(amount / 180.f * F_PI)>(decltype(amount / 180.f * F_PI)(F_PI) * amount / T(180.0));
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	auto rad(const T &amount) -> Angle<decltype(amount * F_PI)>
	{
		return Angle<decltype(amount * F_PI)>(amount);
	}
}

namespace std
{
	/////////////////////////////////////////////////////////////
	template<class T>
	auto sin(const fm::Angle<T> &angle) -> decltype(sin(angle.asRad()))
	{
		return sin(angle.asRad());
	}

	/////////////////////////////////////////////////////////////
	template<class T>
    auto cos(const fm::Angle<T> &angle) -> decltype(cos(angle.asRad()))
    {
		return cos(angle.asRad());
    }

	/////////////////////////////////////////////////////////////
	template<class T>
    auto tan(const fm::Angle<T> &angle) -> decltype(tan(angle.asRad()))
    {
		return tan(angle.asRad());
    }

}

#endif // FRONTIER_ANGLE_INL_INCLUDED
