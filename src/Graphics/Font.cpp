//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
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
		#if defined(FRONTIER_OS_WINDOWS) &&                       \
		   (defined(__GLIBCPP__) || defined (__GLIBCXX__)) &&     \
		  !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION))

			wchar_t character = 0;
			mbtowc(&character, &c, 1);
			cp = (fm::Uint32)character;

		#else

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
		C(gmap.rows.size())
			if (gmap.rows[i].height >= glyphImg.getSize().h &&
				gmap.rows[i].height <= glyphImg.getSize().h*1.3 +2 &&
				gmap.rows[i].width+1+glyphImg.getSize().w < gmap.atlas.getSize().w)
					return &gmap.rows[i];

		if (!gmap.rows.size() && glyphImg.getSize().h*1.1 < gmap.atlas.getSize().h)
		{
			gmap.rows.push_back(priv::GlyphMap::Row(0,glyphImg.getSize().h*1.1));
			return &gmap.rows[0];
		}
		else if (gmap.rows.size() && gmap.rows.back().start+gmap.rows.back().height+glyphImg.getSize().h*1.1 < gmap.atlas.getSize().h)
		{
			gmap.rows.push_back(priv::GlyphMap::Row(gmap.rows.back().start+gmap.rows.back().height,glyphImg.getSize().h*1.1));
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
    bool Font::loadFromFile(const std::string &fileName)
    {
    	init();
        return m_renderer->loadFromFile(fileName);
    }


	////////////////////////////////////////////////////////////
    bool Font::loadFromMemory(const void *fileContent,unsigned int fileLength)
    {
        init();
        return m_renderer->loadFromMemory(fileContent,fileLength);
    }


	////////////////////////////////////////////////////////////
    Font::reference Font::setSmooth(bool smooth)
    {
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
        std::map<priv::GlyphMap::Identifier,Glyph>::const_iterator it=(*m_glyphMaps)[m_renderer->m_currentSize].glyphTable.find(priv::GlyphMap::Identifier(letter,style));
		if (it!=(*m_glyphMaps)[m_renderer->m_currentSize].glyphTable.end())
			return it->second;
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
    	if (!m_renderer)
			return Glyph(NULL,0,0,0,0,0,0);
		setSize(characterSize);
		
		bool newTex = ((*m_glyphMaps)[characterSize].glyphTable.find(priv::GlyphMap::Identifier(codePoint,style)) == (*m_glyphMaps)[characterSize].glyphTable.end());
		
		priv::GlyphMap &gmap = (*m_glyphMaps)[characterSize];

		if (newTex)
			gmap.atlas.loadFromImage(Image(characterSize,characterSize,Color(0,0,0,0))),
			gmap.atlas.setRepeated(false),
			gmap.atlas.setSmooth(false);

		fm::vec2 leftDown;
		Image glyphImg = renderGlyph(codePoint,style,&leftDown);

		priv::GlyphMap::Row *rowToInsert = findGlyphPlace(gmap,glyphImg);


		if (!rowToInsert)
		{
			fm::vec2 maxSize(Texture::getMaximumSize());
			Image oldImg = gmap.atlas.copyToImage(),newImg;
			fm::vec2 newSize = oldImg.getSize();
			newSize.h = std::max(newSize.h*2.0,newSize.h+glyphImg.getSize().h*2.0);
			newSize.w = std::min(std::max(newSize.w*2,glyphImg.getSize().w*2)  ,maxSize.w);
			if (newSize.h > maxSize.h)
			{
				fg_log << "font couldn't send glyph texture to gl, because its too big" << std::endl;
				return Glyph();
			}
			newImg.create(newSize,Color(0,0,0,0));
			oldImg.copyTo(newImg,0,0);
			gmap.atlas.loadFromImage(newImg);
			rowToInsert = findGlyphPlace(gmap,glyphImg);
		}

		gmap.atlas.update(glyphImg,rowToInsert->width+1,rowToInsert->start+1);
		rowToInsert->width+=glyphImg.getSize().w+1;
		gmap.glyphTable[priv::GlyphMap::Identifier(codePoint,style)] = Glyph(&gmap.atlas,rowToInsert->width-glyphImg.getSize().w,rowToInsert->start+1,glyphImg.getSize().w,glyphImg.getSize().h,leftDown.x,leftDown.y);
		return gmap.glyphTable[priv::GlyphMap::Identifier(codePoint,style)];
    }


	////////////////////////////////////////////////////////////
	void Font::cleanUp()
	{
        if (m_refCount)
        {
            (*m_refCount)--;
            if (!(*m_refCount))
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
	bool Font::Renderer::loadFromFile(const std::string &fileName)
	{
		if (!m_fontInfo)
			 m_fontInfo = new stbtt_fontinfo;
		std::ifstream in(fileName.c_str(), std::fstream::in | std::fstream::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			std::size_t length = in.tellg();
			if (in.fail())
			{
				fg_log << "failed to find eof in " << fileName << std::endl;
				m_loaded = false;
				return false;
			}
			m_fileContent.resize(length);
			in.seekg(0, std::ios::beg);
			in.read((char*)&m_fileContent[0],length);
			if (in.fail())
			{
				fg_log << "failed to read content of " << fileName << std::endl;
				m_loaded = false;
				return false;
			}
			in.close();
		}
		else
		{
			fg_log << "failed to open " << fileName << std::endl,
			m_loaded = false;
			return false;
		}
			
		
		if (!stbtt_InitFont((stbtt_fontinfo*)m_fontInfo,&m_fileContent[0],0))
		{
			fg_log << "failed to init stbtt" << std::endl;
			m_loaded = false;
			return false;
		}
		
		m_loaded = true;
		setSize(64);
		
		return true;
	}

	////////////////////////////////////////////////////////////
	bool Font::Renderer::loadFromMemory(const void *fileContent,unsigned int fileSizeInBytes)
	{
		if (!m_fontInfo)
			 m_fontInfo = new stbtt_fontinfo;
		
		m_fileContent = std::vector<unsigned char>((const unsigned char*)fileContent,((const unsigned char*)fileContent)+fileSizeInBytes);
			
		if (!stbtt_InitFont((stbtt_fontinfo*)m_fontInfo,&m_fileContent[0],0))
		{
			fg_log << "failed to init stbtt" << std::endl;
			m_loaded = false;
			return false;
		}
		
		m_loaded = true;
		setSize(64);
		
		return true;
	}

	////////////////////////////////////////////////////////////
	Image Font::Renderer::renderGlyph(priv::CodePoint letter,unsigned int style,fm::vector2<float> *leftDown) const
	{
		if (!m_loaded)
			return Image();
		if ((style & Style::Subscript) xor (style & Style::Superscript))
			setSize(m_currentSize*.7);
		
		
		int xmin,ymin,xmax,ymax;
		stbtt_GetCodepointBitmapBox((stbtt_fontinfo*)m_fontInfo, letter, m_scale, m_scale, &xmin, &ymax, &xmax, &ymin);

		ymin*=-1;
		ymax*=-1;


		unsigned int w=(xmax-xmin),h=(ymax-ymin);

		std::vector<unsigned char> bitmap;
		bitmap.resize(w*h);

		stbtt_MakeCodepointBitmap((stbtt_fontinfo*)m_fontInfo,&bitmap[0],w,h,w,m_scale,m_scale,letter);
		
		if (w==0 && h==0)
			return Image();


		if (style & Style::Bold)
		{
			std::vector<unsigned char> obitmap = bitmap;
			bitmap.resize((w+2)*(h+2));
			Cxy(w+2,h+2)
			{
				int db=0,sum=0;
				int ax=int(x)-1,ay=int(y)-1;

				for(int x1=-1;x1<=1;x1++)for(int y1=-1;y1<=1;y1++)
				{
					//if (fm::vec2(x1,y1).LENGTH()<=1)
					{
						int deltaVal=(fm::rect2i(0,0,w-1,h-1).contains(fm::vec2(ax+x1,ay+y1))) ? obitmap[(ax+x1)+(ay+y1)*w]: 0;
						sum+=std::abs(deltaVal) * (fm::vec2(x1,y1).LENGTH()>=1 ? (fm::vec2(x1,y1).LENGTH()==1 ? .22 : .12) : 1.1),
						db++;
					}
				}
				bitmap[x+y*(w+2)] = std::min(255.0,db ? sum : 0.0);
			}
			w+=2,h+=2,xmax++,xmin--,xmax++,ymin--;
		}
		if (style & Style::Italic)
		{
			float offset=.2;

			int newW=(w+h*offset);
			std::vector<unsigned char> obitmap = bitmap;
			bitmap.resize(0);
			bitmap.resize(newW*h,0);

			Cxy(w,h)
			{
				int pt=x+y*newW - (y*offset == int(y*offset));
				float ratio = offset*y-std::floor(offset*y);
				bitmap[pt+std::floor(offset*(h-y-1))] += obitmap[x+y*w]*(ratio);
				bitmap[pt+std::ceil(offset*(h-y-1))] += obitmap[x+y*w]*(1-ratio);
			}
			C(newW*h)
				bitmap[i] = std::min(255,int(bitmap[i]));

			w=newW;
		}
		if (style & Style::Underlined)
		{
			int lineW=int(m_maxH/10.f);
			int lineMax=-2*int(m_maxH/10.f);
			if (lineMax-lineW < ymin)
			{
				int dif = ymin-(lineMax-lineW);
				bitmap.resize(w*(h+dif),0);

				ymin = lineMax-lineW;
				h = ymax-ymin;
			}
			Cx(w)
				Cy((unsigned int)(lineW))
					bitmap[x+(ymax-lineMax+y)*w] = 255;
		}
		if (style & Style::Crossed)
		{
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
			Cx(w)
				Cy((unsigned int)(lineW))
					bitmap[x+(ymax-lineMin+y)*w] = 255;
		}
		if (style & Style::Outline)
		{
			std::vector<unsigned char> obitmap = bitmap;
			bitmap.resize((w+2)*(h+2));
			Cxy(w+2,h+2)
			{
				int db=0,sum=0;
				int ax=int(x)-1,ay=int(y)-1;
				int curVal = fm::rect2i(0,0,w-1,h-1).contains(fm::vec2(ax,ay)) ? obitmap[ax+ay*w] : 0;

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
				bitmap[x+y*(w+2)] = std::min(255.0,db ? sum/db*1.6 : 0.0)/*std::min(255.0,db ? std::pow(sum/db,1.1) : 0.0)*(curVal < 230 ? 1 : .5)*/;
			}
			w+=2,h+=2,xmax++,xmin--,xmax++,ymin--;
		}
		if (style & Style::Superscript && !(style & Style::Subscript))
		{
			bitmap.resize(w*(h+h/2.f),0);
			ymax+=h/2.f;
			h+=h/2.f;
		}
		if (style & Style::Subscript && !(style & Style::Superscript))
		{
			bitmap.resize(w*(h+h/2.f),0);
			ymin-=h/2.f;
			Cx(w)
				Cy(h)
					std::swap(bitmap[x+int(h*1.5f-y-1)*w],bitmap[x+int(h-y-1)*w]);
			h+=h/2.f;
		}


		if ((style & Style::Subscript) xor (style & Style::Superscript))
			setSize(m_currentSize/.7);


		Image ret;
		ret.create(w,h);
		Cxy(w,h)
			ret.setPixel(x,y,Color(255,bitmap[x+y*w]));


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
