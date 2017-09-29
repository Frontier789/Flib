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
#ifndef FRONTIER_BIGUINT_INL_INCLUDED
#define FRONTIER_BIGUINT_INL_INCLUDED
#include <FRONTIER/System/BigInt.hpp>
#include <FRONTIER/System/BigUint.hpp>

namespace fm
{
	namespace priv 
	{
		template <typename T>
		void divideU(BigUint<T> n,BigUint<T> d,BigUint<T> &q,BigUint<T> &rem) 
		{
			if (d == 0)
			{
				rem = 0;
				q = -1;
			}
			else
			{
				BigUint<T> x    = 1;
				BigUint<T> quot = 0;

				while ((n >= d) && (((d >> (sizeof(BigUint<T>) * 8 - 1)) & 1) == 0)) 
				{
					x <<= 1;
					d <<= 1;
				}

				while (x != 0) 
				{
					if (n >= d) 
					{
						n -= d;
						quot |= x;
					}

					x >>= 1;
					d >>= 1;
				}

				q   = quot;
				rem = n;
			}
		}
	}
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint() : lo(0), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Uint8 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Uint16 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Uint32 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Uint64 u) : lo(u), hi(0)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Int8 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Int16 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Int32 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(Int64 i) : lo(i < 0 ? -i : i), hi(0)
	{
		if (i < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(float f) : lo(f < 0 ? -f : f), hi(0)
	{
		if (f < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(double d) : lo(d < 0 ? -d : d), hi(0)
	{
		if (d < 0) (*this) = -(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(const BigInt<BaseUint> &bi) : lo(bi.getLoUint()), hi(bi.getHiUint())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(const BigUint &bi) : lo(bi.lo), hi(bi.hi)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint>::BigUint(const std::string &str) : lo(0), hi(0)
	{
		if (str.length()) 
		{
			int base = 10;

			std::string::const_iterator i = str.begin();
			
			bool negative = false;
			
			if (*i == '-') 
			{
				negative = true;
				++i;
			}

			if (i != str.end()) 
			{
				if (*i == '0') // 052 -> base 8
				{
					base = 8;
					++i;
					
					if (i != str.end()) 
					{
						if (*i == 'x') // 0x52 -> base 16
						{
							base = 16;
							++i;
						}
					}
				}

				for (;i != str.end();++i) 
				{
					unsigned int n;
					const char ch = *i;
					
					if (ch == '\'' || ch == ' ') continue; // Allow separators

					if (ch >= 'A' && ch <= 'Z')  // A-Z
					{
						if (ch-'A' + 10 >= base) break;
						
						n = ch-'A' + 10;
					} 
					else if (ch >= 'a' && ch <= 'z') // a-z
					{
						if (ch-'a' + 10 >= base) break;
						
						n = ch-'a' + 10;
					} 
					else if (ch >= '0' && ch <= '9') // 0-9
					{
						if (ch-'0' >= base) break;
						
						n = ch-'0';
					} 
					else break;

					(*this) *= base;
					(*this) += n;
				}
			}
			
			if (negative) (*this) = -(*this);
		}
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	template<Size N>
	inline BigUint<BaseUint>::BigUint(const char (&str)[N])
	{
		(*this) = BigUint<BaseUint>(std::string(str));
	}
	
	namespace priv
	{
		template<class BigUintType>
		class ToInt
		{
		public:
			static int get(const BigUintType &b)
			{
				return ToInt<typename BigUintType::BaseType>::get(b.getLoUint());
			}
		};
		
		template<>
		class ToInt<Uint64>
		{
		public:
			static int get(const Uint64 &b)
			{
				return (int)b;
			}
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	std::string BigUint<BaseUint>::toString(unsigned int base) const
	{
		if (lo == 0 && hi == 0)
			return "0";

		std::string ret;
		
		if (base < 2)  base = 2;
		if (base > 37) base = 37;

    	BigUint<BaseUint> cpy(*this);

		while (cpy.lo != 0 || cpy.hi != 0) 
    	{
			BigUint<BaseUint> rem;
			priv::divideU(cpy, BigUint<BaseUint>(base), cpy, rem);
			
			int index = priv::ToInt<BigUint<BaseUint> >::get(rem);
			
			ret = "0123456789abcdefghijklmnopqrstuvwxyz"[index] + ret;
    	}

    	return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	bool BigUint<BaseUint>::operator==(const BigUint &bi) const 
	{
		return hi == bi.hi && lo == bi.lo;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	bool BigUint<BaseUint>::operator!=(const BigUint &bi) const 
	{
		return !(*this == bi);
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	bool BigUint<BaseUint>::operator<(const BigUint &bi) const
	{
		return ((hi == bi.hi) ? lo < bi.lo : hi < bi.hi);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	bool BigUint<BaseUint>::operator>(const BigUint &bi) const
	{
		return (*this != bi) && (bi < (*this));
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	bool BigUint<BaseUint>::operator<=(const BigUint &bi) const
	{
		return (*this == bi) || ((*this) < bi);
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	bool BigUint<BaseUint>::operator>=(const BigUint &bi) const
	{
		return (*this == bi) || (bi < (*this));
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    bool BigUint<BaseUint>::operator!() const 
    {
		return hi == 0 && lo == 0;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigUint<BaseUint> BigUint<BaseUint>::operator-() const 
    {
		BigUint<BaseUint> ret = *this;
		
		ret = ~ret + 1;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigUint<BaseUint> BigUint<BaseUint>::operator~() const 
    {
		BigUint<BaseUint> ret;
		ret.lo  = ~lo;
		ret.hi  = ~hi;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigUint<BaseUint> &BigUint<BaseUint>::operator++() 
    {
		if (++lo == 0) ++hi;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigUint<BaseUint> &BigUint<BaseUint>::operator--() 
    {
		if (lo-- == 0) --hi;
		
    	return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> &BigUint<BaseUint>::operator+=(const BigUint &bi)
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
	BigUint<BaseUint> &BigUint<BaseUint>::operator-=(const BigUint &bi)
	{
		return (*this) += -bi;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> &BigUint<BaseUint>::operator*=(const BigUint &bi)
	{
		if (bi.lo == 0 && bi.hi == 0) 
		{
			hi = 0;
			lo = 0;
		} 
		else if (bi.lo != 1 || bi.hi != 0) 
		{
			BigUint<BaseUint> a(*this);
    		BigUint<BaseUint> t = bi;

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
	BigUint<BaseUint> &BigUint<BaseUint>::operator|=(const BigUint &bi) 
	{
    	hi |= bi.hi;
    	lo |= bi.lo;
    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> &BigUint<BaseUint>::operator&=(const BigUint &bi) 
    {
		hi &= bi.hi;
    	lo &= bi.lo;
    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigUint<BaseUint> &BigUint<BaseUint>::operator^=(const BigUint &bi) 
    {
		hi ^= bi.hi;
    	lo ^= bi.lo;
    	return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> &BigUint<BaseUint>::operator/=(const BigUint &bi) 
	{
		BigUint<BaseUint> rem;
		priv::divideU(*this, bi, *this, rem);
		
		return *this;
    }

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> &BigUint<BaseUint>::operator%=(const BigUint &b) 
	{
		BigUint<BaseUint> q;
		priv::divideU(*this, b, q, *this);
		return *this;
    }

	/////////////////////////////////////////////////////////////
	template<class BaseUint>
    BigUint<BaseUint> &BigUint<BaseUint>::operator<<=(const BigUint& rhs) 
    {
		BaseUint n = rhs.lo;

		if (n >= sizeof(BigUint<BaseUint>)*8) 
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
    BigUint<BaseUint> &BigUint<BaseUint>::operator>>=(const BigUint<BaseUint> &rhs) 
    {
		BaseUint n = rhs.lo;

		if (n >= sizeof(BigUint<BaseUint>)*8) 
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
	BigUint<BaseUint> BigUint<BaseUint>::operator+(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret += bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator-(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret -= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator*(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret *= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator/(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret /= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator|(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret |= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator&(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret &= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator^(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret ^= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator%(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret %= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator>>(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret >>= bi;
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<class BaseUint>
	BigUint<BaseUint> BigUint<BaseUint>::operator<<(const BigUint &bi) const
	{
		BigUint<BaseUint> ret = (*this);
		ret <<= bi;
		return ret;
	}
	
	//////////////////////////////////////////////////////////////////////////
	template<class BaseUint>
	const BaseUint &BigUint<BaseUint>::getLoUint() const
	{
		return lo;
	}
	
	//////////////////////////////////////////////////////////////////////////
	template<class BaseUint>
	const BaseUint &BigUint<BaseUint>::getHiUint() const
	{
		return hi;
	}
}

#include <iosfwd>

////////////////////////////////////////////////////////////
template<class BaseUint,class CharT,class CharTraitT>
inline std::basic_ostream<CharT,CharTraitT> &operator<<(std::basic_ostream<CharT,CharTraitT> &out, const fm::BigUint<BaseUint> &bi)
{
	return out << bi.toString();
}

////////////////////////////////////////////////////////////
template<class BaseUint,class CharT,class CharTraitT>
inline std::basic_istream<CharT,CharTraitT> &operator>>(std::basic_istream<CharT,CharTraitT> &in, fm::BigUint<BaseUint> &bi)
{
	std::string str;
	in>>str;
	
	bi = fm::BigInt<BaseUint>(str);
	
	return in;
}

#endif // FRONTIER_BIGUINT_INL_INCLUDED
