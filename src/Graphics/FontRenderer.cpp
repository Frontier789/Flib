#include <FRONTIER/Graphics/FontRenderer.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <fstream>
#include <cstring>

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#define STBTT_MAX_OVERSAMPLE 64
#include "stb_truetype/stb_truetype.h"

namespace fg
{
	/////////////////////////////////////////////////////////////
	FontRenderer::FontRenderer() : m_fileContent(nullptr),
								   m_currentSize(10),
								   m_stbFontInfo(new stbtt_fontinfo),
								   m_fileSize(0),
								   m_ownData(false)
	{
		
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	FontRenderer::FontRenderer(const FontRenderer &renderer) : m_fileContent(nullptr),
															   m_currentSize(10),
															   m_stbFontInfo(new stbtt_fontinfo),
															   m_fileSize(0),
															   m_ownData(false)
	{
		(*this) = renderer;
	}
#endif

	/////////////////////////////////////////////////////////////
	FontRenderer::FontRenderer(FontRenderer &&renderer) : m_fileContent(nullptr),
														  m_currentSize(10),
														  m_stbFontInfo(new stbtt_fontinfo),
														  m_fileSize(0),
														  m_ownData(false)
	{
		renderer.swap(*this);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	FontRenderer &FontRenderer::operator=(const FontRenderer &renderer)
	{
		if (m_ownData) delete m_fileContent;
		
		m_currentSize = renderer.m_currentSize;
		m_metrics     = renderer.m_metrics;    
		m_fileSize    = renderer.m_fileSize;  
		m_ownData     = renderer.m_ownData;    
		
		if (m_ownData)
		{
			fm::Uint8 *buf = new fm::Uint8[m_fileSize];
			std::memcpy(buf,renderer.m_fileContent,m_fileSize);
			m_fileContent = buf;
		}
		else
		{
			m_fileContent = renderer.m_fileContent;
		}
		
		if (m_fileContent)
			stbtt_InitFont((stbtt_fontinfo*)m_stbFontInfo, m_fileContent, stbtt_GetFontOffsetForIndex(m_fileContent,0));
	}
#endif
	
	/////////////////////////////////////////////////////////////
	FontRenderer &FontRenderer::operator=(FontRenderer &&renderer)
	{
		return this->swap(renderer);
	}
	
	/////////////////////////////////////////////////////////////
	FontRenderer &FontRenderer::swap(FontRenderer &renderer)
	{
		std::swap(m_currentSize ,renderer.m_currentSize );
		std::swap(m_metrics     ,renderer.m_metrics     );
		std::swap(m_stbFontInfo ,renderer.m_stbFontInfo );
		std::swap(m_fileContent ,renderer.m_fileContent );
		std::swap(m_fileSize    ,renderer.m_fileSize    );
		std::swap(m_ownData     ,renderer.m_ownData     );
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	FontRenderer::~FontRenderer()
	{
		// free FreeType side data
		if (m_ownData)
			delete[] m_fileContent;
		
		delete (stbtt_fontinfo*)m_stbFontInfo;
	}

	/////////////////////////////////////////////////////////////
	void FontRenderer::clean()
	{
		if (m_ownData) delete[] m_fileContent;
		
		m_fileContent = nullptr;
		m_ownData  = false;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result FontRenderer::loadFromFile(const std::string &fileName,unsigned int size)
	{
		clean();
		
		m_ownData = true;

		std::ifstream in(fileName.c_str(), std::fstream::in | std::fstream::binary);
		
		if (!in)
		{
			clean();
			return fm::Result("IOError",fm::Result::OPFailed,"FileNotFound","loadFromFile",__FILE__,__LINE__,fileName);
		}
		
		in.seekg(0, std::ios::end);
		m_fileSize = in.tellg();
		
		if (in.fail())
		{
			clean();
			return fm::Result("IOError",fm::Result::OPFailed,"FileNotReadable","loadFromFile",__FILE__,__LINE__,fileName);
		}
		
		m_fileContent = new fm::Uint8[m_fileSize];
		in.seekg(0, std::ios::beg);
		in.read((char*)m_fileContent, m_fileSize);
		
		if (in.fail())
		{
			clean();
			return fm::Result("IOError",fm::Result::OPFailed,"FileNotReadable","loadFromFile",__FILE__,__LINE__,fileName);
		}
		
		in.close();
			
		if (!stbtt_InitFont((stbtt_fontinfo*)m_stbFontInfo, m_fileContent, stbtt_GetFontOffsetForIndex(m_fileContent,0)))
		{
			clean();
			return fm::Result("FTError",fm::Result::OPFailed,"InvalidFont","loadFromFile",__FILE__,__LINE__,fileName);
		}
		
		setCharacterSize(size);
			
		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	fm::Result FontRenderer::loadFromMemory(const fm::Uint8 *fileContent,fm::Size fileSizeInBytes,unsigned int size)
	{
		clean();
		
		m_ownData   = false;
		m_fileSize = fileSizeInBytes;
		m_fileContent  = fileContent;
		
		if (!stbtt_InitFont((stbtt_fontinfo*)m_stbFontInfo, m_fileContent, stbtt_GetFontOffsetForIndex(m_fileContent,0)))
		{
			clean();
			return fm::Result("FTError",fm::Result::OPFailed,"InvalidFont","loadFromMemory",__FILE__,__LINE__);
		}
		
		setCharacterSize(size);
			
		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	fm::Result FontRenderer::copyFromMemory(const fm::Uint8 *fileContent,fm::Size fileSizeInBytes,unsigned int size)
	{
		clean();
		
		m_ownData   = true;
		m_fileSize = fileSizeInBytes;
		
		fm::Uint8 *buf = new fm::Uint8[fileSizeInBytes];
		std::memcpy(buf,fileContent,fileSizeInBytes);
		m_fileContent = buf;
		
		if (!stbtt_InitFont((stbtt_fontinfo*)m_stbFontInfo, m_fileContent, stbtt_GetFontOffsetForIndex(m_fileContent,0)))
		{
			clean();
			return fm::Result("FTError",fm::Result::OPFailed,"InvalidFont","copyFromMemory",__FILE__,__LINE__);
		}
		
		setCharacterSize(size);
			
		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	fg::Image FontRenderer::renderGlyph(const fm::Uint32 &letter,unsigned int style,fm::vec2 *leftDown) const
	{
		if (!m_fileContent) 
			return fg::Image();
		
		if (letter == ' ')
		{
			float scl = stbtt_ScaleForMappingEmToPixels((stbtt_fontinfo*)m_stbFontInfo, m_currentSize);
			
			int ix0,ix1,iy0,iy1;
			
			stbtt_GetCodepointBitmapBox((stbtt_fontinfo*)m_stbFontInfo, 'w', scl, scl, &ix0, &iy0, &ix1, &iy1);
			
			return fg::Image(fm::vec2s(ix1-ix0,iy1-iy0),fg::Color(255,255,255,0));
		}
		
		unsigned char *bitmap;
		fm::vec2i size,offset;

		bitmap = stbtt_GetCodepointBitmap((stbtt_fontinfo*)m_stbFontInfo, 0,stbtt_ScaleForMappingEmToPixels((stbtt_fontinfo*)m_stbFontInfo, m_currentSize), letter, &size.w, &size.h, &offset.x, &offset.y);
		
		if (leftDown)
			*leftDown = offset;
		
		fg::Image img;
		img.create(size);
		
		Cv(size)
			img.set(p,fg::Color(255,bitmap[p.x + p.y*size.w]));
		
		stbtt_FreeBitmap(bitmap,0);
		
		return img;
/*
		// if rendering upper/lower index decrease size
		unsigned int originalSize = m_currentSize;
		if ((style & Glyph::Subscript) xor (style & Glyph::Superscript ))
			setCharacterSize(m_currentSize*.7);

		// get glyph index
		FT_UInt glyphIndex = FT_Get_Char_Index(*((FT_Face*)m_ftFaceData),letter);

		// load glyph data
		int error = FT_Load_Glyph(*((FT_Face*)m_ftFaceData),glyphIndex,FT_LOAD_FORCE_AUTOHINT | FT_LOAD_DEFAULT);

		if (error)
			return fg::Image();

		FT_Face face = (*((FT_Face*)m_ftFaceData));

		// embolden and italicise
		if (style & fg::Glyph::Bold)
			FT_GlyphSlot_Embolden(face->glyph);

		if (style & fg::Glyph::Italic)
			FT_GlyphSlot_Oblique(face->glyph);

		// render glyph image
		error = FT_Render_Glyph( face->glyph,FT_RENDER_MODE_NORMAL);

		if (error)
			return fg::Image();

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
*/
		return img;
	}

	/////////////////////////////////////////////////////////////
	bool FontRenderer::hasGlyph(const fm::Uint32 &letter) const
	{
		if (!m_fileContent)
			return false;

		return stbtt_FindGlyphIndex((stbtt_fontinfo*)m_stbFontInfo,letter) != 0;
	}

	/////////////////////////////////////////////////////////////
	const fg::Metrics &FontRenderer::getMetrics() const
	{
		return m_metrics;
	}

	/////////////////////////////////////////////////////////////
	int FontRenderer::getKerning(const fm::Uint32 &leftCodePoint,const fm::Uint32 &rightCodePoint) const
	{
		if (!m_fileContent)
			return 0;
		
		return stbtt_ScaleForMappingEmToPixels((stbtt_fontinfo*)m_stbFontInfo, m_currentSize) * stbtt_GetCodepointKernAdvance((stbtt_fontinfo*)m_stbFontInfo, leftCodePoint,rightCodePoint);
	}

	/////////////////////////////////////////////////////////////
	const FontRenderer &FontRenderer::setCharacterSize(unsigned int size) const
	{
		if (m_currentSize == size)
			return *this;
			
		m_currentSize = size;
			
		if (!m_fileContent)
			return *this;
		
		stbtt_GetFontVMetrics((stbtt_fontinfo*)m_stbFontInfo,&m_metrics.maxH,&m_metrics.minH,&m_metrics.lineGap);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	unsigned int FontRenderer::getCharacterSize() const
	{
		return m_currentSize;
	}
}
