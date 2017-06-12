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

#include <FRONTIER/Graphics/SpriteStateAnimation.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/Gui/GuiScrollBar.hpp>

#define FRONTIER_SCROLLBAR

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Enum for the scroll direction
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
	class ScrollBar : public GuiScrollBar, public ClickListener
	{
		fg::SpriteAnimation m_handleAnim; ///< Sprite anim for handle
		ScrollDirection m_direction; ///< The direction
		fg::DrawData m_railDraw;     ///< DrawData for the rail
		float m_grabState; ///< The state the scrollbar was in when it was grabbed
		fm::vec2 m_grabp;  ///< The point where the scroller was grabbed
		bool m_grabbed;    ///< True iff the scrolles is grabbed
		
		/////////////////////////////////////////////////////////////
		/// @brief Setup the rail draw for the current size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setupRail();
		
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
		/// 
		/////////////////////////////////////////////////////////////
		ScrollBar(GuiContext &cont,fm::vec2s size,fm::Delegate<void,GuiScrollBar &,float> callback = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the gui element
		/// 
		/// @param size The size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size) override;
		
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
