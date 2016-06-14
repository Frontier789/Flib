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
#ifndef FRONTIER_STRING_HPP_INCLUDED
#define FRONTIER_STRING_HPP_INCLUDED
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_STRING
#include <locale>
#include <string>

namespace fm
{
    typedef Uint32 Char;

	/////////////////////////////////////////////////////////////
	/// @brief A string class that automatically converts input to utf32 encoding
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API String
	{
		std::basic_string<fm::Uint32> m_str; ///< The internal storage string
	public:
		typedef String &reference;
		typedef const String &const_reference;

		typedef std::basic_string<fm::Uint32>::iterator iterator;
		typedef std::basic_string<fm::Uint32>::const_iterator const_iterator;

		static fm::Size npos; ///< The biggest representable position

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		String();

		/////////////////////////////////////////////////////////////
		/// @brief Construct from a range of elements
		///
		/// @param begIt The first item to include
		/// @param endIt The first item not to include
		///
		/////////////////////////////////////////////////////////////
		String(const_iterator begIt,const_iterator endIt);

		/////////////////////////////////////////////////////////////
		/// @brief Construct containing n identical characters
		///
		/// @param charCount The number of characters
		/// @param character The character to use
		///
		/////////////////////////////////////////////////////////////
		String(fm::Size charCount,fm::Uint32 character);

		/* ansi */

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a single character
		///
		/// @param character The character
		/// @param loc The locale to use
		///
		/////////////////////////////////////////////////////////////
		String(char character,const std::locale &loc = std::locale());

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from an ansi text
		///
		/// @param text The text
		/// @param loc The locale to use
		///
		/////////////////////////////////////////////////////////////
		String(const char *text,const std::locale &loc = std::locale());

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from an ansi text
		///
		/// @param text The text
		/// @param loc The locale to use
		///
		/////////////////////////////////////////////////////////////
		String(const std::string &text,const std::locale &loc = std::locale());

		/* wide */

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a single wide character
		///
		/// @param character The wide character
		///
		/////////////////////////////////////////////////////////////
		String(wchar_t character);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a wide text
		///
		/// @param text The wide text
		///
		/////////////////////////////////////////////////////////////
		String(const wchar_t *text);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a wide text
		///
		/// @param text The wide text
		///
		/////////////////////////////////////////////////////////////
		String(const std::wstring &text);

		/* utf32 */

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a single utf32 character
		///
		/// @param character The utf32 character
		///
		/////////////////////////////////////////////////////////////
		String(fm::Uint32 character);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf32 text
		///
		/// @param text The utf32 text
		///
		/////////////////////////////////////////////////////////////
		String(const fm::Uint32 *text);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf32 text
		///
		/// @param text The utf32 text
		/// @param charCount The number of characters to copy
		///
		/////////////////////////////////////////////////////////////
		String(const fm::Uint32 *text,fm::Size charCount);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf32 text
		///
		/// @param text The utf32 text
		///
		/////////////////////////////////////////////////////////////
		String(const std::basic_string<fm::Uint32> &text);

		/* static load */

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf8 text
		///
		/// @param first The first used character
		/// @param last The first character that is not used
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The loaded string
		///
		/////////////////////////////////////////////////////////////
		template <typename InputIterator>
		static String fromUtf8(InputIterator first, InputIterator last,fm::Uint32 invalidSign = 0xFFFD);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf8 string literal
		///
		/// @param bytes The string literal (e.g. "some UTF8 text")
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The loaded string
		///
		/////////////////////////////////////////////////////////////
		template <fm::Size byteCount>
		static String fromUtf8(const char (&bytes)[byteCount],fm::Uint32 invalidSign = 0xFFFD);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf8 text
		///
		/// @param bytes A pointer to the beginning of the utf8 encoded text
		/// @param byteCount The number of bytes in the text (not including the terminating 0 if any)
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The loaded string
		///
		/////////////////////////////////////////////////////////////
		static String fromUtf8(const char *bytes,fm::Size byteCount,fm::Uint32 invalidSign);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf8 text
		///
		/// @param str The uft8 encoded string
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The loaded string
		///
		/////////////////////////////////////////////////////////////
		static String fromUtf8(const std::string &str,fm::Uint32 invalidSign = 0xFFFD);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf16 text
		///
		/// @param first The first used character
		/// @param last The first character that is not used
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The loaded string
		///
		/////////////////////////////////////////////////////////////
		template <typename InputIterator>
		static String fromUtf16(InputIterator first, InputIterator last,fm::Uint32 invalidSign = 0xFFFD);

		/////////////////////////////////////////////////////////////
		/// @brief Load the content of the string from a utf32 text
		///
		/// @param first The first used character
		/// @param last The first character that is not used
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The loaded string
		///
		/////////////////////////////////////////////////////////////
		template <typename InputIterator>
		static String fromUtf32(InputIterator first, InputIterator last,fm::Uint32 invalidSign = 0xFFFD);

		/* convert */

		/////////////////////////////////////////////////////////////
		/// @brief Convert the string to ansi string
		///
		/// @param loc The locale to use
		///
		/// @return The constructed string
		///
		/////////////////////////////////////////////////////////////
		std::string str(const std::locale &loc = std::locale()) const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert the string to wide string
		///
		/// @return The constructed string
		///
		/////////////////////////////////////////////////////////////
		std::wstring wstr() const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert the string to utf8 string
		///
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The constructed string
		///
		/////////////////////////////////////////////////////////////
		std::basic_string<fm::Uint8> toUtf8(fm::Uint32 invalidSign = 0xFFFD) const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert the string to utf16 string
		///
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The constructed string
		///
		/////////////////////////////////////////////////////////////
		std::basic_string<fm::Uint16> toUtf16(fm::Uint32 invalidSign = 0xFFFD) const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert the string to utf32 string
		///
		/// @param invalidSign The codepoint to insert if the character is invalid (0 means omit)
		///
		/// @return The constructed string
		///
		/////////////////////////////////////////////////////////////
		std::basic_string<fm::Uint32> toUtf32(fm::Uint32 invalidSign = 0xFFFD) const;

		/* operations */

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of characters in the string
		///
		/// @return The number of characters
		///
		/////////////////////////////////////////////////////////////
		fm::Size size() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of characters in the string
		///
		/// @return The number of characters
		///
		/////////////////////////////////////////////////////////////
		fm::Size length() const;

		/////////////////////////////////////////////////////////////
		/// @brief Access element
		///
		/// @param index The index of the character
		///
		/// @return Reference to the character
		///
		/////////////////////////////////////////////////////////////
		fm::Uint32 &at(fm::Size index);

		/////////////////////////////////////////////////////////////
		/// @brief Access element
		///
		/// @param index The index of the character
		///
		/// @return Reference to the character
		///
		/////////////////////////////////////////////////////////////
		const fm::Uint32 &at(fm::Size index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Erase a part of the string
		///
		/// @param pos The position of the first character to be erased
		/// @param len The number of characters to be erased
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference erase(fm::Size pos = 0,fm::Size len = String::npos);

		/////////////////////////////////////////////////////////////
		/// @brief Insert a string into the string
		///
		/// @param pos The position of the character before the string is to inserted
		/// @param str The string to be inserted
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference insert(fm::Size pos,const String &str);

		/////////////////////////////////////////////////////////////
		/// @brief Replace characters in the string
		///
		/// The number of characters replaced is
		/// min( length()-pos, str.length(), len )
		///
		/// @param pos The position of the first character that is to be replaced
		/// @param str The string to be inserted
		/// @param len The maximum number of characters to be replaced
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference replace(fm::Size pos,const String &str,fm::Size len = String::npos);

		/////////////////////////////////////////////////////////////
		/// @brief Resize the string
		///
		/// If the new size is smaller than the old, the string is truncated
		/// otherwise (new size)-size character of @a paddingChar is inserted at the end
		///
		/// @param size The new size
		/// @param paddingChar The character to use when the size is growing
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference resize(fm::Size size,fm::Uint32 paddingChar = 'a');

		/////////////////////////////////////////////////////////////
		/// @brief Append the string's back
		///
		/// @param str The string to insert at the end
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference push_back(const fm::String &str);

		/////////////////////////////////////////////////////////////
		/// @brief Append the string's front
		///
		/// @param str The string to insert at the beginning
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference push_front(const fm::String &str);

		/////////////////////////////////////////////////////////////
		/// @brief Pop the string's back
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference pop_back();

		/////////////////////////////////////////////////////////////
		/// @brief Pop the string's front
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference pop_front();

		/////////////////////////////////////////////////////////////
		/// @brief Perform a linear search in the string
		///
		/// @param str The string to be found
		/// @param pos The position of the first character to be examined
		/// @param len The maximum number of characters to be examined
		///
		/// @return The beginning of the first match (String::npos on no match)
		///
		/////////////////////////////////////////////////////////////
		fm::Size find(const String &str,fm::Size pos = 0,fm::Size len = String::npos) const;

		/////////////////////////////////////////////////////////////
		/// @brief Perform a linear search in the string in reverse order
		///
		/// @param str The string to be found
		/// @param pos The position of the first character to be examined
		/// @param len The maximum number of characters to be examined
		///
		/// @return The beginning of the first match (String::npos on no match)
		///
		/////////////////////////////////////////////////////////////
		fm::Size find_last_of(const String &str,fm::Size pos = 0,fm::Size len = String::npos) const;

		/////////////////////////////////////////////////////////////
		/// @brief Count the occurences of a string in the text
		///
		/// @param str The string to count
		/// @param pos The position of the first character to be examined
		/// @param len The maximum number of characters to be examined
		/// @param canOverlap Iff true the occurences are allowed to overlap
		///
		/// @return The number of matches
		///
		/////////////////////////////////////////////////////////////
		fm::Size count(const String &str,fm::Size pos = 0,fm::Size len = String::npos,bool canOverlap = true) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy a part of the string
		///
		/// The number of copied characters is
		/// min( length(), len )
		///
		/// @param pos The position of the first character to be copied
		/// @param len The number of characters to copy
		///
		/// @return The beginning of the first match (String::npos on no match)
		///
		/////////////////////////////////////////////////////////////
		String substr(fm::Size pos = 0,fm::Size len = String::npos) const;

		/* iterator */

		/////////////////////////////////////////////////////////////
		/// @brief Get a iterator to the beginning of the string
		///
		/// @return The iterator
		///
		/////////////////////////////////////////////////////////////
		iterator begin();

		/////////////////////////////////////////////////////////////
		/// @brief Get a iterator to the beginning of the string
		///
		/// @return The iterator
		///
		/////////////////////////////////////////////////////////////
		const_iterator begin() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a iterator to the end of the string
		///
		/// @return The iterator
		///
		/////////////////////////////////////////////////////////////
		iterator end();

		/////////////////////////////////////////////////////////////
		/// @brief Get a iterator to the end of the string
		///
		/// @return The iterator
		///
		/////////////////////////////////////////////////////////////
		const_iterator end() const;

		/* operators */

		/////////////////////////////////////////////////////////////
		/// @brief Convert the string to ansi string
		///
		/// @return The string
		///
		/////////////////////////////////////////////////////////////
		operator std::string() const;

		/////////////////////////////////////////////////////////////
		/// @brief Convert the string to wide string
		///
		/// @return The string
		///
		/////////////////////////////////////////////////////////////
		operator std::wstring() const;

		/////////////////////////////////////////////////////////////
		/// @brief Append the string
		///
		/// @param str The string to add
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator+=(const String &str);

		/////////////////////////////////////////////////////////////
		/// @brief Get a character of the string
		///
		/// @param pos The position of the character
		///
		/// @return The character
		///
		/////////////////////////////////////////////////////////////
		const fm::Uint32 &operator[](fm::Size pos) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a character of the string
		///
		/// @param pos The position of the character
		///
		/// @return The character
		///
		/////////////////////////////////////////////////////////////
		fm::Uint32 &operator[](fm::Size pos);

		/////////////////////////////////////////////////////////////
		/// @brief Swap the content of the string with another string
		///
		/// @param s The string to operate on
		///
		/// @return Reference to this string
		///
		/////////////////////////////////////////////////////////////
		fm::String &swap(fm::String &s);
	};

	/////////////////////////////////////////////////////////////
	/// @brief Check if two strings are equal
	///
	/// @param str1 The first string
	/// @param str2 The second string
	///
	/// @return True iff the two strings match
	///
	/////////////////////////////////////////////////////////////
	bool operator==(const String &str1, const String &str2);

	/////////////////////////////////////////////////////////////
	/// @brief chaeck two strings are different
	///
	/// @param str1 The first string
	/// @param str2 The second string
	///
	/// @return True iff the two strings don't match
	///
	/////////////////////////////////////////////////////////////
	bool operator!=(const String &str1, const String &str2);

	/////////////////////////////////////////////////////////////
	/// @brief Check if a string is alphabetically comes before an other
	///
	/// @param str1 The first string
	/// @param str2 The second string
	///
	/// @return True iff the the first string comes before the second
	///
	/////////////////////////////////////////////////////////////
	bool operator<(const String &str1, const String &str2);

	/////////////////////////////////////////////////////////////
	/// @brief Check if a string is alphabetically comes after an other
	///
	/// @param str1 The first string
	/// @param str2 The second string
	///
	/// @return True iff the the first string comes after the second
	///
	/////////////////////////////////////////////////////////////
	bool operator>(const String &str1, const String &str2);

	/////////////////////////////////////////////////////////////
	/// @brief Concatenate two strings
	///
	/// @param str1 The first string
	/// @param str2 The second string
	///
	/// @return The concatenated string
	///
	/////////////////////////////////////////////////////////////
	String operator+(const String &str1, const String &str2);

	String toString(char               num);
	String toString(unsigned char      num);
	String toString(short              num);
	String toString(unsigned short     num);
	String toString(int                num);
	String toString(unsigned int       num);
	String toString(long               num);
	String toString(unsigned long      num);
	String toString(long long          num);
	String toString(unsigned long long num);
}

#endif // FRONTIER_STRING_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/String.inl>
#endif
