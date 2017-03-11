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
#ifndef FRONTIER_GUITEXT_HPP_INCLUDED
#define FRONTIER_GUITEXT_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Graphics/Font.hpp>

#define FRONTIER_GUITEXT

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Values used to encode wrapping mechanisms
	/// 
	/////////////////////////////////////////////////////////////
	enum TextWrap {
		TextWrapWord, ///< Wrap only at word endings
		TextWrapChar, ///< Wrap anywhere
		TextWrapOff   ///< Don't wrap at all
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Values used to encode text align
	/// 
	/////////////////////////////////////////////////////////////
	enum TextAlign {
		TextAlignLeft,   ///< Align the text left
		TextAlignCenter, ///< Align the text centered
		TextAlignRight   ///< Align the text right
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Gui class that stores data of a font
	/// 
	/////////////////////////////////////////////////////////////
	class GuiFont
	{
	public:
		fm::String fontName; ///< The name of the font
		fg::Font font; ///< Direct pointer to the font
		
		/////////////////////////////////////////////////////////////
		/// @brief constructor
		/// 
		/////////////////////////////////////////////////////////////
		GuiFont();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the gui font from a font
		/// 
		/// @param font The font to use
		/// 
		/////////////////////////////////////////////////////////////
		GuiFont(fg::Font font);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the gui font from a font name
		/// 
		/// @param fontName The name of the font to use
		/// 
		/////////////////////////////////////////////////////////////
		GuiFont(fm::String fontName);
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the font from the gui font
		/// 
		/// @param owner The gui window that owns the font
		/// 
		/// @return The font
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font getFont(fm::Ref<GuiContext> owner);
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Gui element used to display a text
	/// 
	/////////////////////////////////////////////////////////////
	class GuiText : public GuiElement
	{
		const fg::Texture *m_tex; ///< The texture the font uses
		fg::DrawData m_drawdata;  ///< Data used when drawing
		fm::vec2 m_viewOffset; ///< The offset of the view rectangle
		TextWrap m_wrapMode;   ///< The way the text gets wrapped
		fm::Size m_charSize;   ///< The caharacter size used when drawing
		fm::String m_string;   ///< The string being displayed
		TextAlign m_align;     ///< The align of the text
		fm::vec4 m_clr; ///< The color of the text
		GuiFont m_font; ///< The fint used at displaying
		
		void updateDrawData(); ///< Internal function
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param owner The owner of the guielement
		/// 
		/////////////////////////////////////////////////////////////
		GuiText(GuiContext &owner);

		/////////////////////////////////////////////////////////////
		/// @brief construct the text with given size and owner
		/// 
		/// @param size The size of the visible area
		/// @param owner The owner of the guielement
		/// 
		/////////////////////////////////////////////////////////////
		GuiText(fm::vec2 size,GuiContext &owner);

		/////////////////////////////////////////////////////////////
		/// @brief construct the text with given text and owner
		/// 
		/// @param text The text of the guitext
		/// @param owner The owner of the guielement
		/// 
		/////////////////////////////////////////////////////////////
		GuiText(fm::String text,GuiContext &owner);

		/////////////////////////////////////////////////////////////
		/// @brief construct the text with given text, size and owner
		/// 
		/// @param text The text of the guitext
		/// @param size The size of the visible area
		/// @param owner The owner of the guielement
		/// 
		/////////////////////////////////////////////////////////////
		GuiText(fm::String text,fm::vec2 size,GuiContext &owner);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the gui text
		/// 
		/// @param size The new size
		/// 
		/////////////////////////////////////////////////////////////
		void setSize(fm::vec2s size) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the text of the gui text
		/// 
		/// @param str The new text
		/// 
		/////////////////////////////////////////////////////////////
		void setText(const fm::String &str);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the text of the gui text
		/// 
		/// @return The text
		/// 
		/////////////////////////////////////////////////////////////
		const fm::String &getText() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the font of the gui text
		/// 
		/// @param font The new font
		/// 
		/////////////////////////////////////////////////////////////
		void setFont(GuiFont font);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the font of the gui text
		/// 
		/// @return The font
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font getFont();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the name of the font the gui text uses
		/// 
		/// @return The name of the font
		/// 
		/////////////////////////////////////////////////////////////
		fm::String getFontName() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the character size of the gui text
		/// 
		/// @param charSize The new character size
		/// 
		/////////////////////////////////////////////////////////////
		void setCharacterSize(fm::Size charSize);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the character size of the gui text
		/// 
		/// @return The character size
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getCharacterSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the wrap mode of the gui text
		/// 
		/// @param charSize The new wrap mode
		/// 
		/////////////////////////////////////////////////////////////
		void setWrap(TextWrap wrap);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the wrap mode of the gui text
		/// 
		/// @return The wrap mode
		/// 
		/////////////////////////////////////////////////////////////
		TextWrap getWrapMode() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the align of the gui text
		/// 
		/// @param charSize The new align
		/// 
		/////////////////////////////////////////////////////////////
		void setAlign(TextAlign mode);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the align of the gui text
		/// 
		/// @return The align
		/// 
		/////////////////////////////////////////////////////////////
		TextAlign getAlign() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the color of the gui text
		/// 
		/// @param charSize The new color
		/// 
		/////////////////////////////////////////////////////////////
		void setColor(fm::vec4 clr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the color of the gui text
		/// 
		/// @return The color
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec4 getColor() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the view offset of the gui text
		/// 
		/// @param charSize The new view offset
		/// 
		/////////////////////////////////////////////////////////////
		void setViewOffset(fm::vec2 viewOffset);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the view offset of the gui text
		/// 
		/// @return The view offset
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getViewOffset() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw the gui text
		/// 
		/// @param shader The shader to use
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
	};
}

#endif // FRONTIER_GUITEXT_HPP_INCLUDED
