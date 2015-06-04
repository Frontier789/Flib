////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_BASICTEXT_HPP_INCLUDED
#define FRONTIER_BASICTEXT_HPP_INCLUDED
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Gui/Widget.hpp>
#define FRONTIER_BASICTEXT

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief The base class of noninteractive displayed text
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API BasicText : public Widget
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "BasicText"
		
	protected:
		const fg::Texture *m_tex; ///< The texture in which  the font stores the glyphs
		unsigned int m_charSize;  ///< The current character size
		fm::mat4 m_transform; ///< The current transform matrix
		std::string m_text;   ///< The current string
		fg::Font *m_font; ///< The current font
		fm::vec4 m_clr;   ///< The current color
		
		fm::vec2 *m_pts;  ///< The positions of vertices (contains m_quadCount*4 elements)
		fm::vec2 *m_tpts; ///< The texture coordinates of vertices (contains m_quadCount*4 elements)
		fm::vec4 *m_clrs; ///< The colors of vertices (contains m_quadCount*4 elements)
		fm::Uint16 *m_inds;   ///< The indices of vertices (contains m_quadCount*6 elements)
		fm::Size m_quadCount; ///< The number of quads to be drawn
		
		bool m_monoSpace; ///< The current state of monospacing
		float m_maxWidth; ///< The current maximum width
		bool  m_wordWrap; ///< The current state of word wrap
		
		virtual void buildVertices(); ///< Internal function that calculates the vertices to be drawn
		
		virtual void setupPosition(); ///< Internal function that updates the position
		
	public:
		typedef BasicText &reference;
		typedef const BasicText &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// If monospacing is not enabled characters will be put
		/// Right after each-other, else the offset
		/// between character middle-lines are constant
		///
		/// @param name The identifier of the instance
		/// @param anchor The anchor of the widget
		/// @param size The size of the widget
		/// @param parent The parent of the widget
		/// @param monoSpace True to enable monospacing
		/// @param maxWidth The maximum width a line can reach in pixels
		/// @param wordWrap If true words won't be cut in half
		///
		/////////////////////////////////////////////////////////////
		BasicText(const std::string &name = "NoName",
				   const Anchor &anchor = Anchor(),
				   const fm::vec2 &size = fm::vec2(),
				   Widget *parent = fm::nullPtr,
				   const std::string &text = std::string(),
				   fg::Font *font = fm::nullPtr,
				   const fm::vec4 &clr = fm::vec4::White,
				   unsigned int charSize = 42,
				   bool monoSpace = false,
				   float maxWidth = 0,
				   bool wordWrap  = true);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~BasicText();

		/////////////////////////////////////////////////////////////
		/// @brief Set the anchor of the widget
		///
		/// @param anchor The new anchor
		///
		/////////////////////////////////////////////////////////////
		virtual void setAnchor(const Anchor &anchor);

		/////////////////////////////////////////////////////////////
		/// @brief Set the parent of the widget
		///
		/// @param parent The new parent
		///
		/////////////////////////////////////////////////////////////
		virtual void setParent(Widget *parent);


		/////////////////////////////////////////////////////////////
		/// @brief Set the string of the static text
		///
		/// @param text The new string
		///
		/////////////////////////////////////////////////////////////
		virtual void setText(const std::string &text);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current string of the static text
		///
		/// @return The current string
		///
		/////////////////////////////////////////////////////////////
		virtual const std::string &getText() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the font of the static text
		///
		/// @param font The new font
		///
		/////////////////////////////////////////////////////////////
		virtual void setFont(fg::Font *font);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current font of the static text
		///
		/// @return The current font
		///
		/////////////////////////////////////////////////////////////
		virtual fg::Font *getFont() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the color of the static text
		///
		/// @param clr The new color
		///
		/////////////////////////////////////////////////////////////
		virtual void setColor(const fm::vec4 &clr);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current color of the static text
		///
		/// @return The current color
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec4 getColor() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the character size of the static text
		///
		/// @param charSize The new character size
		///
		/////////////////////////////////////////////////////////////
		virtual void setCharSize(unsigned int charSize);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current character size of the static text
		///
		/// @return The current character size
		///
		/////////////////////////////////////////////////////////////
		virtual unsigned int getCharSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Enable/disable monoSpacing
		///
		/// @param monoSpacing The new state of monoSpacing
		///
		/////////////////////////////////////////////////////////////
		virtual void setMonoSpacing(bool monoSpacing);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current state of monoSpacing
		///
		/// @return The current state of monoSpacing
		///
		/////////////////////////////////////////////////////////////
		virtual bool getMonoSpacing() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set maximum width 0 means infinite
		///
		/// @param maxWidth The new maximum width
		///
		/////////////////////////////////////////////////////////////
		virtual void setMaxWidth(float maxWidth);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current maximum width
		///
		/// @return The current maximum width
		///
		/////////////////////////////////////////////////////////////
		virtual float getMaxWidth() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the word wrap
		/// 
		/// If the word wrap is enabled no word will be 
		/// cut in half when adapting to maxWidth
		/// 
		/// @param wordWrap The new state of word wrap
		///
		/////////////////////////////////////////////////////////////
		virtual void setWordWrap(bool wordWrap);

		/////////////////////////////////////////////////////////////
		/// @brief Get state of word wrap
		///
		/// @return The state of word wrap
		///
		/////////////////////////////////////////////////////////////
		virtual bool getWordWrap() const;

		/////////////////////////////////////////////////////////////
		/// @brief Noop
		///
		/// Noop, because BasicText calculates its size internally
		///
		/// @param size Unused
		///
		/////////////////////////////////////////////////////////////
		virtual void setSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Handle being on the drawing phrase
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of parent
		///
		/////////////////////////////////////////////////////////////
		virtual void onParentChange();

		/////////////////////////////////////////////////////////////
		/// @brief Get the real type of the class
		///
		/// When creating a new class it may override this
		/// function to provide a unique name as return value
		///
		/////////////////////////////////////////////////////////////
		virtual const std::string &getRealType() const;
	};
}

#endif // FRONTIER_BasicText_HPP_INCLUDED
