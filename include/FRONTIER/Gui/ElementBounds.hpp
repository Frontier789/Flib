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
#ifndef FRONTIER_ELEMENTBOUNDARIES_HPP_INCLUDED
#define FRONTIER_ELEMENTBOUNDARIES_HPP_INCLUDED
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/GuiWindow.hpp>
#define FRONTIER_ELEMENTBOUNDARIES
#include <future>

namespace fgui {
	/////////////////////////////////////////////////////////////
	/// @brief Gui element for visualizing gui element boundaries
	/// 
	/// Draws colored rectangles around gui elements
	/// 
	/////////////////////////////////////////////////////////////
	class ElementBounds : public GuiElement
	{
		fm::vec4 m_col; ///< The color to use for the rectangels
		fg::DrawData m_dd; ///< Internal data
		bool m_enabled; ///< Enabeld flags
		GuiElement *m_target; ///< The target hierarchy
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Construct the element
		/// 
		/// @param cont The owner context
		/// @param target The hierarchy to traverse (null to inlude the whole context)
		/// @param col The drawing color
		/// 
		/////////////////////////////////////////////////////////////
		ElementBounds(GuiContext &cont,GuiElement *target = nullptr,fm::vec4 col = fm::vec4::Red);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the drawing color
		/// 
		/// @param col The new drawing color
		/// 
		/////////////////////////////////////////////////////////////
		void setColor(fm::vec4 col);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current drawing color
		/// 
		/// @return The drawing color
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec4 getColor() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Enable or disable the drawing
		/// 
		/// @param enabled Tru to enable, false to disable
		/// 
		/////////////////////////////////////////////////////////////
		void enable(bool enabled = true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the drawing is enabled
		/// 
		/// @return True iff the drawing is enabled
		/// 
		/////////////////////////////////////////////////////////////
		bool enabled() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief draw the gui element
		///
		/// @param shader The shader to use
		///
		/////////////////////////////////////////////////////////////
		void onDraw(fg::ShaderManager &shader) override;
	};
}

#endif // FRONTIER_ELEMENTBOUNDARIES_HPP_INCLUDED
