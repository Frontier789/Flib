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
#ifndef FRONTIER_GUISCROLLBAR_HPP_INCLUDED
#define FRONTIER_GUISCROLLBAR_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Gui/CallbackUser.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/ScrollListener.hpp>

#define FRONTIER_GUISCROLLBAR

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Base class for gui classes that can be scrolled
	///
	/////////////////////////////////////////////////////////////
	class GuiScrollBar : public GuiElement, public ScrollListener, public CallbackUser<GuiScrollBar,float>
	{
		fm::Size m_stateCount; ///< The number of states the scrller can be in (0 for inf)
		float m_state; ///< The current state in range [0,1]
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param cont The owner context
		/// 
		/////////////////////////////////////////////////////////////
		GuiScrollBar(GuiContext &cont);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the element is scrolled
		/// 
		/// @param amount The amount the element is scrolled
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onScroll(float amount);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the number of states the scroller can be in
		/// 
		/// @param stateCount The number of states (0 for inf)
		/// 
		/////////////////////////////////////////////////////////////
		void setStateCount(fm::Size stateCount);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current state
		/// 
		/// @param state The new current state (will be clamped to [0,1])
		/// 
		/////////////////////////////////////////////////////////////
		void setState(float state);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of states the scroller can be in
		/// 
		/// @return The number of states (0 for inf)
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getStateCount();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current state
		/// 
		/// @return The current current state (will be clamped to [0,1])
		/// 
		/////////////////////////////////////////////////////////////
		float getState();
	};
}

#endif // FRONTIER_GUISCROLLBAR_HPP_INCLUDED
