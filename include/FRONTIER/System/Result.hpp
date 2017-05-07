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
#include <FRONTIER/System/CommonTypes.hpp>
#define FRONTIER_RESULT
#include <vector>
#include <string>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to encode the result of an operation
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	class Result
	{
	public:
		/// Encodes the level of the error
		enum ErrorLevel {
			OPDone,    ///< The operation succeeded
			OPChanged, ///< The operation was modified in order to be completed
			OPFailed   ///< The operation failed
		};
		
		std::string type;  ///< Holds the type of the error 
		ErrorLevel  level; ///< Holds the level of the error 
		std::string error; ///< Holds the description of the error 
		std::string func;  ///< Holds the function name where the error happened
		std::string file;  ///< The file where the error was raised
		fm::Size line;     ///< The line where the error was raised
		std::vector<std::string> details; ///< Holds additional error dependant values
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Result();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a Result out of its members
		///
		/// @param type Holds the type of the error 
		/// @param level Holds the level of the error 
		/// @param error Holds the description of the error 
		/// @param func Holds the function name where the error happened
		/// @param file The file where the error was raised
		/// @param line The line where the error was raised
		/// @param details Holds additional error dependant values
		///
		/////////////////////////////////////////////////////////////
		template<class... StringClass>
		Result(const std::string &type,
			   ErrorLevel level,
			   const std::string &error,
			   const std::string &func,
			   const std::string &file,
			   fm::Size line,
			   const StringClass &... details);
		
		/////////////////////////////////////////////////////////////
		/// @brief Bool conversion operator
		/// 
		/// checks level against OPFailed and type's emptiness
		/// 
		/// @return True iff the result is not an error
		///
		/////////////////////////////////////////////////////////////
		operator bool() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the result is an error
		/// 
		/// checks level against OPFailed and type's emptiness
		/// 
		/// @return True iff the result is an error
		///
		/////////////////////////////////////////////////////////////
		bool operator!() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check the error type
		/// 
		/// @param errorType The error type to check against
		/// 
		/// @return True iff the error type of the result equals to @a errorType
		///
		/////////////////////////////////////////////////////////////
		bool operator==(const std::string &errorType) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check the error level
		/// 
		/// @param errorLevel The error level to check against
		/// 
		/// @return True iff the error level of the result equals to @a errorType
		///
		/////////////////////////////////////////////////////////////
		bool operator==(ErrorLevel errorLevel) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check the error type
		/// 
		/// @param errorType The error type to check against
		/// 
		/// @return False iff the error type of the result equals to @a errorType
		///
		/////////////////////////////////////////////////////////////
		bool operator!=(const std::string &errorType)  const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check the error level
		/// 
		/// @param errorLevel The error level to check against
		/// 
		/// @return False iff the error level of the result equals to @a errorType
		///
		/////////////////////////////////////////////////////////////
		bool operator!=(ErrorLevel errorLevel) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Overwrite the result if the other one is an error
		/// 
		/// @param result The result
		/// 
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		Result &operator+=(const Result &result);
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert the result to a human-readable string
		/// 
		/// @param fullDesc True to get full description
		/// 
		/// @return The description string 
		///
		/////////////////////////////////////////////////////////////
		std::string toString(bool fullDesc = true) const;
	};
}

#endif // FRONTIER_RESULT_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Result.inl>
#endif // FRONTIER_DONT_INCLUDE_INL

