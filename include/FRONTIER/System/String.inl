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
#ifndef FRONTIER_STRING_INL_INCLUDED
#define FRONTIER_STRING_INL_INCLUDED

namespace fm
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		unsigned short getTrailingBytes(fm::Uint8 u);

		/////////////////////////////////////////////////////////////
		template<class InputIterator>
		inline bool isLegalUTF8(InputIterator source,int length)
		{
			fm::Uint8 a;
			InputIterator srcptr = source+length;

			if (length == 4)
				if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
					return false;

			if (length >= 3)
				if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
					return false;

			if (length >= 2)
			{
				if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
					return false;

				if (*source == 0xE0)
				{
					if (a < 0xA0) return false;
				}
				else if (*source == 0xED)
				{
					if (a < 0x9F) return false;
				}
				else if (*source == 0xF0)
				{
					if (a < 0x90) return false;
				}
				else if (*source == 0xF4)
				{
					if (a < 0x8F) return false;
				}
				else
					if (a < 0x80) return false;
			}


			if (length >= 1)
				if (*source >= 0x80 && *source < 0xC2)
					return false;

			return *source <= 0xF4;
		}

		/////////////////////////////////////////////////////////////
		fm::Uint32 getOffsetUTF8(fm::Uint8 u);
	}

	/////////////////////////////////////////////////////////////
	template <fm::Size byteCount>
	inline String String::fromUtf8(const char (&bytes)[byteCount],fm::Uint32 invalidSign)
	{
		return fromUtf8<const char*>((const char *)bytes,(const char *)bytes+byteCount-1,invalidSign);
	}

	/////////////////////////////////////////////////////////////
	template <typename InputIterator8bit>
	inline String String::fromUtf8(InputIterator8bit first, InputIterator8bit last,fm::Uint32 invalidSign)
	{
		String ret;

		while (first < last)
		{
			fm::Uint32 c = 0;
			unsigned short bytes = priv::getTrailingBytes(*first);

			if (bytes >= last-first)
			{
				ret += invalidSign;
				break;
			}

			if (!priv::isLegalUTF8(first,bytes+1))
			{
				ret += invalidSign;
				first += bytes+1;
				continue;
			}

			switch (bytes)
			{
				case 5: c += (fm::Uint8)*first++; c <<= 6;
				case 4: c += (fm::Uint8)*first++; c <<= 6;
				case 3: c += (fm::Uint8)*first++; c <<= 6;
				case 2: c += (fm::Uint8)*first++; c <<= 6;
				case 1: c += (fm::Uint8)*first++; c <<= 6;
				case 0: c += (fm::Uint8)*first++;
			}

			c -= priv::getOffsetUTF8(bytes);


			if (c <= 0x0010FFFF)
			{
				if (c >= 0xD800 && c <= 0xDFFF)
				{
					ret += invalidSign;
				}
				else
					ret += c;
			}
			else
			{
				ret += invalidSign;
			}
		}

		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template <typename InputIterator8bit>
	inline String String::fromUtf16(InputIterator8bit first, InputIterator8bit last,fm::Uint32 invalidSign)
	{
		String ret;
		
		while (first != last) 
		{
			fm::Uint16 c = fm::Uint16(fm::Uint8(*first) << 8) + fm::Uint8(*(first+1));
			first += 2;
			
			if (c < 0xE000)
				ret += (fm::Uint32)c;
			else
			{
				if ((c & 0xfffffc00) == 0xd800 && first < last && (*first & 0xfffffc00) == 0xdc00)
					ret += (fm::Uint32(c) << 10) + fm::Uint8(*first++) - 0x35fdc00;
				else
					ret += invalidSign;
			}
		}
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template <typename InputIterator8bit>
	inline String String::fromUtf32(InputIterator8bit first, InputIterator8bit last,fm::Uint32 invalidSign)
	{
		(void)invalidSign;

		String ret;

		while (first != last)
		{
			fm::Uint32 c = 0;
			for(int i = 0;i<4;++i) c = (fm::Uint16(fm::Uint8(c)) << 8) + fm::Uint8(*first++);
			
			ret += c;
		}

		return ret;
	}
}

#endif // FRONTIER_STRING_INL_INCLUDED
