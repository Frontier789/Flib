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
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Metrics.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/Image.hpp>
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
	/// This class heavily depends on FreeType2 (check out http://www.freetype.org/)
	/// 
	/// @ingroup Graphics
	/// 
	//////////////////////////////////
	class FRONTIER_API FontRenderer : fm::NonCopyable
	{
	private:
		mutable void *m_ftLibrary;  ///< The FreeType library data
		mutable void *m_ftFaceData; ///< The FreeType data of the font loaded
		bool m_loaded; ///< Indicates whether the object holds a valid font
		mutable fm::Size m_currentSize; ///< The current character size
		mutable fg::Metrics m_metrics;  ///< The metrics of the font

	public:
		typedef FontRenderer &reference;
		typedef const FontRenderer &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// After this the object is invalid (uninitialized)
		///
		/////////////////////////////////////////////////////////////
		FontRenderer();

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~FontRenderer();

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
		fm::Result loadFromFile(const std::string &fileName,unsigned int size=64);

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
		fg::Image renderGlyph(const fm::Uint32 &letter,unsigned int style = Glyph::Regular,fm::vector2<float> *leftDown = fm::nullPtr) const;
		
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
		/////////////////////////////////////////////////////////////
		void setCharacterSize(unsigned int size) const;

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