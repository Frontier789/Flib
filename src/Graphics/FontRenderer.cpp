#include <FRONTIER/Graphics/FontRenderer.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/System/Math.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SYNTHESIS_H

namespace fg
{
	/////////////////////////////////////////////////////////////
	#define FRONTIER_FT_PRINT_ERROR(func,error) fg::fg_log << "Internal FreeType function failed in " __FILE__ ": " << __LINE__ << " FT_errorCode = " << error << std::endl

	/////////////////////////////////////////////////////////////
	FontRenderer::FontRenderer() : m_ftLibrary(new FT_Library),
								   m_ftFaceData(new FT_Face),
								   m_loaded(false),
								   m_currentSize(0)
	{
		int error = FT_Init_FreeType((FT_Library*)m_ftLibrary);

		if (error)
			FRONTIER_FT_PRINT_ERROR(FT_Init_FreeType,error);
	}

	/////////////////////////////////////////////////////////////
	FontRenderer::~FontRenderer()
	{
		// free FreeType side data
		if (m_loaded)
			FT_Done_Face(*((FT_Face*)m_ftFaceData));
		FT_Done_FreeType(*((FT_Library*)m_ftLibrary));

		// delete allocated memory
		delete (FT_Library*)m_ftLibrary;
		delete (FT_Face*)m_ftFaceData;
	}

	/////////////////////////////////////////////////////////////
	bool FontRenderer::loadFromFile(const std::string &fileName,unsigned int size)
	{
		// free the current font
		if (m_loaded)
			FT_Done_Face(*((FT_Face*)m_ftFaceData));

		// load the first font face in the file
		int error = FT_New_Face(*((FT_Library*)m_ftLibrary),fileName.c_str(),0,(FT_Face*)m_ftFaceData);

		// check error
		if (error)
			FRONTIER_FT_PRINT_ERROR(FT_New_Face,error);

		m_loaded = (error==0);

		// set character size
		setCharacterSize(size);

		return m_loaded;
	}

	/////////////////////////////////////////////////////////////
	bool FontRenderer::loadFromMemory(const void *fileContent,fm::Size fileSizeInBytes,unsigned int size)
	{
		// free the current font
		if (m_loaded)
			FT_Done_Face(*((FT_Face*)m_ftFaceData));

		// load the first font face in the file
		int error = FT_New_Memory_Face(*((FT_Library*)m_ftLibrary),
									   (const FT_Byte*)fileContent,
									   fileSizeInBytes,
									   0,
									   (FT_Face*)m_ftFaceData);

		// check error
		if (error)
			FRONTIER_FT_PRINT_ERROR(FT_New_Memory_Face,error);

		m_loaded = (error==0);

		// set character size
		setCharacterSize(size);

		return m_loaded;
	}

	/////////////////////////////////////////////////////////////
	fg::Image FontRenderer::renderGlyph(const fm::Uint32 &letter,unsigned int style,fm::vector2<float> *leftDown) const
	{
		if (!m_loaded)
			return fg::Image();

		// if rendering upper/lower index decrease size
		unsigned int originalSize = m_currentSize;
		if ((style & Glyph::Subscript) xor (style & Glyph::Superscript ))
			setCharacterSize(m_currentSize*.7);

		// get glyph index
		FT_UInt glyphIndex = FT_Get_Char_Index(*((FT_Face*)m_ftFaceData),letter);

		// load glyph data
		int error = FT_Load_Glyph(*((FT_Face*)m_ftFaceData),glyphIndex,FT_LOAD_FORCE_AUTOHINT | FT_LOAD_DEFAULT);

		if (error)
		{
			FRONTIER_FT_PRINT_ERROR(FT_Load_Glyph,error);
			return fg::Image();
		}

		FT_Face face = (*((FT_Face*)m_ftFaceData));

		// embolden and italicise
		if (style & fg::Glyph::Bold)
			FT_GlyphSlot_Embolden(face->glyph);

		if (style & fg::Glyph::Italic)
			FT_GlyphSlot_Oblique(face->glyph);

		// render glyph image
		error = FT_Render_Glyph( face->glyph,FT_RENDER_MODE_NORMAL);

		if (error)
		{
			FRONTIER_FT_PRINT_ERROR(FT_Render_Glyph,error);
			return fg::Image();
		}

		// get bitmap details
		fm::Size width  = face->glyph->bitmap.width;
		fm::Size height = face->glyph->bitmap.rows;

		std::vector<fm::Uint8> bitmap(face->glyph->bitmap.buffer,face->glyph->bitmap.buffer+width*height);

		float offsetx = (face->glyph->metrics.horiBearingX>>6);
		float offsety = (face->glyph->metrics.horiBearingY>>6)-float(height);

		// manually create upper index if requested
		if (style & fg::Glyph::Superscript && !(style & fg::Glyph::Subscript))
		{
			fm::Size deltaH = height*0.4f;
			bitmap.resize(width*(height+deltaH),0);
			offsety+=deltaH;
			height+=deltaH;
		}

		// manually create lower index if requested
		if (style & fg::Glyph::Subscript && !(style & fg::Glyph::Superscript))
		{
			fm::Size deltaH = height*0.4f;
			bitmap.resize(width*(height+deltaH),0);
			offsety-=deltaH;
			Cxy(width,height)
			{
				fm::Uint8 tmp = bitmap[x+int(height+deltaH-y-1)*width];
				bitmap[x+int(height+deltaH-y-1)*width] = bitmap[x+int(height-y-1)*width];
				bitmap[x+int(height-y-1)*width] = tmp;
			}
			height+=height/2.f;
		}

		// reset the size back if needed
		if ((style & fg::Glyph::Subscript) xor (style & fg::Glyph::Superscript))
			setCharacterSize(originalSize);

		// manually strike through and/or underline if requested
		for (int i=0;i<bool(style & fg::Glyph::Crossed)+bool(style & fg::Glyph::Underlined);i++)
		{
			bool crossed = ((style & fg::Glyph::Crossed) && i==0);

			int ymax = offsety+height;
			float &ymin = offsety;

			// if the line would be out of the image (take account the y offset like in '~' as the leeter might not "sit" on the baseline)
			int lineW = int(m_metrics.maxH/15.f);
			int lineP = int(m_metrics.maxH*(crossed ? 0.2 : 0.0) );
			if (ymin > lineP)
			{
				bitmap.resize(width*(height+int(ymin-lineP)),0);

				ymin = lineP;
				height = ymax-ymin;
			}
			else if (ymax < lineW+lineP)
			{
				bitmap.resize(width*(height+(lineW+lineP-ymax)),0);

				ymax = lineW+lineP;
				height = ymax-ymin;
			}

			// simply set the value to one for every pixel in the line
			Cx(width)
				Cy((unsigned int)(lineW))
					bitmap[x+(height-1-y+int(ymin)-lineP)*width] = 255;
		}

		// manually create outline if requested
		if (style & fg::Glyph::Outline)
		{
			std::vector<fm::Uint8> oldData = bitmap;

			bitmap.resize((width+2)*(height+2));

			// get a bigger bitmap
			Cxy(width+2,height+2)
			{
				int db=0,sum=0;
				int ax=int(x)-1,ay=int(y)-1;
				int curVal = fm::rect2i(0,0,width-1,height-1).contains(fm::vec2i(ax,ay)) ? oldData[ax+ay*width] : 0;

				// this algorithm uses the difference between the neighbour pixels (the bigger the difference the bigger the output will be)
				for(int x1=-1;x1<=1;x1++) for(int y1=-1;y1<=1;y1++)
				{
					if (fm::vec2(x1,y1).LENGTH()>=1)
					{
						int deltaVal=(fm::rect2i(0,0,width-1,height-1).contains(fm::vec2i(ax+x1,ay+y1))) ? oldData[(ax+x1)+(ay+y1)*width]-curVal : -curVal;
						deltaVal*=(deltaVal > 0 ? .9 : 1.4);
						sum+=(deltaVal < 0 ? -deltaVal : deltaVal),
						db++;
					}
				}

				// do some scaling on the value and clamp it to [0;255]
				bitmap[x+y*(width+2)] = fm::math::min(255.0,db ? sum/db*1.6 : 0.0);
			}

			// update glyph image details
			width+=2,height+=2;offsetx--,offsety--;
		}

		Cxy(width,height)
		{
			float f = bitmap[y*width + x] / 255.0;
			bitmap[y*width + x] = fm::math::sqrt3(f) * 255.0;
		}

		if (leftDown)
			leftDown->x = offsetx,
			leftDown->y = offsety;

		// convert the bitmap to fg::Image
		fg::Image img;
		img.create(width,height);

		Cxy(width,height)
			img.setPixel(x,y,fg::Color(255,255,255,bitmap[y*width + x]));

		return img;
	}

	/////////////////////////////////////////////////////////////
	bool FontRenderer::hasGlyph(const fm::Uint32 &letter) const
	{
		if (!m_loaded)
			return false;

		return FT_Get_Char_Index(*((FT_Face*)m_ftFaceData),letter) != 0;
	}

	/////////////////////////////////////////////////////////////
	const fg::Metrics &FontRenderer::getMetrics() const
	{
		return m_metrics;
	}

	/////////////////////////////////////////////////////////////
	int FontRenderer::getKerning(const fm::Uint32 &leftCodePoint,const fm::Uint32 &rightCodePoint) const
	{
		if (!m_loaded)
			return 0;

		// get glyph indices
		FT_UInt leftGlyphIndex  = FT_Get_Char_Index(*((FT_Face*)m_ftFaceData),leftCodePoint);
		FT_UInt rightGlyphIndex = FT_Get_Char_Index(*((FT_Face*)m_ftFaceData),rightCodePoint);

		// retrieve kerning offset
		if (leftGlyphIndex && rightGlyphIndex)
		{
			FT_Vector delta;

			// get kerning from FreeType
			int error = FT_Get_Kerning(*((FT_Face*)m_ftFaceData),
						   			   leftGlyphIndex,
						   			   rightGlyphIndex,
						   			   FT_KERNING_DEFAULT,
						   			   &delta);
			if (error)
				FRONTIER_FT_PRINT_ERROR(FT_Get_Kerning,error);

			return delta.x >> 6;
		}

		return 0;
	}

	/////////////////////////////////////////////////////////////
	void FontRenderer::setCharacterSize(unsigned int size) const
	{
		if (!m_loaded)
			return;

		// avoid switch if unnecessary
		if (m_currentSize == size)
			return;

		m_currentSize = size;

		// tell FreeType to switch size
		int error = FT_Set_Pixel_Sizes(*((FT_Face*)m_ftFaceData),m_currentSize,0);

		// check error
		if (error)
			FRONTIER_FT_PRINT_ERROR(FT_Set_Pixel_Sizes,error);

		FT_Face face = (*((FT_Face*)m_ftFaceData));

		// simply read the metric data
		m_metrics.maxH = face->size->metrics.ascender >> 6;
		m_metrics.minH = face->size->metrics.descender >> 6;
		m_metrics.lineGap = face->size->metrics.height >> 6;
	}

	/////////////////////////////////////////////////////////////
	unsigned int FontRenderer::getCharacterSize() const
	{
		return m_currentSize;
	}
}
