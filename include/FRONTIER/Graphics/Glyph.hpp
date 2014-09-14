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
