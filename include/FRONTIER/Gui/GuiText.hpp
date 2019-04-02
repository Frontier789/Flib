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
#ifndef FRONTIER_GUITEXT_HPP_INCLUDED
#define FRONTIER_GUITEXT_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Sprite.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/System/util/API.h>
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
	/// @ingroup Gui
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiFont
	{
	public:
		fm::String fontName; ///< The name of the font
		mutable fg::Font font; ///< Direct pointer to the font
		
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
		/// Fontname can refer to a file or an os built-in font such as arial
		/// 
		/// @param fontName The name of the font to use
		/// 
		/////////////////////////////////////////////////////////////
		GuiFont(fm::String fontName);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the gui font from a font name
		/// 
		/// Fontname can refer to a file or an os built-in font such as arial
		/// 
		/// @param fontName The name of the font to use
		/// 
		/////////////////////////////////////////////////////////////
		GuiFont(const char *fontName);
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the font from the gui font
		/// 
		/// @param owner The gui window that owns the font
		/// 
		/// @return The font
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font getFont(fm::Ref<const GuiContext> owner) const;
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Gui element used to display a text
	///
	/// @ingroup Gui
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiText : public GuiElement
	{
		std::vector<fg::FontSprite> m_sprites; ///< The sprites that build up the text
		fg::Glyph::Style m_style;  ///< The style of the text
		fm::vec2 m_viewOffset; ///< The offset of the view rectangle
		fm::vec2 m_viewSize;   ///< The size of the view rectangle 
		TextWrap m_wrapMode;   ///< The way the text gets wrapped
		fm::Size m_charSize;   ///< The caharacter size used when drawing
		fm::String m_string;   ///< The string being displayed
		TextAlign m_align;     ///< The align of the text
		fm::vec4 m_clr; ///< The color of the text
		GuiFont m_font; ///< The font used at displaying
		
		void updateSprites(); ///< Internal function
		
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
		/// @param owner The owner of the guielement
		/// @param size The size of the visible area
		/// 
		/////////////////////////////////////////////////////////////
		GuiText(GuiContext &owner,fm::vec2 size);

		/////////////////////////////////////////////////////////////
		/// @brief construct the text with given text and owner
		/// 
		/// @param text The text of the guitext
		/// @param owner The owner of the guielement
		/// 
		/////////////////////////////////////////////////////////////
		GuiText(GuiContext &owner,fm::String text);

		/////////////////////////////////////////////////////////////
		/// @brief construct the text with given text, size and owner
		/// 
		/// @param owner The owner of the guielement
		/// @param size The size of the visible area
		/// @param text The text of the guitext
		/// 
		/////////////////////////////////////////////////////////////
		GuiText(GuiContext &owner,fm::vec2 size,fm::String text);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the gui text
		/// 
		/// @param size The new size
		/// 
		/////////////////////////////////////////////////////////////
		void setSize(fm::vec2s size) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the gui text
		/// 
		/// @param pos The new position
		/// 
		/////////////////////////////////////////////////////////////
		void setPosition(fm::vec2i pos) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the text of the gui text
		/// 
		/// @param str The new text
		/// 
		/////////////////////////////////////////////////////////////
		void setText(const fm::String &str);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the style of the gui text
		/// 
		/// @param style The new style
		/// 
		/////////////////////////////////////////////////////////////
		void setStyle(const fg::Glyph::Style &style);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the style of the gui text
		/// 
		/// @return The style
		/// 
		/////////////////////////////////////////////////////////////
		fg::Glyph::Style getStyle() const;
		
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
		fg::Font getFont() const;
		
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
		/// @param wrap The new wrap mode
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
		/// @param mode The new align
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
		/// @param clr The new color
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
		/// @param viewOffset The new view offset
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
		/// @brief Access one of the sprites of the gui text
		/// 
		/// @return Reference to the sprite
		/// 
		/////////////////////////////////////////////////////////////
		fg::FontSprite &getCharSprite(fm::Size index);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of sprites composing the gui text
		/// 
		/// @return The  number of sprites
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getCharSpriteCount() const; 
		
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
