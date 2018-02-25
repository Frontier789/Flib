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
#ifndef FRONTIER_GUICALLBACK_HPP_INCLUDED
#define FRONTIER_GUICALLBACK_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>
#include <vector>

#define FRONTIER_GUICALLBACK

namespace fw
{
	class Event;
}

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Base class for classes that act as a callback
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiCallback
	{
		std::vector<fm::Delegate<bool,fw::Event&>> m_handlers; ///< Internal vector of handlers
	public:
		
		/////////////////////////////////////////////////////////////
		/// @brief Register a handler 
		/// 
		/// @param handler The handler to add
		///
		/////////////////////////////////////////////////////////////
		void addEventHandler(fm::Delegate<bool,fw::Event&> handler);
		
		/////////////////////////////////////////////////////////////
		/// @brief Forward event to all handlers
		/// 
		/// @param ev The event to forward
		/// 
		/// @return True iff the event is handled
		/// 
		/////////////////////////////////////////////////////////////
		bool forwardToHandlers(fw::Event &ev);
	
		/////////////////////////////////////////////////////////////
		/// @brief Test if a point is inside the Gui element
		/// 
		/// @param p The point
		/// 
		/// @return True if the gui element contains the point
		///
		/////////////////////////////////////////////////////////////
		virtual bool contains(fm::vec2 p) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set this element as the active element in the owning layout
		/// 
		/// @param active True to activate false to clear being active
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setActive(bool active = true);
	};
}

#endif // FRONTIER_GUICALLBACK_HPP_INCLUDED
