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
#ifndef FRONTIER_RESULT_HPP_INCLUDED
#define FRONTIER_RESULT_HPP_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_RESULT
#include <string>

namespace fm
{
	class Error;
	
	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to represent an error state
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Result
	{
	public:
		typedef Result &reference;
		typedef const Result &const_reference;
		
		std::string id;     //< "" on no error, the identifier of the error othervise
		std::string detail; //< The type of the error
		std::string desc;   //< A human-readable description of the error
		std::string func;   //< The function in which th error happened
		std::string file;   //< The file in which the error was raised
		fm::Size line;      //< The line on which the function was called

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param id The identifier of the error
		/// @param detail The type of the error
		/// @param desc A human-readable description of the error
		/// @param file The file in which the error was raised
		/// @param func The function in which th error happened
		///
		/////////////////////////////////////////////////////////////
		Result(const std::string &id = "",
			   const std::string &detail = "",
			   const std::string &desc = "",
			   const std::string &func = "",
			   const std::string &file = "",
			   fm::Size line = 0);
		
		/////////////////////////////////////////////////////////////
		/// @brief Constructor from an error class
		///
		/// @param error The error to convert
		///
		/////////////////////////////////////////////////////////////
		Result(const Error &error);
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to bool
		/// 
		/// @return true iff id is "" (false otherwise)
		/// 
		/////////////////////////////////////////////////////////////
		operator bool() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief "not" operator
		/// 
		/// @return false iff id is "" (true otherwise)
		/// 
		/////////////////////////////////////////////////////////////
		bool operator!() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Assign an error
		/// 
		/// @param error The error to use
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Result &operator=(const Error &error);
		
		/////////////////////////////////////////////////////////////
		/// @brief Compose a string from the result
		/// 
		/// @return The composed string
		/// 
		/////////////////////////////////////////////////////////////
		std::string toString() const;
	};
}

namespace std
{
    template<typename,typename>
    class basic_ostream;
    template<typename,typename>
    class basic_istream;
    template<typename>
    class char_traits;
}

////////////////////////////////////////////////////////////
std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out,const fm::Result &result);

#endif // FRONTIER_RESULT_HPP_INCLUDED
