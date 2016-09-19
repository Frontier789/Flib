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
#ifndef FRONTIER_BIGUINT_HPP_INCLUDED
#define FRONTIER_BIGUINT_HPP_INCLUDED
#include <FRONTIER/System/macros/TYPES.hpp>
#define FRONTIER_BIGINT
#include <string>

namespace fm
{
	template<class> class BigInt;
	
	//////////////////////////////////////////////////////////////////////////
	template<class BaseUint>
	class BigUint
	{
		BaseUint lo;
		BaseUint hi;
	public:
		typedef BaseUint BaseType;
		
		//////////////////////////////////////////////////////////////////////////
		BigUint();
		BigUint(Uint8  u);
		BigUint(Uint16 u);
		BigUint(Uint32 u);
		BigUint(Uint64 u);
		BigUint(Int8  i);
		BigUint(Int16 i);
		BigUint(Int32 i);
		BigUint(Int64 i);
		BigUint(float f);
		BigUint(double d);
		BigUint(const BigInt<BaseUint> &bi);
		BigUint(const BigUint<BaseUint> &bi);
		BigUint(const std::string &str);
		template<Size N>
		BigUint(const char (&str)[N]);
		
		//////////////////////////////////////////////////////////////////////////
		std::string toString(unsigned int base = 10) const;
		
		//////////////////////////////////////////////////////////////////////////
		bool operator!() const;
		
		//////////////////////////////////////////////////////////////////////////
		bool operator==(const BigUint &bi) const;
		bool operator!=(const BigUint &bi) const;
		bool operator< (const BigUint &bi) const;
		bool operator> (const BigUint &bi) const;
		bool operator<=(const BigUint &bi) const;
		bool operator>=(const BigUint &bi) const;
		
		//////////////////////////////////////////////////////////////////////////
		BigUint<BaseUint> operator-() const;
		BigUint<BaseUint> operator~() const;
		
		//////////////////////////////////////////////////////////////////////////
		BigUint<BaseUint> &operator++();
		BigUint<BaseUint> &operator--();
		
		//////////////////////////////////////////////////////////////////////////
		BigUint<BaseUint> &operator+=(const BigUint &bi);
		BigUint<BaseUint> &operator-=(const BigUint &bi);
		BigUint<BaseUint> &operator*=(const BigUint &bi);
		BigUint<BaseUint> &operator|=(const BigUint &bi);
		BigUint<BaseUint> &operator&=(const BigUint &bi);
		BigUint<BaseUint> &operator^=(const BigUint &bi);
		BigUint<BaseUint> &operator/=(const BigUint &bi);
		BigUint<BaseUint> &operator%=(const BigUint &b);
		BigUint<BaseUint> &operator<<=(const BigUint& rhs);
		BigUint<BaseUint> &operator>>=(const BigUint<BaseUint> &rhs);
		
		//////////////////////////////////////////////////////////////////////////
		BigUint<BaseUint> operator+(const BigUint &bi) const;
		BigUint<BaseUint> operator-(const BigUint &bi) const;
		BigUint<BaseUint> operator*(const BigUint &bi) const;
		BigUint<BaseUint> operator|(const BigUint &bi) const;
		BigUint<BaseUint> operator&(const BigUint &bi) const;
		BigUint<BaseUint> operator^(const BigUint &bi) const;
		BigUint<BaseUint> operator/(const BigUint &bi) const;
		BigUint<BaseUint> operator%(const BigUint &b)  const;
		BigUint<BaseUint> operator<<(const BigUint& rhs) const;
		BigUint<BaseUint> operator>>(const BigUint<BaseUint> &rhs) const;
		
		//////////////////////////////////////////////////////////////////////////
		const BaseUint &getLoUint() const;
		const BaseUint &getHiUint() const;
	};
	
	//////////////////////////////////////////////////////////////////////////
	typedef BigUint<Uint64> Uint128;
	typedef BigUint<BigUint<Uint64> > Uint256;
	typedef BigUint<BigUint<BigUint<Uint64> > > Uint512;
	typedef BigUint<BigUint<BigUint<BigUint<Uint64> > > > Uint1024;
}

#endif // FRONTIER_BIGUINT_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/BigUint.inl>
#endif // FRONTIER_DONT_INCLUDE_INL