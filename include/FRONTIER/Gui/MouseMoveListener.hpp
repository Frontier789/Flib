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
#ifndef FRONTIER_MOUSEMOVELISTENER_HPP_INCLUDED
#define FRONTIER_MOUSEMOVELISTENER_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Gui/GuiCallback.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_MOUSEMOVELISTENER

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Listener for mouse enter, leave, move
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API MouseMoveListener : public virtual GuiCallback
	{
	protected:
		fm::vec2 m_prevPos; ///< The last position of the mouse inside the gui element
		bool m_mouseIn; ///< Indicates the mouse being inside the gui element
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		MouseMoveListener();
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse enters the gui element
		/// 
		/// @param p The position of the mouse after entering
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseEnter(fm::vec2 p);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse leaves the gui element
		/// 
		/// @param p The position of the mouse after leaving
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseLeave(fm::vec2 p);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse moves inside the gui element
		/// 
		/// @param p The position of the mouse after moving
		/// @param prevP The position of the mouse before moving
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseMoved(fm::vec2 p,fm::vec2 prevP);
		
		/////////////////////////////////////////////////////////////
		/// @brief Checks whether the mouse is inside the gui element
		/// 
		/// @return True iff the mouse is inside
		/// 
		/////////////////////////////////////////////////////////////
		bool mouseInside() const;
	};
}

#endif // FRONTIER_MOUSEMOVELISTENER_HPP_INCLUDED
