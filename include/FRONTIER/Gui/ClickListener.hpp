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
#ifndef FRONTIER_CLICKLISTENER_HPP_INCLUDED
#define FRONTIER_CLICKLISTENER_HPP_INCLUDED

#include <FRONTIER/Gui/MouseMoveListener.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/System/Time.hpp>

#define FRONTIER_CLICKLISTENER

namespace fgui
{
	class GuiWindow;
	
	/////////////////////////////////////////////////////////////
	/// @brief Listener for mouse click, doubleclick, press, release
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API ClickListener : public virtual MouseMoveListener
	{
		fm::Clock m_clickClks[fw::Mouse::ButtonCount]; ///< Clock for double click
		fm::vec2 m_pressPoses[fw::Mouse::ButtonCount]; ///< The position where the button was pressed
		bool m_btnPressed[fw::Mouse::ButtonCount];     ///< Mouse button states
		fm::Time m_dblClickLength; ///< The maximal time between two clicks to count as double click
		float m_maxClickDist;      ///< The radius of the circle in which
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		ClickListener();
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the maximal time between two clicks to count as double click
		/// 
		/// @param maxTime The new maximal time
		/// 
		/////////////////////////////////////////////////////////////
		void setDoubleClickTime(fm::Time maxTime);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the maximal time between two clicks to count as double click
		/// 
		/// @return The maximal time
		/// 
		/////////////////////////////////////////////////////////////
		fm::Time getDoubleClickTime() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the maximal time between two clicks to count as double click
		/// 
		/// @param dist The new maximal distance
		/// 
		/////////////////////////////////////////////////////////////
		void setMaxClickDistance(float dist);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the maximal time between two clicks to count as double click
		/// 
		/// @return The maximal time
		/// 
		/////////////////////////////////////////////////////////////
		float getMaxClickDistance() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when a mouse button is pressed while inside the gui element
		/// 
		/// @param button The button that was pressed
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onPress(fw::Mouse::Button button,fm::vec2 p);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when a mouse button is released while inside the gui element
		/// 
		/// @param button The button that was released
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onRelease(fw::Mouse::Button button,fm::vec2 p);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when a mouse button is clicked while inside the gui element
		/// 
		/// @param button The button that was clicked
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onClick(fw::Mouse::Button button,fm::vec2 p);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when a mouse button is double clicked while inside the gui element
		/// 
		/// @param button The button that was double clicked
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDblClick(fw::Mouse::Button button,fm::vec2 p);
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the gui element is pressed
		/// 
		/// @param button The button to check
		/// 
		/////////////////////////////////////////////////////////////
		bool isPressed(fw::Mouse::Button button) const;
	};
}

#endif // FRONTIER_CLICKLISTENER_HPP_INCLUDED
