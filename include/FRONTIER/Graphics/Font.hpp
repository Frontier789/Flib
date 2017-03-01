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
#ifndef FRONTIER_FONT_HPP_INCLUDED
#define FRONTIER_FONT_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/Graphics/TextureAtlas.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Metrics.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/util/API.h>
#define FRONTIER_FONT
#include <string>
#include <map>

namespace fm
{
	template<class>
	class vector2;

	class String;
}

namespace fg
{
	class FontRenderer;

	//////////////////////////////////
	/// @brief Reference counted font class that implements <a href="http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02#Creating_a_texture_atlas">Font atlas</a> technique
	///
	//////////////////////////////////
	class FRONTIER_API Font
	{
	private:
		/////////////////////////////////////////////////////////////
		/// @brief Uniquely defines a character
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API Identifier
		{
		public:
			fm::Uint32 codePoint; ///< The codepoint of the character
			unsigned int style;  ///< The style of the character (e.g. Underlined)

			/////////////////////////////////////////////////////////////
			Identifier();

			/////////////////////////////////////////////////////////////
			Identifier(const fm::Uint32 &codePoint,unsigned int style);

			/////////////////////////////////////////////////////////////
			bool operator<(const Identifier &other) const;
		};
	
		mutable std::map<unsigned int,TextureAtlas<Identifier> > *m_texAtlases; ///< Maps characer sizes to different font atlases
		FontRenderer *m_renderer; ///< A pointer to a fg::Font::Renderer that does the rendering part
		fm::Size *m_refCount; ///< The reference count
		
	public:
		typedef Font &reference;
		typedef const Font &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// After this the object is invalid (uninitialized)
		///
		/////////////////////////////////////////////////////////////
		Font();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// only copies reference
		///
		/// @param copy The font to copy
		///
		/////////////////////////////////////////////////////////////
		Font(const Font &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param move The font to move
		///
		/////////////////////////////////////////////////////////////
		Font(Font &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment
		/// 
		/// only copies reference
		/// 
		/// @param copy The font to copy
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Font &operator=(const Font &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment
		///
		/// @param move The font to move
		/// 
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		Font &operator=(Font &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~Font();

		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file
		///
		/// @param fileName The name of the ttf file
		/// @param characterSize The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromFile(const std::string &fileName,unsigned int characterSize = 14);

		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a system ttf file
		///
		/// @param fileName The name of the ttf file
		/// @param characterSize The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadSysFont(const std::string &fileName,unsigned int characterSize = 14);

		/////////////////////////////////////////////////////////////
		/// @brief Load the default Font from a system ttf file
		///
		/// @param characterSize The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadDefSysFont(unsigned int characterSize = 14);

		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file that loaded in memory
		///
		/// @param fileContent A pointer to the beggining of the file in memory
		/// @param fileSizeInBytes The number of bytes in the loaded file
		/// @param size The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromMemory(const fm::Uint8 *fileContent,fm::Size fileSizeInBytes,unsigned int characterSize = 14);

		/////////////////////////////////////////////////////////////
		/// @brief Copy the Font from a ttf file that loaded in memory
		///
		/// @param fileContent A pointer to the beggining of the file in memory
		/// @param fileSizeInBytes The number of bytes in the loaded file
		/// @param characterSize The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result copyFromMemory(const fm::Uint8 *fileContent,fm::Size fileSizeInBytes,unsigned int characterSize = 14);

		/////////////////////////////////////////////////////////////
		/// @brief Create a real copy of the font
		///
		/// @return The copy
		///
		/////////////////////////////////////////////////////////////
		Font createHardCopy() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the mignification and magnification filter on the texture atlases
		///
		/// @param smooth If true then linear filtering is used, nearest filtering otherwise
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setSmooth(bool smooth);

		/////////////////////////////////////////////////////////////
		/// @brief Get a glyph from its codepoint and style with the current size
		///
		/// This function searches the glyphTable and if doesn't
		/// find the glyph it creates the glyph.
		///
		/// @param codepoint The codepoint of the glyph
		/// @param type The style of the glyph
		///
		/// @return The glyph
		///
		/////////////////////////////////////////////////////////////
		Glyph getGlyph(const fm::Uint32 &codepoint,unsigned int type = Glyph::Regular) const;

		/////////////////////////////////////////////////////////////
		/// @brief Precache characters
		///
		/// @param characters The characters to precache
		/// @param type The style of the glyphs
		///
		/////////////////////////////////////////////////////////////
		void preCache(const fm::String &characters,unsigned int type = Glyph::Regular) const;

		/////////////////////////////////////////////////////////////
		/// @brief Render a glyph's image with the current size
		///
		/// This function does not access the glyphTable
		///
		/// @param codepoint The codepoint of the glyph
		/// @param style The style of the glyph
		/// @param leftDown If not NULL it will be set to the offset of the glyph from the baseline
		///
		/// @return The rendered image
		///
		/////////////////////////////////////////////////////////////
		Image renderGlyph(const fm::Uint32 &codepoint,unsigned int style = Glyph::Regular,fm::vector2<float> *leftDown = nullptr) const;

		/////////////////////////////////////////////////////////////
		/// @brief Find out if a given glyph is present in the font
		///
		/// @param codepoint The codepoint of the glyph
		/// @param style The style of the glyph
		///
		/// @return True iff the glyph is present
		///
		/////////////////////////////////////////////////////////////
		bool hasGlyph(const fm::Uint32 &codepoint,unsigned int style = Glyph::Regular) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get information about the font metrics
		///
		/// @return The metrics of the font
		///
		/////////////////////////////////////////////////////////////
		fg::Metrics getMetrics() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the kerning offset between two glyphs
		///
		/// @param leftCodePoint  The first letter
		/// @param rightCodePoint The second letter
		///
		/// @return The kerning offset
		///
		/////////////////////////////////////////////////////////////
		int getKerning(const fm::Uint32 &leftCodePoint,const fm::Uint32 &rightCodePoint) const;

		/////////////////////////////////////////////////////////////
		/// @brief Change the currently used size
		///
		/// @param characterSize The new size
		///
		/////////////////////////////////////////////////////////////
		void setCharacterSize(unsigned int characterSize) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the currently set character size
		///
		/// @return The character size
		///
		/////////////////////////////////////////////////////////////
		unsigned int getCharacterSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the underlying renderer
		///
		/// @return The renderer (can be null)
		///
		/////////////////////////////////////////////////////////////
		FontRenderer *getRenderer();

		/////////////////////////////////////////////////////////////
		/// @brief Get the currently used font atlas
		///
		/// @return The font atlas
		///
		/////////////////////////////////////////////////////////////
		const Texture &getTexture() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the image of a glyph manually
		///
		/// @param img The image of the glyph
		/// @param letter The codepoint of the glyph
		/// @param type The style of the glyph
		/// @param leftdown The offset of the glyph from the baseline
		/// @param characterSize The target character size (0 means the currently set)
		///
		/// @return The glyph of the uploaded image
		///
		/////////////////////////////////////////////////////////////
		Glyph upload(const fg::Image &img,const fm::Uint32 &letter,unsigned int type = Glyph::Regular,const fm::vec2i &leftdown = fm::vec2i(),unsigned int characterSize = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Swap the content of the two objects
		///
		/// swaps in constant time
		///
		/// @param font The object to swap with
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference swap(Font &font);

	private:
		/////////////////////////////////////////////////////////////
		/// @brief Internal function used to clean up the resources
		///
		/////////////////////////////////////////////////////////////
		void cleanUp();

		/////////////////////////////////////////////////////////////
		/// @brief Internal function used to initialize the resources
		///
		/////////////////////////////////////////////////////////////
		void init();
	};
}

#endif // FRONTIER_FONT_HPP_INCLUDED
