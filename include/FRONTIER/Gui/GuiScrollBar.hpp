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

#include <FRONTIER/Gui/ScrollListener.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Gui/CallbackUser.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_GUISCROLLBAR

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Base class for gui classes that can be scrolled
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiScrollBar : public GuiElement, public ScrollListener, public CallbackUser<GuiScrollBar>
	{
		float m_scrollState; ///< The current state in range [0,1]
		float m_scrollSize;  ///< The amount to change the state when scrolled
		
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
		/// @brief Set the size of one scroll
		/// 
		/// @param scrollSize The new scroll size
		/// 
		/////////////////////////////////////////////////////////////
		void setScrollSize(float scrollSize);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current state
		/// 
		/// @param state The new current state (will be clamped to [0,1])
		/// 
		/////////////////////////////////////////////////////////////
		void setScrollState(float state);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the size of one scroll
		/// 
		/// @return The scroll size
		/// 
		/////////////////////////////////////////////////////////////
		float getScrollSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current state
		/// 
		/// @return The current current state (will be clamped to [0,1])
		/// 
		/////////////////////////////////////////////////////////////
		float getScrollState() const;
	};
}

#endif // FRONTIER_GUISCROLLBAR_HPP_INCLUDED
