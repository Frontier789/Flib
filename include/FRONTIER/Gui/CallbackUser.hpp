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
#ifndef FRONTIER_CALLBACKUSER_HPP_INCLUDED
#define FRONTIER_CALLBACKUSER_HPP_INCLUDED

#include <FRONTIER/System/Delegate.hpp>

#define FRONTIER_CALLBACKUSER

namespace fw
{
	class Event;
}

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Base class for classes that feature a callback
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class... ArgTypes>
	class CallbackUser
	{
		fm::Delegate<void,ObjectType &,ArgTypes...> m_callback; ///< The callback
	public:
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the callback
		/// 
		/// @param callback The callback
		///
		/////////////////////////////////////////////////////////////
		void setCallback(fm::Delegate<void,ObjectType &,ArgTypes...> callback);
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the callback
		/// 
		/// @param arguments The arguments
		///
		/////////////////////////////////////////////////////////////
		void callCallback(ArgTypes... arguments);
	};
}

#endif // FRONTIER_CALLBACKUSER_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Gui/CallbackUser.inl>
#endif
