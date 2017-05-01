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
#ifndef FRONTIER_GUIBUTTON_HPP_INCLUDED
#define FRONTIER_GUIBUTTON_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/GuiText.hpp>

#define FRONTIER_GUIBUTTON

namespace fw
{
	class Event;
}

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Basic button class capable of displaying a text and having a callback
	/// 
	/////////////////////////////////////////////////////////////
	class GuiButton : public virtual GuiElement, public ClickListener
	{
		fm::Delegate<void,GuiButton &> m_callback; ///< The callback to call on click
		GuiText m_text; ///< The gui elem for text display on the button
		
	protected:
		/////////////////////////////////////////////////////////////
		/// @brief States of a button
		/// 
		/////////////////////////////////////////////////////////////
		enum InnerState {
			Normal, ///< The mouse is not over the button
			Hover,  ///< The mouse is on the button
			Pressed ///< The button is pressed
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Apply a state to the button
		/// 
		/// To be implemented by derived classes
		/// 
		/// @param state The state to apply
		/// 
		/////////////////////////////////////////////////////////////
		virtual void applyState(InnerState state);
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param cont The owner context
		/// 
		/////////////////////////////////////////////////////////////
		GuiButton(GuiContext &cont);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct with given text
		/// 
		/// @param cont The owner context
		/// @param text The text to use
		/// 
		/////////////////////////////////////////////////////////////
		GuiButton(GuiContext &cont,const fm::String &text);
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw the button
		/// 
		/// @param shader The shader to use
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the button
		/// 
		/// @param size The new size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the button
		/// 
		/// @param pos The new position
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPosition(fm::vec2i pos) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the text element owned by the button
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiText &getGuiText();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the text element owned by the button
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		const GuiText &getGuiText() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the displayed text on the button
		/// 
		/// @param text The new text
		/// 
		/////////////////////////////////////////////////////////////
		void setText(const fm::String &text);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the callback of the button
		/// 
		/// @param callback The callback
		/// 
		/////////////////////////////////////////////////////////////
		void setCallback(fm::Delegate<void,GuiButton &> callback);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse enters the gui element
		/// 
		/// @param p The position of the mouse after entering
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseEnter(fm::vec2 p) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse leaves the gui element
		/// 
		/// @param p The position of the mouse after leaving
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseLeave(fm::vec2 p) override;
		
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
		/// @brief Called when a mouse button is clicked while inside the gui element
		/// 
		/// @param button The button that was clicked
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onClick(fw::Mouse::Button button,fm::vec2 p) override;
	};
}

#endif // FRONTIER_GUIBUTTON_HPP_INCLUDED
