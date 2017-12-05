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
#ifndef FRONTIER_SCROLLBAR_HPP_INCLUDED
#define FRONTIER_SCROLLBAR_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/Gui/GuiScrollBar.hpp>
#include <FRONTIER/Graphics/Sprite.hpp>
#include <FRONTIER/Gui/PushButton.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_SCROLLBAR

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Enum for the scroll direction
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	enum ScrollDirection {
		ScrollHorizontal = 0, ///< Horizontal
		ScrollVertical   = 1, ///< Vertical
		ScrollLeftRight  = 0, ///< Horizontal
		ScrollUpDown     = 1, ///< Vertical
		ScrollDefault    = 2  ///< Depends on size
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Skinned scrollbar gui element
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API ScrollBar : public GuiScrollBar, public ClickListener
	{
	public:
		ScrollDirection m_direction; ///< The direction
		fg::DrawData m_railDraw;     ///< DrawData for the rail
		PushButton m_handle;   		 ///< PushButton for handle
		float m_grabState; ///< The state the scrollbar was in when it was grabbed
		fm::vec2 m_grabp;  ///< The point where the scroller was grabbed
		bool m_grabbed;    ///< True iff the scrolles is grabbed
		
		/////////////////////////////////////////////////////////////
		/// @brief Setup the draws for the current size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setupDraws();
		
		/////////////////////////////////////////////////////////////
		/// @brief Test if a point is inside the drag area
		/// 
		/// @param p The point
		/// 
		/// @return True iff p is in the drag area
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool inDragArea(const fm::vec2 &p) const;
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param cont The owning context
		/// 
		/////////////////////////////////////////////////////////////
		ScrollBar(GuiContext &cont);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param cont The owning context
		/// @param size The size of the scrollbar
		/// @param callback The callback to use
		/// 
		/////////////////////////////////////////////////////////////
		ScrollBar(GuiContext &cont,fm::vec2s size,fm::Delegate<void,GuiScrollBar &> callback = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of one scroll
		/// 
		/// @param scrollSize The new scroll size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setScrollSize(float scrollSize) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current state
		/// 
		/// @param state The new current state (will be clamped to [0,1])
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setScrollState(float state) override;

		/////////////////////////////////////////////////////////////
		/// @brief Internally handle an event
		/// 
		/// @param ev The event
		/// 
		/// @return True iff the event got handled
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool onEvent(fw::Event &ev) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the element is scrolled
		/// 
		/// @param amount The amount the element is scrolled
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onScroll(float amount) override;
	
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the gui element
		/// 
		/// @param size The size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size) override;
	
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the gui element
		/// 
		/// @param pos The position
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPosition(fm::vec2i pos) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse moves inside the gui element
		/// 
		/// @param p The position of the mouse after moving
		/// @param prevP The position of the mouse before moving
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseMoved(fm::vec2 p,fm::vec2 prevP) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when a mouse button is pressed while inside the gui element
		/// 
		/// @param button The button that was pressed
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onPress(fw::Mouse::Button button,fm::vec2 p) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when a mouse button is released while inside the gui element
		/// 
		/// @param button The button that was released
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onRelease(fw::Mouse::Button button,fm::vec2 p) override;
			
		/////////////////////////////////////////////////////////////
		/// @brief draw the gui element
		///
		/// @param shader The shader to use
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
			
		/////////////////////////////////////////////////////////////
		/// @brief Set the current scroll direction
		///
		/// @param direction The new scroll direction
		///
		/////////////////////////////////////////////////////////////
		void setScrollDirection(ScrollDirection direction);
			
		/////////////////////////////////////////////////////////////
		/// @brief Get the current scroll direction
		///
		/// @return current scroll direction
		///
		/////////////////////////////////////////////////////////////
		ScrollDirection getScrollDirection() const;
	};
}

#endif // FRONTIER_SCROLLBAR_HPP_INCLUDED
