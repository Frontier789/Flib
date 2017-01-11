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
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/System/String.hpp>
#include <cstdlib>
#include <cwchar>
#include <locale>

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		fm::Uint32 FRONTIER_API utf32FromAnsi(char character,const std::locale &loc = std::locale())
		{
        #ifdef FRONTIER_OS_WINDOWS

            (void)loc;

            std::mbstate_t mbt;
            wchar_t wc = 0;
            std::mbrtowc(&wc,&character,1,&mbt);

            return (fm::Uint32)wc;

        #else

            return (fm::Uint32)std::use_facet< std::ctype<wchar_t> >(loc).widen(character);

        #endif // FRONTIER_OS_WINDOWS
		}

		/////////////////////////////////////////////////////////////
		fm::Uint32 FRONTIER_API utf32FromWide(wchar_t character)
		{
			return (fm::Uint32)character;
		}

		/////////////////////////////////////////////////////////////
		unsigned short FRONTIER_API getTrailingBytes(fm::Uint8 u)
		{
			if (u < 192) return 0;
			if (u < 224) return 1;
			if (u < 240) return 2;
			if (u < 248) return 3;
			if (u < 252) return 4;
			return 5;
		}

		/////////////////////////////////////////////////////////////
		fm::Uint32 FRONTIER_API getOffsetUTF8(fm::Uint8 u)
		{
			if (u == 0) return 0x00000000UL;
			if (u == 1) return 0x00003080UL;
			if (u == 2) return 0x000E2080UL;
			if (u == 3) return 0x03C82080UL;
			if (u == 4) return 0xFA082080UL;
			return 0x82082080UL;
		}
	}

	fm::Size String::npos = fm::Size(-1);

	/////////////////////////////////////////////////////////////
	String::String()
	{

	}

	/////////////////////////////////////////////////////////////
	String::String(String::const_iterator begIt,String::const_iterator endIt)
	{
		while (begIt != endIt)
			m_str += *begIt++;
	}

	/////////////////////////////////////////////////////////////
	String::String(fm::Size charCount,fm::Uint32 character) : m_str(charCount,character)
	{

	}

	/////////////////////////////////////////////////////////////
	String::String(char character,const std::locale &loc)
	{
		m_str += priv::utf32FromAnsi(character,loc);
	}

	/////////////////////////////////////////////////////////////
	String::String(const char *text,const std::locale &loc)
	{
		if (text)
			while (*text)
			{
				m_str += priv::utf32FromAnsi(*text,loc);
				text++;
			}
	}

	/////////////////////////////////////////////////////////////
	String::String(const std::string &text,const std::locale &loc)
	{
		C(text.length())
			m_str += priv::utf32FromAnsi(text[i],loc);
	}

	/////////////////////////////////////////////////////////////
	String::String(wchar_t character)
	{
		m_str += priv::utf32FromWide(character);
	}

	/////////////////////////////////////////////////////////////
	String::String(const wchar_t *text)
	{
		if (text)
			while (*text)
			{
				m_str += priv::utf32FromWide(*text);
				text++;
			}
	}

	/////////////////////////////////////////////////////////////
	String::String(const std::wstring &text)
	{
		C(text.length())
			m_str += priv::utf32FromWide(text[i]);
	}

	/////////////////////////////////////////////////////////////
	String::String(fm::Uint32 character)
	{
		m_str += character;
	}

	/////////////////////////////////////////////////////////////
	String::String(const fm::Uint32 *text)
	{
		if (text)
		{
			const fm::Uint32 *p = text;
			while (*p) p++;

			m_str.assign(text,p);
		}
	}

	/////////////////////////////////////////////////////////////
	String::String(const fm::Uint32 *text,fm::Size charCount)
	{
		if (text)
			m_str.assign(text,text+charCount);
		else
            resize(charCount);
	}

	/////////////////////////////////////////////////////////////
	String::String(const std::basic_string<fm::Uint32> &text) : m_str(text)
	{

	}

	/////////////////////////////////////////////////////////////
	std::string String::str(const std::locale &loc) const
	{
		fm::Size s = m_str.size();

		std::string ret;
		ret.reserve(s+1);

		C(s)
		{
        #ifdef FRONTIER_OS_WINDOWS
            (void)loc;
			char *c = new char[MB_CUR_MAX];

            std::mbstate_t mbt;
			if (std::wcrtomb(c, wchar_t(m_str[i]),&mbt) != std::size_t(-1))
				ret += *c;

			delete[] c;
        #else

			char c = std::use_facet< std::ctype<wchar_t> >(loc).narrow(m_str[i],'\0');
			if (c != '\0' || m_str[i] == 0)
                ret += m_str[i];

        #endif // FRONTIER_OS_WINDOWS
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
	std::wstring String::wstr() const
	{
		std::wstring ret;
		fm::Size s = m_str.size();

		if (sizeof(wchar_t) == 4)
		{
			ret.resize(s);

			C(s)
				ret[i] = (wchar_t)m_str[i];
		}
		else
		{
			C(s)
			{
				fm::Uint32 c = m_str[i];
				if ((c <= 0xFFFF) && ((c < 0xD800) || (c > 0xDFFF)))
				{
					ret += (wchar_t)c;
				}
			}
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
	String String::fromUtf8(const char *bytes,fm::Size byteCount,fm::Uint32 invalidSign)
	{
		return fromUtf8<const char *>(bytes,bytes+byteCount+1,invalidSign);
	}

	/////////////////////////////////////////////////////////////
	String String::fromUtf8(const std::string &str,fm::Uint32 invalidSign)
	{
		return fromUtf8<std::string::const_iterator>(str.begin(),str.end(),invalidSign);
	}


	/////////////////////////////////////////////////////////////
	const fm::Uint8  FRONTIER_FIRSTBYTEMARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
    const fm::Uint32 FRONTIER_BYTEMASK = 0xBF;
    const fm::Uint32 FRONTIER_BYTEMARK = 0x80;

	/////////////////////////////////////////////////////////////
	std::basic_string<fm::Uint8> String::toUtf8(fm::Uint32 invalidSign) const
	{
		std::basic_string<fm::Uint8> ret;

		C(m_str.size())
		{
			fm::Uint32 ch = m_str[i];
			fm::Uint8  bytes = 0;


        	if ((ch > 0x0010FFFF) || ((ch >= 0xD800) && (ch <= 0xDBFF)))
			{
				// Invalid character
				if (invalidSign)
					ret += invalidSign;
			}
			else
			{
				     if (ch < 0x80)        bytes = 1;
		        else if (ch < 0x800)       bytes = 2;
		        else if (ch < 0x10000)     bytes = 3;
		        else if (ch <= 0x0010FFFF) bytes = 4;
		        else                       bytes = 3,
		                                   ch = invalidSign;

		        C(bytes)
		        	ret.push_back('a');

		        switch (bytes)
		        {
		            case 4: ret[ret.size()-bytes+3] = ((ch | FRONTIER_BYTEMARK) & FRONTIER_BYTEMASK); ch >>= 6;
		            case 3: ret[ret.size()-bytes+2] = ((ch | FRONTIER_BYTEMARK) & FRONTIER_BYTEMASK); ch >>= 6;
		            case 2: ret[ret.size()-bytes+1] = ((ch | FRONTIER_BYTEMARK) & FRONTIER_BYTEMASK); ch >>= 6;
		            case 1: ret[ret.size()-bytes+0] =  (ch | FRONTIER_FIRSTBYTEMARK[bytes]);
	        	}
			}
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
	std::basic_string<fm::Uint16> String::toUtf16(fm::Uint32 invalidSign) const
	{
		std::basic_string<fm::Uint16> ret;
		fm::Size s = m_str.size();
		ret.reserve(s);

		C(s)
		{
			fm::Uint32 c = m_str[i];

			// check if simply convertible
			if (c < 0xFFFF)
			{
				// check if the value is reserved
				if (c < 0xD800 || c > 0xDFFF)
					ret += c;
				else
					if (invalidSign)
						ret += invalidSign;
			}
			// check if valid
			else if (c <= 0x0010FFFF)
			{
				c -= 0x0010000;
				ret += Uint16((c >> 10)	 + 0xD800);
				ret += Uint16((c & 0x3FFUL) + 0xDC00);
			}
			else
				if (invalidSign)
					ret += invalidSign;
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
	std::basic_string<fm::Uint32> String::toUtf32(fm::Uint32 invalidSign) const
	{
		(void)invalidSign;

		return m_str;
	}

	/////////////////////////////////////////////////////////////
	fm::Size String::size() const
	{
		return m_str.size();
	}

	/////////////////////////////////////////////////////////////
	fm::Size String::length() const
	{
		return m_str.size();
	}

	/////////////////////////////////////////////////////////////
	fm::Uint32 &String::at(fm::Size index)
	{
		return m_str[index];
	}

	/////////////////////////////////////////////////////////////
	const fm::Uint32 &String::at(fm::Size index) const
	{
		return m_str[index];
	}

	/////////////////////////////////////////////////////////////
	String::reference String::erase(fm::Size pos,fm::Size len)
	{
		m_str.erase(pos,len);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	String::reference String::insert(fm::Size pos,const String &str)
	{
		fm::Size len = size();

		if (pos > len) pos = len;

		m_str.insert(pos,str.m_str);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	String::reference String::replace(fm::Size pos,const String &str,fm::Size len)
	{
		fm::Size s = size();
		fm::Size s2 = str.size();

		if (s-pos < len)
			len = s-pos;

		if (s2 < len)
			len = s2;

		C(len)
			m_str[pos+i] = str[i];

		return *this;
	}

	/////////////////////////////////////////////////////////////
	String::reference String::resize(fm::Size size,fm::Uint32 paddingChar)
	{
		m_str.resize(size,paddingChar);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	String::reference String::push_back(const fm::String &str)
	{
		return insert(size(),str);
	}

	/////////////////////////////////////////////////////////////
	String::reference String::push_front(const fm::String &str)
	{
		return insert(0,str);
	}

	/////////////////////////////////////////////////////////////
	String::reference String::pop_back()
	{
		if (size())
			erase(size()-1,1);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	String::reference String::pop_front()
	{
		if (size())
			erase(0,1);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	fm::Size String::find(const String &str,fm::Size pos,fm::Size len) const
	{
		fm::Size s = size();
		fm::Size s2 = str.size();

		if (pos + s2 >= s + 1) return fm::String::npos;

		if (len > s+1-pos-s2)
			len = s+1-pos-s2;

		Cx(len)
		{
			bool ok = true;

			Cy(s2)
				if (m_str[pos+x+y] != str[y])
				{
					ok = false;
					break;
				}

			if (ok)
				return pos+x;
		}

		return fm::String::npos;
	}

	/////////////////////////////////////////////////////////////
	fm::Size String::find_last_of(const String &str,fm::Size pos,fm::Size len) const
	{
		fm::Size s = size();
		fm::Size s2 = str.size();

		if (len > s+1-pos-s2)
			len = s+1-pos-s2;

		Cx(len)
		{
			bool ok = true;

			Cy(s2)
				if (m_str[pos+len-1-x+y] != str[y])
				{
					ok = false;
					break;
				}

			if (ok)
				return pos+len-1-x;
		}

		return fm::String::npos;
	}

	/////////////////////////////////////////////////////////////
	fm::Size String::count(const String &str,fm::Size pos,fm::Size len,bool canOverlap) const
	{
		if (!str.size())
			return fm::String::npos;

		fm::Size ret = 0;

		for (fm::Size i=pos;i<size() && i<pos+len;i++)
		{
			bool k = true;
			Cx(str.size())
			{
				if (i+x>=size() || i+x>=pos+len)
					i = size(),
					x = str.size(),
					k = false;
				else if (m_str[i+x] != str[x])
					k = false;
			}

			if (k)
				ret++;

			if (!canOverlap)
				i += str.size()-1;
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
	String String::substr(fm::Size pos,fm::Size len) const
	{
		String ret;
		ret.m_str = m_str.substr(pos,len);

		return ret;
	}

	/////////////////////////////////////////////////////////////
	void String::clear()
	{
		m_str = std::basic_string<fm::Uint32>();
	}

	/////////////////////////////////////////////////////////////
	String::iterator String::begin()
	{
		return m_str.begin();
	}

	/////////////////////////////////////////////////////////////
	String::const_iterator String::begin() const
	{
		return m_str.begin();
	}

	/////////////////////////////////////////////////////////////
	String::iterator String::end()
	{
		return m_str.end();
	}

	/////////////////////////////////////////////////////////////
	String::const_iterator String::end() const
	{
		return m_str.end();
	}

	/////////////////////////////////////////////////////////////
	String::operator std::string() const
	{
		return str();
	}

	/////////////////////////////////////////////////////////////
	String::operator std::wstring() const
	{
		return wstr();
	}

	/////////////////////////////////////////////////////////////
	String::reference String::operator+=(const String &str)
	{
		m_str += str.m_str;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	const fm::Uint32 &String::operator[](fm::Size pos) const
	{
		return m_str[pos];
	}

	/////////////////////////////////////////////////////////////
	fm::Uint32 &String::operator[](fm::Size pos)
	{
		return m_str[pos];
	}

	/////////////////////////////////////////////////////////////
	fm::String &String::swap(fm::String &s)
	{
		m_str.swap(s.m_str);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	bool operator==(const String &str1, const String &str2)
	{
		fm::Size s1 = str1.size();
		fm::Size s2 = str2.size();

		if (s1 == s2)
		{
			String::const_iterator it1 = str1.begin();
			String::const_iterator it2 = str2.begin();

			while (it1 != str1.end())
			{
				if (*it1 != *it2)
					return false;

				++it1;
				++it2;
			}

			return true;
		}

		return false;
	}

	/////////////////////////////////////////////////////////////
	bool operator!=(const String &str1, const String &str2)
	{
		return !(str1 == str2);
	}

	/////////////////////////////////////////////////////////////
	bool operator<(const String &str1, const String &str2)
	{
		fm::Size s1 = str1.size();
		fm::Size s2 = str2.size();

		String::const_iterator it1 = str1.begin();
		String::const_iterator it2 = str2.begin();

		while (it1 != str1.end() && it2 != str2.end())
		{
			if (*it1 != *it2)
				return *it1 < *it2;

			++it1;
			++it2;
		}

		return s1 < s2;
	}

	/////////////////////////////////////////////////////////////
	bool operator>(const String &str1, const String &str2)
	{
		return str2<str1;
	}

	/////////////////////////////////////////////////////////////
	String operator+(const String &str1, const String &str2)
	{
		String ret;
		ret.insert(0,str1);
		ret.insert(str1.size(),str2);

		return ret;
	}

	/////////////////////////////////////////////////////////////
	String toString(char num,fm::Size base)
	{
		return toString((long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(unsigned char num,fm::Size base)
	{
		return toString((unsigned long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(short num,fm::Size base)
	{
		return toString((long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(unsigned short num,fm::Size base)
	{
		return toString((unsigned long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(int num,fm::Size base)
	{
		return toString((long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(unsigned int num,fm::Size base)
	{
		return toString((unsigned long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(long num,fm::Size base)
	{
		return toString((long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(unsigned long num,fm::Size base)
	{
		return toString((long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(long long num,fm::Size base)
	{
		if (num < 0)
			return "-"+toString((unsigned long long)(-num),base);

		return toString((unsigned long long)num,base);
	}

	/////////////////////////////////////////////////////////////
	String toString(unsigned long long ull,fm::Size base)
	{
	    if (!ull) return "0";
	    
	    const char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

		String ret;
		while (ull)
		{
			fm::Uint32 index = ull%base;
			
			if (index < sizeof(chars)/sizeof(*chars))
				ret = chars[index] + ret,
			
			ull /= base;
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
}
