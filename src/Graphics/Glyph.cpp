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

