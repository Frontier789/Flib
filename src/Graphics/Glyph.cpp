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
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/System/Vector2.hpp>
namespace fg
{
    /// constructors /////////////////////////////////////////////////////////
	Glyph::Glyph() : tex(0)
	{

	}

    ////////////////////////////////////////////////////////////
	Glyph::Glyph(const Glyph &copy) : tex(copy.tex),
									  pos(copy.pos),
									  size(copy.size),
									  leftdown(copy.leftdown)
	{
		
	}


    ////////////////////////////////////////////////////////////
	Glyph::Glyph(const Texture *tex,unsigned int x,unsigned int y,unsigned int w,unsigned int h,int xMin,int yMin) : tex(tex),
                                                                                                                     pos(x,y),
                                                                                                                     size(w,h),
                                                                                                                     leftdown(yMin,xMin)
    {

    }

	//////////////////////////////////
	Glyph::Glyph(const Texture *tex,const fm::vec2u &pos,const fm::vec2u &size,const fm::vec2i &leftdown) : tex(tex),
																											pos(pos),
																											size(size),
																											leftdown(leftdown)
	{

	}
}

