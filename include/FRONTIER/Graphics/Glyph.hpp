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
#ifndef FRONTIER_GLYPH_HPP_INCLUDED
#define FRONTIER_GLYPH_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/Vector2.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_GLYPH
namespace fg
{
	class Texture;
	//////////////////////////////////
	/// @brief Class used to hold information about a glyph
	///
	//////////////////////////////////
	class FRONTIER_API Glyph
	{
	public:
		typedef Glyph &reference;
		typedef const Glyph &const_reference;

		const Texture *tex; ///< The texture in which the glyph is defined
		fm::vec2u pos;      ///< The position in the texture in pixels
		fm::vec2u size;     ///< The size in the texture in pixels
		fm::vec2i leftdown; ///< The difference from the leftdown corner

		//////////////////////////////////
		/// @brief Default constructor
		///
		//////////////////////////////////
		Glyph();

		//////////////////////////////////
		/// @brief Copy constructor
		///
		//////////////////////////////////
		Glyph(const_reference copy);

		//////////////////////////////////
		/// @brief Construct the Glyph from its values
		///
		//////////////////////////////////
		Glyph(const Texture *tex,unsigned int x,unsigned int y,unsigned int w,unsigned int h,int xMin,int yMin);

		//////////////////////////////////
		/// @brief Construct the Glyph from its values
		///
		//////////////////////////////////
		Glyph(const Texture *tex,const fm::vec2u &pos,const fm::vec2u &size,const fm::vec2i &leftdown);
	};
}
#endif
