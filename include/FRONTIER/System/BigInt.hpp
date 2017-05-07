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
#ifndef FRONTIER_BIGINT_HPP_INCLUDED
#define FRONTIER_BIGINT_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>
#define FRONTIER_BIGINT
#include <string>

namespace fm
{
	template<class> class BigUint;

	//////////////////////////////////////////////////////////////////////////
	/// @brief Class for storing big signed integers with linear operation time in bytesize
	/// 
	/// @ingroup System
	///
	//////////////////////////////////////////////////////////////////////////
	template<class BaseUint>
	class BigInt
	{
		BaseUint lo;
		BaseUint hi;
	public:
		typedef BaseUint BaseType;
		
		//////////////////////////////////////////////////////////////////////////
		BigInt();
		BigInt(Uint8  u);
		BigInt(Uint16 u);
		BigInt(Uint32 u);
		BigInt(Uint64 u);
		BigInt(Int8  i);
		BigInt(Int16 i);
		BigInt(Int32 i);
		BigInt(Int64 i);
		BigInt(float f);
		BigInt(double d);
		BigInt(const BigUint<BaseUint> &bi);
		BigInt(const BigInt<BaseUint> &bi);
		BigInt(const std::string &str);
		template<Size N>
		BigInt(const char (&str)[N]);
		
		//////////////////////////////////////////////////////////////////////////
		std::string toString(unsigned int base = 10) const;
		
		//////////////////////////////////////////////////////////////////////////
		bool operator!() const;
		
		//////////////////////////////////////////////////////////////////////////
		bool operator==(const BigInt &bi) const;
		bool operator!=(const BigInt &bi) const;
		bool operator< (const BigInt &bi) const;
		bool operator> (const BigInt &bi) const;
		bool operator<=(const BigInt &bi) const;
		bool operator>=(const BigInt &bi) const;
		
		//////////////////////////////////////////////////////////////////////////
		BigInt<BaseUint> operator-() const;
		BigInt<BaseUint> operator~() const;
		
		//////////////////////////////////////////////////////////////////////////
		BigInt<BaseUint> &operator++();
		BigInt<BaseUint> &operator--();
		
		//////////////////////////////////////////////////////////////////////////
		BigInt<BaseUint> &operator+=(const BigInt &bi);
		BigInt<BaseUint> &operator-=(const BigInt &bi);
		BigInt<BaseUint> &operator*=(const BigInt &bi);
		BigInt<BaseUint> &operator|=(const BigInt &bi);
		BigInt<BaseUint> &operator&=(const BigInt &bi);
		BigInt<BaseUint> &operator^=(const BigInt &bi);
		BigInt<BaseUint> &operator/=(const BigInt &bi);
		BigInt<BaseUint> &operator%=(const BigInt &b);
		BigInt<BaseUint> &operator<<=(const BigInt& rhs);
		BigInt<BaseUint> &operator>>=(const BigInt<BaseUint> &rhs);
		
		//////////////////////////////////////////////////////////////////////////
		BigInt<BaseUint> operator+(const BigInt &bi) const;
		BigInt<BaseUint> operator-(const BigInt &bi) const;
		BigInt<BaseUint> operator*(const BigInt &bi) const;
		BigInt<BaseUint> operator|(const BigInt &bi) const;
		BigInt<BaseUint> operator&(const BigInt &bi) const;
		BigInt<BaseUint> operator^(const BigInt &bi) const;
		BigInt<BaseUint> operator/(const BigInt &bi) const;
		BigInt<BaseUint> operator%(const BigInt &b)  const;
		BigInt<BaseUint> operator<<(const BigInt& rhs) const;
		BigInt<BaseUint> operator>>(const BigInt<BaseUint> &rhs) const;
		
		//////////////////////////////////////////////////////////////////////////
		const BaseUint &getLoUint() const;
		const BaseUint &getHiUint() const;
	};
	
	//////////////////////////////////////////////////////////////////////////
	namespace priv
	{
		template <typename T>
		void divide(BigInt<T> n,BigInt<T> d,BigInt<T> &q,BigInt<T> &rem);
	}
	
	//////////////////////////////////////////////////////////////////////////
	typedef BigInt<Uint64> Int128;
	typedef BigInt<BigUint<Uint64> > Int256;
	typedef BigInt<BigUint<BigUint<Uint64> > > Int512;
	typedef BigInt<BigUint<BigUint<BigUint<Uint64> > > > Int1024;
}

#endif // FRONTIER_BIGINT_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/BigInt.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
