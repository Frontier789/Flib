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

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/Graphics/TextureAtlas.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Metrics.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/System/Result.hpp>
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
	/// @brief Font class that implements <a href="http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02#Creating_a_texture_atlas">Font atlas</a> technique
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

		
		fm::Size *m_refCount;     ///< How many instances does reference this object
		FontRenderer *m_renderer; ///< A pointer to a fg::Font::Renderer that does the rendering part
		mutable std::map<unsigned int,TextureAtlas<Identifier> > *m_texAtlases; ///< Maps characer sizes to different font atlases
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
		/// @brief Default constructor
		///
		/// If @a copy is invalid (uninitialized) then this the object will be invalid (uninitialized)
		///
		/////////////////////////////////////////////////////////////
		Font(const Font &copy);


		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~Font();

		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file
		///
		/// If the file cannot be loaded or does not contain a valid
		/// ttf font then an error is prompted to fg_log and object becomes invalid
		///
		/// @param fileName The name of the ttf file
		/// @param size The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromFile(const std::string &fileName,unsigned int size = 64);

		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file that loaded in memory
		///
		/// If @a fileContent contains an invalid
		/// ttf font then an error is prompted to fg_log and object becomes invalid
		///
		/// @param fileContent A pointer to the beggining of the file in memory
		/// @param fileSizeInBytes The number of bytes in the loaded file
		/// @param size The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromMemory(const void *fileContent,fm::Size fileSizeInBytes,unsigned int size=64);

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
		/// @param letter The codepoint of the glyph
		/// @param type The style of the glyph
		///
		/// @return The glyph
		///
		/////////////////////////////////////////////////////////////
		Glyph getGlyph(const fm::Uint32 &letter,unsigned int type = Glyph::Regular) const;

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
		/// @param letter The codepoint of the glyph
		/// @param style The style of the glyph
		/// @param leftDown If not NULL it will be set to the offset of the glyph from the baseline
		///
		/// @return The rendered image
		///
		/////////////////////////////////////////////////////////////
		Image renderGlyph(const fm::Uint32 &letter,unsigned int style = Glyph::Regular,fm::vector2<float> *leftDown = fm::nullPtr) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if a given glyph is present in the font
		///
		/// @param letter The codepoint of the glyph
		/// @param style The style of the glyph
		///
		/// @return True iff the glyph is present
		///
		/////////////////////////////////////////////////////////////
		bool hasGlyph(const fm::Uint32 &letter,unsigned int style = Glyph::Regular) const;

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
		/// @param size The new size
		///
		/////////////////////////////////////////////////////////////
		void setCharacterSize(unsigned int size) const;

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
		Glyph upload(const fg::Image &img,const fm::Uint32 &letter,unsigned int type = Glyph::Regular,const fm::vec2s &leftdown = fm::vec2s(),unsigned int characterSize = 0);
		
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

	////////////////////////////////////////////////////////////
	/// @class fg::Font
	/// @ingroup Graphics
	/// 
	/// Example:
	/// @code
	/// fg::Font::Renderer arial;
	/// if (!arial.loadFromFile("arial.ttf"))
	/// {
	///     /* handle error */
	///     return -1;
	/// }
	/// fg::Image A = arial.renderGlyph('A');
	/// A.saveToFile("A.png");
	/// @endcode
	///
	/// @see fg::Font
	/// @see fg::Image
	/// @see fg::FontRenderer
	///
	////////////////////////////////////////////////////////////
}

#endif // FRONTIER_FONT_HPP_INCLUDED
