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
#ifndef FRONTIER_FONTRENDERER_HPP_INCLUDED
#define FRONTIER_FONTRENDERER_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/Graphics/Metrics.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#define FRONTIER_FONTRENDERER
#include <string>

namespace fm
{
	class String;
}

namespace fg
{
	//////////////////////////////////
	/// @brief Used for loading font files and rendering glyphs onto fg::Image
	/// 
	/// This class heavily depends on Stb_TrueType (check out http://www.nothings.org/)
	/// 
	/// @ingroup Graphics
	/// 
	//////////////////////////////////
	class FRONTIER_API FontRenderer
	{
	private:
		const fm::Uint8 *m_fileContent; ///< The font file loaded
		mutable fm::Size m_currentSize; ///< The current character size
		mutable fg::Metrics m_metrics;  ///< The metrics of the font
		mutable void *m_stbFontInfo;    ///< The internal stb font info 
		fm::Size m_fileSize;            ///< The number of bytes in data
		bool m_ownData;                 ///< Indicates whether the data is to be freed by the instance
		
		void clean(); ///< Internal function used to clean up data
		
	public:
		typedef FontRenderer &reference;
		typedef const FontRenderer &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		FontRenderer();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param renderer The instance to copy
		/// 
		/////////////////////////////////////////////////////////////
		FontRenderer(const FontRenderer &renderer) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param renderer The instance to move
		/// 
		/////////////////////////////////////////////////////////////
		FontRenderer(FontRenderer &&renderer);

		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment
		/// 
		/// @param renderer The instance to copy
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		FontRenderer &operator=(const FontRenderer &renderer) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment
		/// 
		/// @param renderer The instance to move
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		FontRenderer &operator=(FontRenderer &&renderer);

		/////////////////////////////////////////////////////////////
		/// @brief Swap two renderers
		/// 
		/// @param renderer The instance to swap with
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		FontRenderer &swap(FontRenderer &renderer);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~FontRenderer();

		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file
		///
		/// @param fileName The name of the ttf file
		/// @param characterSize The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromFile(const std::string &fileName,unsigned int characterSize = 10);

		/////////////////////////////////////////////////////////////
		/// @brief Load the Font from a ttf file that loaded in memory
		/// 
		/// The caller is responsible for freeing the data
		/// 
		/// @param fileContent A pointer to the beggining of the file in memory
		/// @param fileSizeInBytes The number of bytes in the loaded file
		/// @param characterSize The initial size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromMemory(const fm::Uint8 *fileContent,fm::Size fileSizeInBytes,unsigned int characterSize = 10);

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
		fm::Result copyFromMemory(const fm::Uint8 *fileContent,fm::Size fileSizeInBytes,unsigned int characterSize = 10);

		/////////////////////////////////////////////////////////////
		/// @brief Render a glyph's image with the current size
		///
		/// This function does not access the glyphTable
		/// The rendered image is pale white and the alpha channel indicates whether is
		/// a pixel inside of the character
		///
		/// @param letter The codepoint of the glyph
		/// @param style The style of the glyph (defaults to Regular)
		/// @param leftDown If not 0 it will be set to the offset of the glyph from the baseline
		///
		/// @return The rendered image
		///
		/////////////////////////////////////////////////////////////
		fg::Image renderGlyph(const fm::Uint32 &letter,unsigned int style = Glyph::Regular,fm::vec2 *leftDown = nullptr) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if a given glyph is present in the font
		///
		/// @param letter The codepoint of the glyph
		///
		/// @return True iff the glyph is present
		///
		/////////////////////////////////////////////////////////////
		bool hasGlyph(const fm::Uint32 &letter) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get information about the font metrics
		///
		/// @return The metrics of the font
		///
		/////////////////////////////////////////////////////////////
		const fg::Metrics &getMetrics() const;

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
		/// @brief Change the currently used character size
		///
		/// @param size The new size
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		const FontRenderer &setCharacterSize(unsigned int size) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the currently used character size
		///
		/// @return The size
		///
		/////////////////////////////////////////////////////////////
		unsigned int getCharacterSize() const;
	};
}

#endif // FRONTIER_FONTRENDERER_HPP_INCLUDED
