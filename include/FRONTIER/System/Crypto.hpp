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
#ifndef FRONTIER_CRYPTO_HPP_INCLUDED
#define FRONTIER_CRYPTO_HPP_INCLUDED

#include <FRONTIER/System/String.hpp>

#define FRONTIER_CRYPTO

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Create an SHA-256 hash of a given byte array
	///
	/// The result is a hexadecimal number represented with 64 characters of the set [a-z0-9]
	///
	/// @param data Pointer to the data to hash
	/// @param bytes The number of bytes in the input
	///
	/// @return The hash value computed
	///
	/////////////////////////////////////////////////////////////
	std::string sha256(const void *data,fm::Size bytes);
	
	/////////////////////////////////////////////////////////////
	/// @brief Create an SHA-256 hash of a given range
	///
	/// Copies the data into a temporary buffer
	///
	/// @param beg The beginning of the range
	/// @param end The ending of the range
	///
	/// @return The hash value computed
	///
	/////////////////////////////////////////////////////////////
	template<class InputIt>
	std::string sha256(InputIt beg,InputIt end);
}

#endif // FRONTIER_CRYPTO_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Crypto.inl>
#endif