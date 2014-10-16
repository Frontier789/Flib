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
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Rect.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype/stb_truetype.h"
#include <cstdlib>
#include <fstream>
#include <locale>
#include <cmath>
namespace fg
{
	
	////////////////////////////////////////////////////////////
	priv::CodePoint::CodePoint()
	{
		
	}
	
	////////////////////////////////////////////////////////////
	priv::CodePoint::CodePoint(char c)
	{
		// on windows we have this nice mbtowc
		#if defined(FRONTIER_OS_WINDOWS) &&                       \
		   (defined(__GLIBCPP__) || defined (__GLIBCXX__)) &&     \
		  !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION))

			wchar_t character = 0;
			mbtowc(&character, &c, 1);
			cp = (fm::Uint32)character;

		#else // otherwise we'll use std::locale
			
			cp = (fm::Uint32)(std::use_facet< std::ctype<wchar_t> >(std::locale()).widen(c));

		#endif
	}
	
	////////////////////////////////////////////////////////////
	priv::CodePoint::CodePoint(wchar_t c) : cp(c)
	{
		
	}
	
	////////////////////////////////////////////////////////////
	priv::CodePoint::operator fm::Uint32()
	{
		return cp;
	}

	/// FontImpl::GlyphMap::Identifier /////////////////////////////////////////////////////////
	priv::GlyphMap::Identifier::Identifier() {}


	////////////////////////////////////////////////////////////
	priv::GlyphMap::Identifier::Identifier(priv::CodePoint codePoint,unsigned int style) : codePoint(codePoint),
																						   style    (style)
	{

	}


	////////////////////////////////////////////////////////////
	bool priv::GlyphMap::Identifier::operator<(const Identifier &other) const
	{
		// only used when ordering/(searching ordered) storage
		return (codePoint.cp==other.codePoint.cp ? style < other.style : codePoint.cp<other.codePoint.cp);
	}


	/// FontImpl::GlyphMap::Row /////////////////////////////////////////////////////////
	priv::GlyphMap::Row::Row() {}


	////////////////////////////////////////////////////////////
	priv::GlyphMap::Row::Row(unsigned int start,unsigned int height,unsigned int width) : start (start ),
																						  height(height),
																						  width (width )
	{

	}
	
	////////////////////////////////////////////////////////////
	priv::GlyphMap::Row *findGlyphPlace(priv::GlyphMap &gmap,const Image &glyphImg)
	{
		// check for all rows if they big enough for the new glyph image
		C(gmap.rows.size())
			if (gmap.rows[i].height >= glyphImg.getSize().h+2 &&
				gmap.rows[i].height <= glyphImg.getSize().h*1.3 +2 &&
				gmap.rows[i].width+1+glyphImg.getSize().w < gmap.atlas.getSize().w)
					return &gmap.rows[i]; // if so we found it
		
		// if there is not a single row
		if (!gmap.rows.size() && glyphImg.getSize().h+2 < gmap.atlas.getSize().h)
		{
			// then add the first row (a little bigger than the glyph)
			gmap.rows.push_back(priv::GlyphMap::Row(0,glyphImg.getSize().h+2));
			return &gmap.rows[0];
		}
		else // check if adding the new row would be out of bounds
			if (gmap.rows.size() && gmap.rows.back().start+gmap.rows.back().height+glyphImg.getSize().h+2 < gmap.atlas.getSize().h)
			{
				gmap.rows.push_back(priv::GlyphMap::Row(gmap.rows.back().start+gmap.rows.back().height,glyphImg.getSize().h+2));
				return &gmap.rows.back();
			}
		return NULL;
	}
	
	
	
	
	
	/// constructors /////////////////////////////////////////////////////////
    Font::Font() : m_refCount(NULL),
                   m_renderer(NULL),
                   m_glyphMaps(NULL)
    {

    }


	////////////////////////////////////////////////////////////
    Font::Font(Font::const_reference copy) : m_refCount(copy.m_refCount),
                                             m_renderer(copy.m_renderer),
                                             m_glyphMaps(copy.m_glyphMaps)
    {
		if (m_refCount)
		    (*m_refCount)++;
    }


	////////////////////////////////////////////////////////////
    Font::~Font()
    {
    	cleanUp();
    }


	/// functions /////////////////////////////////////////////////////////
    bool Font::loadFromFile(const std::string &fileName,unsigned int size)
    {
    	init();
        return m_renderer->loadFromFile(fileName,size);
    }


	////////////////////////////////////////////////////////////
    bool Font::loadFromMemory(const void *fileContent,unsigned int fileLength,unsigned int size)
    {
        init();
        return m_renderer->loadFromMemory(fileContent,fileLength,size);
    }


	////////////////////////////////////////////////////////////
    Font::reference Font::setSmooth(bool smooth)
    {
		// set for all the atlases
		if (m_glyphMaps)
			for (std::map<unsigned int,priv::GlyphMap>::iterator it = m_glyphMaps->begin();it!=m_glyphMaps->end();it++)
				it->second.atlas.setSmooth(smooth);
		return *this;
    }


	////////////////////////////////////////////////////////////
    void Font::setSize(unsigned int size) const
    {
        if (m_renderer)
            m_renderer->setSize(size);
    }


	////////////////////////////////////////////////////////////
    Glyph Font::getGlyph(priv::CodePoint letter,unsigned int style) const
    {
    	if (!m_renderer)
			return Glyph(NULL,0,0,0,0,0,0);
		
		// we try finding it in the dictionary
        std::map<priv::GlyphMap::Identifier,Glyph>::const_iterator it=(*m_glyphMaps)[m_renderer->m_currentSize].glyphTable.find(priv::GlyphMap::Identifier(letter,style));
		if (it!=(*m_glyphMaps)[m_renderer->m_currentSize].glyphTable.end())
			return it->second;
		
		// no match, create it
		return createGlyph(letter,m_renderer->m_currentSize,style);
    }


	////////////////////////////////////////////////////////////
    Image Font::renderGlyph(priv::CodePoint letter,unsigned int style,fm::vec2 *leftDown) const
    {
        if (!m_renderer)
            return Image();
        return m_renderer->renderGlyph(letter,style,leftDown);
    }
    
	////////////////////////////////////////////////////////////
    Glyph Font::createGlyph(priv::CodePoint codePoint, unsigned int characterSize,unsigned int style) const
    {
		if (!m_renderer || !m_glyphMaps)
			return Glyph(NULL,0,0,0,0,0,0);
		
		// note if we dont have a dictionary for this size yet
		bool newTex = m_glyphMaps->find(characterSize)==m_glyphMaps->end();
		
		// notify renderer
		setSize(characterSize);
		
		priv::GlyphMap &gmap = (*m_glyphMaps)[characterSize];

		if (newTex) // if the dictionary is new we create a small image for it
			gmap.atlas.loadFromImage(Image(characterSize*1.5,characterSize*1.2,Color(0,0,0,0))),
			gmap.atlas.setRepeated(false),
			gmap.atlas.setSmooth(false);
		
		// render the glyph
		fm::vec2 leftDown;
		Image glyphImg = renderGlyph(codePoint,style,&leftDown);
		
		// find out which row we'll insert to
		priv::GlyphMap::Row *rowToInsert = findGlyphPlace(gmap,glyphImg);

		// in case we couldn't find a good row
		if (!rowToInsert)
		{
			// check if we can expand the atlas big enough
			fm::vec2 maxSize(Texture::getMaximumSize());
			fm::vec2 newSize = gmap.atlas.getSize();
			newSize.h = std::max(newSize.h*2.0,newSize.h+glyphImg.getSize().h*2.0);
			newSize.w = std::min(std::max(newSize.w*2,(float)glyphImg.getSize().w*2)  ,maxSize.w); // we can let width be maxSize.w, because we then crate a new row instead
			if (newSize.h > maxSize.h)
			{
				fg_log << "fg::Font is out of Texture space" << std::endl;
				return Glyph();
			}
			
			/** the S_L_O_W part **/
			
			Image newImg;
			Image oldImg = gmap.atlas.copyToImage(); // copy back the texture
			newImg.create(newSize,Color(0,0,0,0));  //  create bigger new image
			oldImg.copyTo(newImg,0,0);             //   copy the old image to the new
			gmap.atlas.loadFromImage(newImg);     //    send the new texture to gl
			rowToInsert = findGlyphPlace(gmap,glyphImg);
		}
		
		if (!rowToInsert) // theorically can never happen, but i dont like dereferencing NULL
			return Glyph(NULL,0,0,0,0,0,0);
		
		// Finally update the texture with the new glyph
		gmap.atlas.update(glyphImg,rowToInsert->width+1,rowToInsert->start+1);
		rowToInsert->width+=glyphImg.getSize().w+1;
		gmap.glyphTable[priv::GlyphMap::Identifier(codePoint,style)] = Glyph(&gmap.atlas,rowToInsert->width-glyphImg.getSize().w,rowToInsert->start+1,glyphImg.getSize().w,glyphImg.getSize().h,leftDown.x,leftDown.y);
		return gmap.glyphTable[priv::GlyphMap::Identifier(codePoint,style)];
    }


	////////////////////////////////////////////////////////////
	void Font::cleanUp()
	{
		// if the class was created successfully
        if (m_refCount)
        {
			// decrease shared count
            (*m_refCount)--;
			if (!(*m_refCount)) // if no more shares left free up resources
            {
				if (m_glyphMaps)
					delete m_glyphMaps;
                if (m_renderer)
                    delete m_renderer;
                if (m_refCount)
                    delete m_refCount;
            }
        }

		m_glyphMaps = NULL;
		m_renderer  = NULL;
		m_refCount  = NULL;
	}


	////////////////////////////////////////////////////////////
	void Font::init()
	{
		// first clean up so we dont leak memory
        cleanUp();
		m_renderer  = new Font::Renderer;
		m_refCount  = new(unsigned int);
		m_glyphMaps = new std::map<unsigned int,priv::GlyphMap>;
        (*m_refCount) = 1;
	}


	////////////////////////////////////////////////////////////
    Font::Metrics Font::getMetrics() const
    {
        if (!m_renderer)
        {
        	Metrics ret;
        	ret.maxH=0,
            ret.minH=0,
            ret.lineGap=0;
            return ret;
        }
        return m_renderer->getMetrics();
    }

	////////////////////////////////////////////////////////////
	const Texture &Font::getTexture() const
	{
	    return (*m_glyphMaps)[m_renderer->m_currentSize].atlas;
	}

	
	
	
	
	
	
	
	
	
	
	
	

	////////////////////////////////////////////////////////////
	Font::Renderer::Renderer() : m_loaded     (false),
								 m_fontInfo   (NULL),
								 m_scale      (0),
								 m_currentSize(0),
								 m_maxH       (0),
								 m_minH       (0),
								 m_lineGap    (0)
	{
		
	}

	////////////////////////////////////////////////////////////
	Font::Renderer::~Renderer()
	{
		if (m_fontInfo)
			delete (stbtt_fontinfo*)m_fontInfo;
	}

	////////////////////////////////////////////////////////////
	bool Font::Renderer::loadFromFile(const std::string &fileName,unsigned int size)
	{
		if (!m_fontInfo)
			 m_fontInfo = new stbtt_fontinfo;
		std::ifstream in(fileName.c_str(), std::fstream::in | std::fstream::binary);
		if (in)
		{
			// find the end of the file
			in.seekg(0, std::ios::end);
			fm::Size length = in.tellg();
			if (in.fail())
			{
				fg_log << "failed to find eof in " << fileName << std::endl;
				m_loaded = false;
				return false;
			}
			
			// we now know how many characters to expect
			m_fileContent.resize(length);
			
			// find the begining of the file
			in.seekg(0, std::ios::beg);
			
			// read length number of characters
			in.read((char*)&m_fileContent[0],length);
			if (in.fail())
			{
				fg_log << "failed to read content of " << fileName << std::endl;
				m_loaded = false;
				return false;
			}
			in.close();
		}
		else // std::ofstream failed to open
		{
			fg_log << "failed to open " << fileName << std::endl,
			m_loaded = false;
			return false;
		}
			
		// ask stbtt to initialize (the file may not be a valid ttf)
		if (!stbtt_InitFont((stbtt_fontinfo*)m_fontInfo,&m_fileContent[0],0))
		{
			fg_log << "failed to init stbtt" << std::endl;
			m_loaded = false;
			return false;
		}
		
		m_loaded = true;
		setSize(size); // set default size
		
		return true;
	}

	////////////////////////////////////////////////////////////
	bool Font::Renderer::loadFromMemory(const void *fileContent,unsigned int fileSizeInBytes,unsigned int size)
	{
		if (!m_fontInfo)
			 m_fontInfo = new stbtt_fontinfo;
		
		// copy the whole data as stbtt needs it when rasterizing
		m_fileContent = std::vector<unsigned char>((const unsigned char*)fileContent,((const unsigned char*)fileContent)+fileSizeInBytes);
		
		// ask stbtt to initialize (the data may not be a valid ttf)
		if (!stbtt_InitFont((stbtt_fontinfo*)m_fontInfo,&m_fileContent[0],0))
		{
			fg_log << "failed to init stbtt" << std::endl;
			m_loaded = false;
			return false;
		}
		
		m_loaded = true;
		setSize(size); // set default size
		
		return true;
	}

	////////////////////////////////////////////////////////////
	Image Font::Renderer::renderGlyph(priv::CodePoint letter,unsigned int style,fm::vector2<float> *leftDown) const
	{
		if (!m_loaded)
			return Image();
		
		// if rendering upper/lower index decrease size
		unsigned int originalSize = m_currentSize;
		if ((style & Style::Subscript) xor (style & Style::Superscript))
			setSize(m_currentSize*.7);
		
		// find out the size of the glyph image
		int xmin,ymin,xmax,ymax;
		stbtt_GetCodepointBitmapBox((stbtt_fontinfo*)m_fontInfo, letter, m_scale, m_scale, &xmin, &ymax, &xmax, &ymin);

		ymin*=-1;
		ymax*=-1;

		unsigned int w=(xmax-xmin),h=(ymax-ymin);
		
		// initialize plain data for the monochrome bitmap
		std::vector<unsigned char> bitmap;
		bitmap.resize(w*h);
		
		// ask stbtt to rasterize
		stbtt_MakeCodepointBitmap((stbtt_fontinfo*)m_fontInfo,&bitmap[0],w,h,w,m_scale,m_scale,letter);
		
		// if stbtt fails 
		if (w==0 && h==0)
			return Image();

		// manually embolden if requested
		if (style & Style::Bold)
		{
			// we'll need a bigger bitmap
			std::vector<unsigned char> obitmap = bitmap;
			bitmap.resize((w+2)*(h+2));
			Cxy(w+2,h+2)
			{
				int db=0,sum=0;
				int ax=int(x)-1,ay=int(y)-1;
				
				// calculate the neighbour average (increase a bit)
				for(int x1=-1;x1<=1;x1++)for(int y1=-1;y1<=1;y1++)
				{
					//if (fm::vec2(x1,y1).LENGTH()<=1)
					{
						int deltaVal=(fm::rect2i(0,0,w-1,h-1).contains(fm::vec2(ax+x1,ay+y1))) ? obitmap[(ax+x1)+(ay+y1)*w]: 0;
						sum+=std::abs(deltaVal) * (fm::vec2(x1,y1).LENGTH()>=1 ? (fm::vec2(x1,y1).LENGTH()==1 ? .22 : .12) : 1.1),
						db++;
					}
				}
				
				// write it back (check for too big value)
				bitmap[x+y*(w+2)] = std::min(255.0,db ? sum : 0.0);
			}
			
			// update glyph image details
			w+=2,h+=2,xmax++,xmin--,xmax++,ymin--;
		}
		
		// manually italicise if requested
		if (style & Style::Italic)
		{
			// the width will end up being width+height*offset
			float offset=.2;

			int newW=(w+h*offset);
			std::vector<unsigned char> obitmap = bitmap;
			bitmap.resize(0);
			bitmap.resize(newW*h,0);

			// slip every pixel to the left by offset
			Cxy(w,h)
			{
				int pt=x+y*newW - (y*offset == int(y*offset));
				float ratio = offset*y-std::floor(offset*y);
				bitmap[pt+std::floor(offset*(h-y-1))] += obitmap[x+y*w]*(ratio);
				bitmap[pt+std::ceil(offset*(h-y-1))] += obitmap[x+y*w]*(1-ratio);
			}
			C(newW*h)
				bitmap[i] = std::min(255,int(bitmap[i]));
			
			// update width
			w = newW;
		}
		
		// manually underline if requested
		if (style & Style::Underlined)
		{
			// if the line would be out of the image (take account the y offset like in '~' as the leeter might not "sit" on the baseline)
			int lineW=int(m_maxH/10.f);
			int lineMax=-2*int(m_maxH/10.f);
			if (lineMax-lineW < ymin)
			{
				int dif = ymin-(lineMax-lineW);
				bitmap.resize(w*(h+dif),0);

				ymin = lineMax-lineW;
				h = ymax-ymin;
			}
			
			// simply set the value to one for every pixel in the line
			Cx(w)
				Cy((unsigned int)(lineW))
					bitmap[x+(ymax-lineMax+y)*w] = 255;
		}
		
		// manually strike through if requested
		if (style & Style::Crossed)
		{
			// if the line would be out of the image
			int lineW=int(m_maxH/10.f);
			int lineMin=m_maxH/3.f;
			if (lineMin+lineW > ymax)
			{
				int dif = (lineMin+lineW)-ymax;
				bitmap.resize(w*(h+dif),0);
				C(w*h)
					bitmap[i+dif*w] = bitmap[i],
					bitmap[i]=0;

				ymax = lineMin+lineW;
				h = ymax-ymin;
			}
			
			// simply set the value to one for every pixel in the line
			Cx(w)
				Cy((unsigned int)(lineW))
					bitmap[x+(ymax-lineMin+y)*w] = 255;
		}
		
		// manually create outline through if requested
		if (style & Style::Outline)
		{
			// we'll end up with a bigger bitmap
			std::vector<unsigned char> obitmap = bitmap;
			bitmap.resize((w+2)*(h+2));
			Cxy(w+2,h+2)
			{
				int db=0,sum=0;
				int ax=int(x)-1,ay=int(y)-1;
				int curVal = fm::rect2i(0,0,w-1,h-1).contains(fm::vec2(ax,ay)) ? obitmap[ax+ay*w] : 0;
				
				// this algorithm uses the difference between the neighbour pixels (the bigger the difference the bigger the output will be)
				for(int x1=-1;x1<=1;x1++)for(int y1=-1;y1<=1;y1++)
				{
					if (fm::vec2(x1,y1).LENGTH()==1)
					{
						int deltaVal=(fm::rect2i(0,0,w-1,h-1).contains(fm::vec2(ax+x1,ay+y1))) ? obitmap[(ax+x1)+(ay+y1)*w]-curVal : 0;
						deltaVal*=(deltaVal > 0 ? .9 : 1.4);
						sum+=std::abs(deltaVal),
						db++;
					}
				}
				
				// do some scaling on the value and clamp it to [0;255]
				bitmap[x+y*(w+2)] = std::min(255.0,db ? sum/db*1.6 : 0.0)/*std::min(255.0,db ? std::pow(sum/db,1.1) : 0.0)*(curVal < 230 ? 1 : .5)*/;
			}
			
			// update glyph image details
			w+=2,h+=2,xmax++,xmin--,xmax++,ymin--;
		}
		
		// manually create upper index if requested
		if (style & Style::Superscript && !(style & Style::Subscript))
		{
			bitmap.resize(w*(h+h/2.f),0);
			ymax+=h/2.f;
			h+=h/2.f;
		}
		
		// manually create lower index if requested
		if (style & Style::Subscript && !(style & Style::Superscript))
		{
			bitmap.resize(w*(h+h/2.f),0);
			ymin-=h/2.f;
			Cx(w)
				Cy(h)
					std::swap(bitmap[x+int(h*1.5f-y-1)*w],bitmap[x+int(h-y-1)*w]);
			h+=h/2.f;
		}

		// set the size back if we rendered index
		if ((style & Style::Subscript) xor (style & Style::Superscript))
			setSize(originalSize);

		// create the image from the monochrome bitmap
		Image ret;
		ret.create(w,h);
		Cxy(w,h)
			ret.setPixel(x,y,Color(255,bitmap[x+y*w]));

		// if the caller needs the offset of the lower left corner, we gracefully grant it
		if (leftDown)
			(*leftDown) = fm::vec2(xmin,ymin);

		return ret;
	}

	////////////////////////////////////////////////////////////
	Font::Metrics Font::Renderer::getMetrics() const
	{
		Font::Metrics ret;
		ret.maxH = m_maxH;
		ret.minH = m_minH;
		ret.lineGap = m_lineGap;
		
		return ret;
	}

	////////////////////////////////////////////////////////////
	void Font::Renderer::setSize(unsigned int size) const
	{
		// setting size means calculating and we'd avoid it if not necessary
		if (m_loaded && size != m_currentSize)
		{
			m_currentSize = size;

			m_scale = stbtt_ScaleForPixelHeight((stbtt_fontinfo*)m_fontInfo, size);

			stbtt_GetFontVMetrics((stbtt_fontinfo*)m_fontInfo, &m_maxH, &m_minH, &m_lineGap);

			m_maxH 	  *= m_scale;
			m_minH 	  *= m_scale;
			m_lineGap *= m_scale;
		}
	}
}