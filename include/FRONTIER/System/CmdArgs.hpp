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
#ifndef FRONTIER_CMDARGS_HPP_INCLUDED
#define FRONTIER_CMDARGS_HPP_INCLUDED

#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/String.hpp>
#include <type_traits>
#include <vector>
#include <map>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class for handling command line arguments 
	/// 
	/////////////////////////////////////////////////////////////
	class CmdArgs
	{
		std::vector<fm::String> m_args; ///< Holds the actual arguments
		std::map<fm::String,fm::Delegate<bool,fm::Result &,fm::Size &>> m_callbacks; //< The registered callbacks
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		CmdArgs();

		/////////////////////////////////////////////////////////////
		/// @brief Constructor that sets args
		/// 
		/// @param argc The number of arguments
		/// @param argv The array of arguments
		/// 
		/////////////////////////////////////////////////////////////
		CmdArgs(int argc,char *argv[]);

		/////////////////////////////////////////////////////////////
		/// @brief Register a new switch to be interpreted
		/// 
		/// The callback shall return true when it accepts the value
		/// And false if the value is out of bounds or otherwise illegal
		/// 
		/// @param switch The switch 
		/// @param callback The callback which will be called
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,class = typename std::enable_if<fm::StringDecoder<T>::value>::type>
		void addSwitch(fm::String argSwitch,fm::Delegate<bool,T> callback = [](T){return true;});

		/////////////////////////////////////////////////////////////
		/// @brief Set the arguments for the handler
		/// 
		/// Discards previous arguments
		/// 
		/// @param argc The number of arguments
		/// @param argv The array of arguments
		/// 
		/////////////////////////////////////////////////////////////
		void setArgs(int argc,char *argv[]);

		/////////////////////////////////////////////////////////////
		/// @brief Add a single new argument
		/// 
		/// Extends the array of argumnets with @a arg
		/// 
		/// @param arg The new argument
		/// 
		/////////////////////////////////////////////////////////////
		void addArg(fm::String arg);

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of arguments 
		/// 
		/// @return The number of arguments
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getArgCount() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get an argument
		/// 
		/// @param index The index of the argument
		/// 
		/// @return The argument
		/// 
		/////////////////////////////////////////////////////////////
		fm::String getArg(fm::Size index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Process the given arguments
		/// 
		/// @return The result of the processing
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result processArgs() const;
	};
	
}

#endif

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/CmdArgs.inl>
#endif
