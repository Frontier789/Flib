////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_STATICTEXT_HPP_INCLUDED
#define FRONTIER_STATICTEXT_HPP_INCLUDED
#include <FRONTIER/System/Transformable.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Graphics/Color.hpp>

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_STATICTEXT
#include <string>
namespace fg
{
	class Font;

	/////////////////////////////////////////////////////////////
	/// @brief Class used to hold, claculate and draw text
	///
	/// Please note that calling fg::StaticText::set* is slow (O(n))
	/// because all of them recalculates the vertices of the object
	///
	/// If kerning is enabled 10 (non newline) characters
	/// take up 2 times the space 5 does
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API StaticText : public Drawable, public fm::Transformable, public fm::NonCopyable
	{
	private:
		fg::Color m_color;      ///< The color of the text
		std::string m_text;     ///< Holds the caracters to be rendered
		unsigned int m_align;   ///< The horizontal align of the text
		fm::vec2 m_boundings;   ///< Caches the bounds of the text
		bool m_kerningEnabled;  ///< Indicates whether kerning is enabled
		const fg::Font *m_font; ///< The font used
		const fg::Texture *m_texture; ///< The texture where the font rendered
		unsigned int m_characterSize; ///< The character height
		unsigned int m_vertexCount;   ///< Stores the amount of vertices
		unsigned int m_indexCount;    ///< Stores the number of indices
		fg::Buffer m_posBuf;    ///< Stores the positions
		fg::Buffer m_clrBuf;    ///< Stores th colors
		fg::Buffer m_texPosBuf; ///< Stores the texture coordinates
		fg::Buffer m_indBuf;    ///< Stores the indices
		void buildVertices();   ///< Internal function used to construct the vertices
	public:
		typedef StaticText &reference;
		typedef const StaticText &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Indicates where the text is clipped
		///
		/////////////////////////////////////////////////////////////
		class Align
		{
		public:
			enum {
				Left   = 0, ///< Every line begins at the left side
				Center = 1, ///< Every line ends at the right side
				Right  = 2  ///< Every line's beginning and ending is equally far from the side
			};
		};


		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		StaticText();

		/////////////////////////////////////////////////////////////
		/// @brief Constuct the text and set its attributes
		///
		/// @param font The font used when drawing
		/// @param text The string to be drawn (understands newline)
		/// @param color The color of the text itself
		/// @param characterSize The height of the biggest character in pixels
		/// @param align The type of alignment (use fg::StaticText::Ailgn::*)
		/// @param enableKerning If true then kerning is enabled
		///
		/////////////////////////////////////////////////////////////
		StaticText(const fg::Font &font,const std::string &text,const fg::Color &color = fg::Color::White,unsigned int characterSize = 64,unsigned int align = Align::Left,bool enableKerning = false);

		/////////////////////////////////////////////////////////////
		/// @brief Change the string of the text
		///
		/// @param text The new string
		///
		/////////////////////////////////////////////////////////////
		void setText(const std::string &text);

		/////////////////////////////////////////////////////////////
		/// @brief Change the font of the text
		///
		/// @param font The new font (if NULL nothing will be drawn)
		///
		/////////////////////////////////////////////////////////////
		void setFont(const fg::Font &font);

		/////////////////////////////////////////////////////////////
		/// @brief Change the color of the text
		///
		/// @param color The new color
		///
		/////////////////////////////////////////////////////////////
		void setColor(const fg::Color &color);

		/////////////////////////////////////////////////////////////
		/// @brief Change the align of the text
		///
		/// @param align The new align
		///
		/////////////////////////////////////////////////////////////
		void setAlign(unsigned int align);

		/////////////////////////////////////////////////////////////
		/// @brief Change the character size of the text
		///
		/// @param size The new height in pixels
		///
		/////////////////////////////////////////////////////////////
		void setCharacterSize(unsigned int size);

		/////////////////////////////////////////////////////////////
		/// @brief Enable (or disable the kerning)
		///
		/// @param enable True to enable false to disable
		///
		/////////////////////////////////////////////////////////////
		void enableKerning(bool enable = true);

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the rendered text (h is negative!)
		///
		/// @return The bounding size
		///
		/////////////////////////////////////////////////////////////
		const fm::vec2 &getBoundings() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the string of the text
		///
		/// @return The string
		///
		/////////////////////////////////////////////////////////////
		const std::string &getText() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the font of the text
		///
		/// @return The font (can return NULL)
		///
		/////////////////////////////////////////////////////////////
		const fg::Font *getFont() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of the text
		///
		/// @return The color
		///
		/////////////////////////////////////////////////////////////
		const fg::Color &getColor() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the align of the text
		///
		/// @return The align
		///
		/////////////////////////////////////////////////////////////
		unsigned int getAlign() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the character size of the text
		///
		/// @return The character size
		///
		/////////////////////////////////////////////////////////////
		unsigned int getCharacterSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Find out whether the kerning is enabled
		///
		/// @return True iff kerning is enabled
		///
		/////////////////////////////////////////////////////////////
		bool isKerningEnabled() const;

		/////////////////////////////////////////////////////////////
		/// @brief Draw the text
		///
		/// This call resolves to fg::draw
		///
		/// @param states The states to be used (texture is ignored)
		///
		/////////////////////////////////////////////////////////////
		virtual void draw(const fg::RenderStates &states) const;
	};
}


#endif //FRONTIER_STATICTEXT_HPP_INCLUDED
