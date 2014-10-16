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
namespace fg
{
    /// constructors /////////////////////////////////////////////////////////
    Glyph::Glyph() {}


    ////////////////////////////////////////////////////////////
	Glyph::Glyph(const Texture *tex,unsigned int x,unsigned int y,unsigned int w,unsigned int h,int xMin,int yMin) : tex(tex),
                                                                                                                     x(x),
                                                                                                                     y(y),
                                                                                                                     w(w),
                                                                                                                     h(h),
                                                                                                                     yMin(yMin),
                                                                                                                     xMin(xMin)
    {

    }
}
