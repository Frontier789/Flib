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
#define FRONTIER_GLYPH
namespace fg
{
	class Texture;
	//////////////////////////////////
	/// Glyph
	//////////////////////////////////
	class Glyph
	{
	public:
		typedef Glyph &reference;
		typedef const Glyph &const_reference;

		const Texture *tex;
		unsigned int x,y,w,h;
		int yMin,xMin;

		//////////////////////////////////
		/// constructors
		//////////////////////////////////
		Glyph();
		Glyph(const Texture *tex,unsigned int x,unsigned int y,unsigned int w,unsigned int h,int xMin,int yMin);
	};
}
#endif
