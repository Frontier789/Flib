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
#ifndef FRONTIER_GUIBUTTON_HPP_INCLUDED
#define FRONTIER_GUIBUTTON_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/Gui/CallbackUser.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/System/util/API.h>
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
	/// @ingroup Gui
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiButton : public GuiElement, public ClickListener, public CallbackUser<GuiButton>
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief States of a button
		/// 
		/////////////////////////////////////////////////////////////
		enum Action {
			Press,   ///< Used when the button is pressed
			Drag,    ///< Used when the button is being pressed and the mouse gets dragged
			Release, ///< Used when the button is released
			Click    ///< Used when the button is clicked
		};
		
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
		/// @param callback The callback to use
		/// 
		/////////////////////////////////////////////////////////////
		GuiButton(GuiContext &cont,const fm::String &text,fm::Delegate<void,GuiButton &> callback = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw the button
		/// 
		/// @param shader The shader to use
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;

		/////////////////////////////////////////////////////////////
		/// @brief update the button
		/// 
		/// @param dt The elapsed time since last update
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onUpdate(const fm::Time &dt) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the button
		/// 
		/// @return The size
		/// 
		/////////////////////////////////////////////////////////////
		virtual fm::vec2s getSize() const override;
		
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
		/// @brief Set the action callback
		/// 
		/// @param callback The new callback
		/// 
		/////////////////////////////////////////////////////////////
		void setActionCallback(fm::Delegate<void,GuiButton &,Action> callback);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the displayed text on the button
		/// 
		/// @param text The new text
		/// @param adaptSize Specifies whether the size of the button should shrink to fit if needed
		/// 
		/////////////////////////////////////////////////////////////
		void setText(const fm::String &text,bool adaptSize = true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the displayed text"s character size on the button
		/// 
		/// @param characterSize The new character size
		/// 
		/////////////////////////////////////////////////////////////
		void setCharacterSize(fm::Size characterSize);
		
		/////////////////////////////////////////////////////////////
		/// @brief Call a function for every gui element in the hierarchy
		/// 
		/// @param func The func to call
		/// 
		/////////////////////////////////////////////////////////////
		virtual void traverseHierarchy(fm::Delegate<void,GuiElement &> func) override;
		
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
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse moves inside the gui element
		/// 
		/// @param p The position of the mouse after moving
		/// @param prevP The position of the mouse before moving
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseMove(fm::vec2 p,fm::vec2 prevP) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Change whether the button is enabled or disabled
		/// 
		/// Only enabled button reacts to pressing
		/// Default is enabled
		/// 
		/// @param enabled True to enable, false to disable
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setEnabled(bool enabled);
		
		/////////////////////////////////////////////////////////////
		/// @brief Check whether the button is enabled or disabled
		/// 
		/// @retun True if enabled, false if disabled
		/// 
		/////////////////////////////////////////////////////////////
		bool getEnabled() const;
		
	protected:
		/////////////////////////////////////////////////////////////
		/// @brief States of a button
		/// 
		/////////////////////////////////////////////////////////////
		enum InnerState {
			Normal  = 0, ///< The mouse is not over the button
			Hovered = 1, ///< The mouse is on the button
			Pressed = 2  ///< The button is pressed
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Apply a state to the button
		/// 
		/// To be implemented by derived classes
		/// 
		/////////////////////////////////////////////////////////////
		virtual void applyState();
		
		InnerState m_state; ///< Stores current state
		
	private:
		fm::Delegate<void,GuiButton &,Action> m_actionCb; ///< Callback for actions
		fm::vec2s m_prevTextSize; ///< Last size of the gui text
		GuiText m_text; ///< The gui elem for text display on the button
		bool m_enabled; ///< Indicates whether the button is enabled
		
		void setState(InnerState state) {m_state = state; applyState();}
	};
}

#endif // FRONTIER_GUIBUTTON_HPP_INCLUDED
