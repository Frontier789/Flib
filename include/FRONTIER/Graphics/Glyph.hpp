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
#ifndef FRONTIER_GLYPH_HPP_INCLUDED
#define FRONTIER_GLYPH_HPP_INCLUDED
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/Vector2.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>
#define FRONTIER_GLYPH

namespace fg
{
	class Texture;

	/////////////////////////////////////////////////////////////
	/// @brief Class used to hold information about a glyph
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Glyph
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Values that indicate the style of a text
		///
		/////////////////////////////////////////////////////////////
		enum Style {
			Regular      = 0x00, ///< Indicates that the text is regular
			Underlined   = 0x01, ///< Indicates that the text is underlined
			Crossed      = 0x02, ///< Indicates that the text is striked through
			Outline      = 0x04, ///< Indicates that the text is an outline
			Bold         = 0x08, ///< Indicates that the text is emboldened
			Italic       = 0x10, ///< Indicates that the text is itlicized
			Superscript  = 0x20, ///< Indicates that the text is saller and is stituated above th baseline
			Subscript    = 0x40, ///< Indicates that the text is saller and is stituated below the baseline
			SigDistField = 0x80  ///< Indicates that the text is rendered using signed distance fields
		};

	public:
		typedef Glyph &reference;
		typedef const Glyph &const_reference;

		const Texture *tex; ///< The texture in which the glyph is defined
		fm::vec2u pos;      ///< The position in the texture in pixels
		fm::vec2u size;     ///< The size in the texture in pixels
		fm::vec2i leftdown; ///< The offset from the leftdown corner

		//////////////////////////////////
		/// @brief Default constructor
		///
		//////////////////////////////////
		Glyph();

		//////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param copy The Glyph to be copied
		/// 
		//////////////////////////////////
		Glyph(const_reference copy);

		//////////////////////////////////
		/// @brief Construct the Glyph from its values
		/// 
		/// @param tex The texture in which the glyph is defined
		/// @param x The x coordinate of the position in the texture in pixels
		/// @param y The y coordinate of the position in the texture in pixels
		/// @param w The width of the subTexture in pixels
		/// @param h The height of the subTexture in pixels
		/// @param xMin The x offset from the leftdown corner
		/// @param yMin The y offset from the leftdown corner
		/// 
		//////////////////////////////////
		Glyph(const Texture *tex,unsigned int x,unsigned int y,unsigned int w,unsigned int h,int xMin,int yMin);

		//////////////////////////////////
		/// @brief Construct the Glyph from its values
		/// 
		/// @param tex The texture in which the glyph is defined
		/// @param pos The position in the texture in pixels
		/// @param size The size in the texture in pixels
		/// @param leftdown The offset from the leftdown corner
		/// 
		//////////////////////////////////
		Glyph(const Texture *tex,const fm::vec2u &pos,const fm::vec2u &size,const fm::vec2i &leftdown);
		
		//////////////////////////////////
		/// @brief Test whether two glyph are the same
		/// 
		/// @param glyph The Glyph to compare to
		/// 
		/// @return True iff the two glyphs are the same
		/// 
		//////////////////////////////////
		bool operator==(const Glyph &glyph) const;
		
		//////////////////////////////////
		/// @brief Test whether two glyph are different
		///
		/// @param glyph The Glyph to compare to
		/// 
		/// @return True iff the two glyphs are different
		/// 
		//////////////////////////////////
		bool operator!=(const Glyph &glyph) const;
	};
}
#endif
