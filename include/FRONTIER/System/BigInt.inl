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
#ifndef FRONTIER_BIGINT_INL_INCLUDED
#define FRONTIER_BIGINT_INL_INCLUDED
#include <FRONTIER/System/BigInt.hpp>
#include <FRONTIER/System/BigUint.hpp>

namespace fm
{
	namespace priv 
	{
		template <typename T>
		inline void divide(BigInt<T> n,BigInt<T> d,BigInt<T> &q,BigInt<T> &rem) 
		{
			T negBit = (T(1) << T(sizeof(T)*8-1));
			
			bool nNeg = (n.getHiUint() & negBit) != 0;
			bool dNeg = (d.getHiUint() & negBit) != 0;
			
			if (nNeg) n = -n;
			if (dNeg) d = -d;
			
			fm::priv::divideU(BigUint<T>(n),BigUint<T>(d),*((BigUint<T>*)&q),*((BigUint<T>*)&rem));
			
			if ((nNeg && !dNeg) || (!nNeg && dNeg)) q = -q;
			if (nNeg) rem = -rem;
		}
	}
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt() : lo(0), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Uint8 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Uint16 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Uint32 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Uint64 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Int8 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Int16 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Int32 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(Int64 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(float f) : lo(f < 0 ? -f : f), hi(0)
	{
		if (f < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(double d) : lo(d < 0 ? -d : d), hi(0)
	{
		if (d < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(const BigInt &bi) : lo(bi.lo), hi(bi.hi)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(const BigUint<BaseUint> &bi) : lo(bi.getLoUint()), hi(bi.getHiUint())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint>::BigInt(const std::string &str) : lo(0), hi(0)
	{
		(*this) = BigUint<BaseUint>(str);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	template<Size N>
	inline BigInt<BaseUint>::BigInt(const char (&str)[N])
	{
		(*this) = BigUint<BaseUint>(str);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline std::string BigInt<BaseUint>::toString(unsigned int base) const
	{
		BaseUint negBit = (BaseUint(1) << BaseUint(sizeof(BaseUint)*8-1));
		
		if ((hi & negBit) != 0)
		{
			return '-'+BigUint<BaseUint>(-(*this)).toString();
		}

    	return BigUint<BaseUint>(*this).toString();
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline bool BigInt<BaseUint>::operator==(const BigInt &bi) const 
	{
		return hi == bi.hi && lo == bi.lo;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline bool BigInt<BaseUint>::operator!=(const BigInt &bi) const 
	{
		return !(*this == bi);
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline bool BigInt<BaseUint>::operator<(const BigInt &bi) const
	{
		BaseUint negBit = (BaseUint(1) << BaseUint(sizeof(BaseUint)*8-1));
		
		bool neg   = (hi    & negBit) != 0;
		bool biNeg = (bi.hi & negBit) != 0;
		
		if (neg != biNeg) return neg;
		
		BigInt<BaseUint> a = (neg ? -(*this) : *this);
		BigInt<BaseUint> b = (neg ? -bi : bi);
		
		return ((a.hi == b.hi) ? a.lo < b.lo : a.hi < b.hi);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline bool BigInt<BaseUint>::operator>(const BigInt &bi) const
	{
		return (*this != bi) && (bi < (*this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline bool BigInt<BaseUint>::operator<=(const BigInt &bi) const
	{
		return (*this == bi) || ((*this) < bi);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline bool BigInt<BaseUint>::operator>=(const BigInt &bi) const
	{
		return (*this == bi) || (bi < (*this));
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    bool BigInt<BaseUint>::operator!() const 
    {
		return hi == 0 && lo == 0;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigInt<BaseUint> BigInt<BaseUint>::operator-() const 
    {
		BigUint<BaseUint> ret = (*this);
		
		ret = ~ret + 1;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigInt<BaseUint> BigInt<BaseUint>::operator~() const 
    {
		BigInt<BaseUint> ret;
		ret.lo  = ~lo;
		ret.hi  = ~hi;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigInt<BaseUint> &BigInt<BaseUint>::operator++() 
    {
		if (++lo == 0) ++hi;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigInt<BaseUint> &BigInt<BaseUint>::operator--() 
    {
		if (lo-- == 0) --hi;
		
    	return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> &BigInt<BaseUint>::operator+=(const BigInt &bi)
	{
		BaseUint cpyLo = lo;

    	lo += bi.lo;
    	hi += bi.hi;

		if (lo < cpyLo)
			hi = hi+1;

    	return *this;
	}
	
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> &BigInt<BaseUint>::operator-=(const BigInt &bi)
	{
		return (*this) += -bi;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> &BigInt<BaseUint>::operator*=(const BigInt &bi)
	{
		if (bi.lo == 0 && bi.hi == 0) 
		{
			hi = 0;
			lo = 0;
		} 
		else if (bi.lo != 1 || bi.hi != 0) 
		{
			BigInt<BaseUint> a(*this);
    		BigInt<BaseUint> t = bi;

    		lo = 0;
    		hi = 0;

    		for (unsigned int i = 0; i < sizeof(BaseUint)*2*8; ++i) 
    		{
				if ((t.lo & 1) != 0)
            		*this += a;
				
        		t >>= 1;
				a <<= 1;
    		}
		}

    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> &BigInt<BaseUint>::operator|=(const BigInt &bi) 
	{
    	hi |= bi.hi;
    	lo |= bi.lo;
    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> &BigInt<BaseUint>::operator&=(const BigInt &bi) 
    {
		hi &= bi.hi;
    	lo &= bi.lo;
    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigInt<BaseUint> &BigInt<BaseUint>::operator^=(const BigInt &bi) 
    {
		hi ^= bi.hi;
    	lo ^= bi.lo;
    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> &BigInt<BaseUint>::operator/=(const BigInt &bi) 
	{
		BigInt<BaseUint> rem;
		priv::divide(*this, bi, *this, rem);
		
		return *this;
    }

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> &BigInt<BaseUint>::operator%=(const BigInt &b) 
	{
		BigInt<BaseUint> q;
		priv::divide(*this, b, q, *this);
		return *this;
    }

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigInt<BaseUint> &BigInt<BaseUint>::operator<<=(const BigInt& rhs) 
    {
		if (rhs < 0) return (*this) >>= -rhs;
    	
		BaseUint n = rhs.lo;

		if (n >= sizeof(BigInt<BaseUint>)*8) 
		{
			hi = 0;
			lo = 0;
		} 
		else 
		{
			const BaseUint halfsize = BaseUint(sizeof(BaseUint)*8);

    		if (n >= halfsize)
    		{
        		n -= halfsize;
        		hi = lo;
        		lo = 0;
    		}

    		if (n != 0)
    		{
        		hi <<= n;

				const BaseUint mask(~(BaseUint(-1) >> n));

        		hi |= (lo & mask) >> (halfsize - n);

        		lo <<= n;
    		}
		}

    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigInt<BaseUint> &BigInt<BaseUint>::operator>>=(const BigInt<BaseUint> &rhs) 
    {
		if (rhs < 0) return (*this) <<= -rhs;
    	
		BaseUint n = rhs.lo;

		if (n >= sizeof(BigInt<BaseUint>)*8) 
		{
			hi = 0;
			lo = 0;
		} 
		else 
		{
			const BaseUint halfsize = BaseUint(sizeof(BaseUint)*8);

    		if (n >= halfsize)
    		{
        		n -= halfsize;
        		lo = hi;
        		hi = 0;
    		}

    		if (n != 0) 
    		{
        		lo >>= n;

				const BaseUint mask(~(BaseUint(-1) << n));

        		lo |= (hi & mask) << (halfsize - n);

        		hi >>= n;
    		}
		}

    	return *this;
	}
		
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator+(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret += bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator-(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret -= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator*(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret *= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator/(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret /= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator|(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret |= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator&(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret &= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator^(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret ^= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator%(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret %= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator>>(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret >>= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	inline BigInt<BaseUint> BigInt<BaseUint>::operator<<(const BigInt &bi) const
	{
		BigInt<BaseUint> ret = (*this);
		ret <<= bi;
		return ret;
	}
	
	//////////////////////////////////////////////////////////////////////////
	template<class BaseUint>
	const BaseUint &BigInt<BaseUint>::getLoUint() const
	{
		return lo;
	}
	
	//////////////////////////////////////////////////////////////////////////
	template<class BaseUint>
	const BaseUint &BigInt<BaseUint>::getHiUint() const
	{
		return hi;
	}
}

namespace std
{
    template<typename,typename>
    class basic_ostream;
    
    template<typename,typename>
    class basic_istream;
}

////////////////////////////////////////////////////////////
template<class BaseUint,class CharT,class CharTraitT>
inline std::basic_ostream<CharT,CharTraitT> &operator<<(std::basic_ostream<CharT,CharTraitT> &out, const fm::BigInt<BaseUint> &bi)
{
	return out << bi.toString();
}

////////////////////////////////////////////////////////////
template<class BaseUint,class CharT,class CharTraitT>
inline std::basic_istream<CharT,CharTraitT> &operator>>(std::basic_istream<CharT,CharTraitT> &in, fm::BigInt<BaseUint> &bi)
{
	std::string str;
	in>>str;
	
	bi = fm::BigInt<BaseUint>(str);
	
	return in;
}

#endif // FRONTIER_BIGINT_INL_INCLUDED
