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
#ifndef FRONTIER_PUSHBUTTON_HPP_INCLUDED
#define FRONTIER_PUSHBUTTON_HPP_INCLUDED

#include <FRONTIER/Graphics/Sprite.hpp>
#include <FRONTIER/Gui/GuiButton.hpp>
#include <FRONTIER/System/util/API.h>
#include <array>

#define FRONTIER_PUSHBUTTON

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Pushable button with a callback and sprite as background
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API PushButton : public GuiButton
	{
		fg::Sprite m_bckgSprite; ///< The background sprite
		
		std::array<fm::vec4,3> m_bckgClrs; ///< The background color
		std::array<std::string,3> m_bckImgs; /// IDs of background images
		
	protected:
		/////////////////////////////////////////////////////////////
		/// @brief Apply a state to the button
		/// 
		/////////////////////////////////////////////////////////////
		virtual void applyState() override;
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param cont The owner context
		/// 
		/////////////////////////////////////////////////////////////
		PushButton(GuiContext &cont);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the button with given text
		/// 
		/// @param cont The owner context
		/// @param text The text
		/// @param callback The callback to use
		/// 
		/////////////////////////////////////////////////////////////
		PushButton(GuiContext &cont,const fm::String &text,fm::Delegate<void,GuiButton &> callback = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief draw the button
		///
		/// @param shader The shader to use
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the button
		/// 
		/// @param size The size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the button
		/// 
		/// @param pos The position
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPosition(fm::vec2i pos) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the color of the background of the button
		/// 
		/// Sets the color of the background sprite
		/// To change the text settings, use getGuiText()
		/// 
		/// @param color The new background color
		/// 
		/////////////////////////////////////////////////////////////
		void setBgColor(fm::vec4 color);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the color of the background of the button for each state
		/// 
		/// Sets the colors of the background sprites
		/// To change the text settings, use getGuiText()
		/// 
		/// @param normal The color to use when the mouse is not over the button
		/// @param hover  The color to use when the mouse is over the button
		/// @param press  The color to use when the mouse is pressing the button
		/// 
		/////////////////////////////////////////////////////////////
		void setBgColor(fm::vec4 normal,fm::vec4 hover,fm::vec4 press);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the background image of the button
		/// 
		/// All three states of the button will have the same image after the call
		/// 
		/// @param id The id of the image, stored in the owner context
		/// 
		/////////////////////////////////////////////////////////////
		void setBgImage(const std::string &id);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the background image of the button
		/// 
		/// Sets the background image for both three states 
		/// Images are loaded from the owner context
		/// 
		/// @param normal The image to diplay when the mouse is not over the button
		/// @param hover  The image to diplay when the mouse is over the button
		/// @param press  The image to diplay when the mouse is pressing the button
		/// 
		/////////////////////////////////////////////////////////////
		void setBgImage(const std::string &normal,const std::string &hover,const std::string &press);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the background image of the button with given framesize
		/// 
		/// All three states of the button will have the same image after the call
		/// 
		/// @param id The id of the image, stored in the owner context
		/// @param frameSize The frame size to apply to the loaded image
		/// 
		/////////////////////////////////////////////////////////////
		void setBgImage(const std::string &id,fm::vec2 framSize);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the background image of the button with given framesize
		/// 
		/// Sets the background image for both three states 
		/// Images are loaded from the owner context
		/// 
		/// @param normal The image to diplay when the mouse is not over the button
		/// @param hover  The image to diplay when the mouse is over the button
		/// @param press  The image to diplay when the mouse is pressing the button
		/// @param frameSize The frame size to apply to the loaded image
		/// 
		/////////////////////////////////////////////////////////////
		void setBgImage(const std::string &normal,const std::string &hover,const std::string &press,fm::vec2 framSize);
	};
}

#endif // FRONTIER_PUSHBUTTON_HPP_INCLUDED
