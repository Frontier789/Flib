////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
#ifndef FRONTIER_CODEPOINT_HPP_INCLUDED
#define FRONTIER_CODEPOINT_HPP_INCLUDED
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_CODEPOINT
#include <locale>

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to convert characters to codepoints
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API CodePoint
	{
	public:
		fm::Uint32 cp; ///< The value of the codepoint

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		CodePoint();

		/////////////////////////////////////////////////////////////
		/// @brief Construct codepoint from character
		///
		/// @param c single-byte character
		/// @param locale The locale to use 
		/// 
		/////////////////////////////////////////////////////////////
		CodePoint(char c,const std::locale &locale = std::locale(""));

		/////////////////////////////////////////////////////////////
		/// @brief Construct codepoint from wide character
		///
		/// @param c multi-byte character
		///
		/////////////////////////////////////////////////////////////
		CodePoint(wchar_t c);

		/////////////////////////////////////////////////////////////
		/// @brief Convert codepoint to integer value
		///
		/// @return The integer value
		///
		/////////////////////////////////////////////////////////////
		operator fm::Uint32() const;
	};
}

#endif // FRONTIER_CODEPOINT_HPP_INCLUDED
