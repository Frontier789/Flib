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
	template<class> class SpriteManagerBase;
	template<class> class SpriteBase;

	//////////////////////////////////
	/// @brief Reference counted font class that implements <a href="http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02#Creating_a_texture_atlas">Font atlas</a> technique
	/// 
	/// Fonts are not thread safe, to use one font in two threads you need
	/// to create a copy with createHardCopy
	/// 
	/// @ingroup Graphics
	///
	//////////////////////////////////
	class FRONTIER_API Font
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Uniquely defines a character
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API Identifier
		{
		public:
			fm::Uint32 codePoint; ///< The codepoint of the character
			Glyph::Style style;  ///< The style of the character (e.g. Underlined)

			/////////////////////////////////////////////////////////////
			Identifier();

			/////////////////////////////////////////////////////////////
			Identifier(const fm::Uint32 &codePoint,Glyph::Style style = Glyph::Regular);

			/////////////////////////////////////////////////////////////
			bool operator<(const Identifier &other) const;

			/////////////////////////////////////////////////////////////
			bool operator==(const Identifier &other) const;
		};
		
	private:
		class SharedData; ///< Stores data shared between fonts
		SharedData *m_sharedData; ///< Data shared between fonts
		
		TextureAtlas<Identifier> &getTexAtl(unsigned int charSize = 0,Glyph::Style style = Glyph::Regular) const; ///< Helper function that returns a texture atlas as needed
		
	public:
		typedef Font &reference;
		typedef const Font &const_reference;
		
		typedef SpriteBase<Identifier> Sprite;
		typedef SpriteManagerBase<Identifier> SpriteManager;

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
		/// @param characterSize The initial size
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
		/// @param style The style of the glyph
		///
		/// @return The glyph
		///
		/////////////////////////////////////////////////////////////
		Glyph getGlyph(const fm::Uint32 &codepoint,Glyph::Style style = Glyph::Regular) const;

		/////////////////////////////////////////////////////////////
		/// @brief Precache characters
		///
		/// @param characters The characters to precache
		/// @param style The style of the glyphs
		///
		/////////////////////////////////////////////////////////////
		void preCache(const fm::String &characters,Glyph::Style style = Glyph::Regular) const;

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
		Image renderGlyph(const fm::Uint32 &codepoint,Glyph::Style style = Glyph::Regular,fm::vector2<float> *leftDown = nullptr) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a glyph's bounding rectangle without rendering
		///
		/// @param letter The codepoint of the glyph
		/// @param style The style of the glyph (defaults to Regular)
		///
		/// @return The bounding rectangle
		///
		/////////////////////////////////////////////////////////////
		fm::rect2i getGlyphRect(const fm::Uint32 &letter,Glyph::Style style = Glyph::Regular) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if a given glyph is present in the font
		///
		/// @param codepoint The codepoint of the glyph
		/// @param style The style of the glyph
		///
		/// @return True iff the glyph is present
		///
		/////////////////////////////////////////////////////////////
		bool hasGlyph(const fm::Uint32 &codepoint,Glyph::Style style = Glyph::Regular) const;

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
		/// @return The renderer
		///
		/////////////////////////////////////////////////////////////
		FontRenderer &getRenderer();

		/////////////////////////////////////////////////////////////
		/// @brief Get the currently used font atlas
		/// 
		/// @param style The style to get the texture for (styles may share texture)
		/// 
		/// @return The font atlas
		///
		/////////////////////////////////////////////////////////////
		const Texture &getTexture(Glyph::Style style = Glyph::Regular) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the currently used sprite manager for a given style
		/// 
		/// Styles may share a single sprite manager
		/// 
		/// @param style The style to get the manager for
		///
		/// @return The sprite manager
		///
		/////////////////////////////////////////////////////////////
		const SpriteManagerBase<Identifier> &getSpriteManager(Glyph::Style style = Glyph::Regular) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the currently used sprite manager for a given style
		/// 
		/// Styles may share a single sprite manager
		/// 
		/// @param style The style to get the manager for
		///
		/// @return The sprite manager
		///
		/////////////////////////////////////////////////////////////
		SpriteManagerBase<Identifier> &getSpriteManager(Glyph::Style style = Glyph::Regular);
		
		/////////////////////////////////////////////////////////////
		/// @brief Call a function for each sprite manager
		/// 
		/// Each sprite manager is passed only once
		/// 
		/// @param func The function
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference forEachSpriteManager(fm::Delegate<void,SpriteManagerBase<Identifier> &,unsigned int,Glyph::Style> func);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a sprite for a code point
		/// 
		/// @param letter The codepoint
		/// @param style The style to use
		/// 
		/// @return The sprite
		///
		/////////////////////////////////////////////////////////////
		SpriteBase<Identifier> getSprite(fm::Uint32 letter,Glyph::Style style = Glyph::Regular);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get many sprites for a code point
		/// 
		/// @param letters The codepoints
		/// @param style The style to use
		/// 
		/// @return The sprite
		///
		/////////////////////////////////////////////////////////////
		std::vector<SpriteBase<Identifier>> getSprites(fm::String letters,Glyph::Style style);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the image of a glyph manually
		///
		/// @param img The image of the glyph
		/// @param letter The codepoint of the glyph
		/// @param style The style of the glyph
		/// @param leftdown The offset of the glyph from the baseline
		/// @param characterSize The target character size (0 means the currently set)
		///
		/// @return The glyph of the uploaded image
		///
		/////////////////////////////////////////////////////////////
		Glyph upload(const fg::Image &img,const fm::Uint32 &letter,Glyph::Style style = Glyph::Regular,const fm::vec2i &leftdown = fm::vec2i(),unsigned int characterSize = 0);

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
		
		/////////////////////////////////////////////////////////////
		/// @brief Load the signed distance field generator shader
		///
		/// This function is inteded to be called internally (public for debgging purposes)
		///
		/// @param sourceTransformer The function to call on th source
		///
		/////////////////////////////////////////////////////////////
		void loadSigDistFieldShader(fm::Delegate<void,fg::ShaderSource &> sourceTransformer = [](fg::ShaderSource &){});
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the font is loaded
		///
		/// @return True iff the font is loaded
		///
		/////////////////////////////////////////////////////////////
		bool isLoaded() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the font is loaded
		///
		/// @return True iff the font is loaded
		///
		/////////////////////////////////////////////////////////////
		operator bool() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if two fonts point to the same object 
		/// 
		/// Reports false when the same file is loaded twice
		/// To two different fg::Font-s and compared
		/// 
		/// @return True iff the two font are the same reference counted object
		///
		/////////////////////////////////////////////////////////////
		bool operator==(const Font &font) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the font is not loaded
		///
		/// @return True iff the font is not loaded
		///
		/////////////////////////////////////////////////////////////
		bool operator!() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw the sprites allocated using this font
		///
		/// @param shader The shader to use
		///
		/////////////////////////////////////////////////////////////
		void onDrawSprites(ShaderManager &shader);

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
	
	typedef Font::Sprite FontSprite;
	typedef Font::SpriteManager FontSpriteManager;
}

#endif // FRONTIER_FONT_HPP_INCLUDED
