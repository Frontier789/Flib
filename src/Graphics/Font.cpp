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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/TextureAtlas.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Rect.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype/stb_truetype.h"
#include <cstdlib>
#include <fstream>
#include <locale>
#include <cmath>
namespace fg
{
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Font::Identifier
	{
	public:
		CodePoint codePoint; ///< The codepoint of the character
		unsigned int style;  ///< The style of the character (e.g. Underlined)

		/////////////////////////////////////////////////////////////
		Identifier();

		/////////////////////////////////////////////////////////////
		Identifier(const CodePoint &codePoint,unsigned int style);

		/////////////////////////////////////////////////////////////
		bool operator<(const Identifier &other) const;
	};

	/// FontImpl::GlyphMap::Identifier /////////////////////////////////////////////////////////
	Font::Identifier::Identifier() {}


	////////////////////////////////////////////////////////////
	Font::Identifier::Identifier(const CodePoint &codePoint,unsigned int style) : codePoint(codePoint),
																				  style    (style)
	{

	}

	////////////////////////////////////////////////////////////
	bool Font::Identifier::operator<(const Identifier &other) const
	{
		// used for sorting
		return (codePoint.cp==other.codePoint.cp ? style < other.style : codePoint.cp<other.codePoint.cp);
	}

	/// constructors /////////////////////////////////////////////////////////
    Font::Font() : m_refCount(fm::nullPtr),
                   m_renderer(fm::nullPtr),
                   m_TexAtlases(fm::nullPtr)
    {

    }


	////////////////////////////////////////////////////////////
    Font::Font(Font::const_reference copy) : m_refCount(copy.m_refCount),
                                             m_renderer(copy.m_renderer),
                                             m_TexAtlases(copy.m_TexAtlases)
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
		if (m_TexAtlases)
			for (std::map<unsigned int,TextureAtlas<Identifier> >::iterator it = m_TexAtlases->begin();it!=m_TexAtlases->end();it++)
				it->second.getTexture().setSmooth(smooth);
		return *this;
    }


	////////////////////////////////////////////////////////////
    void Font::setCharacterSize(unsigned int size) const
    {
        if (m_renderer)
            m_renderer->setCharacterSize(size);
    }


	////////////////////////////////////////////////////////////
    Glyph Font::getGlyph(const CodePoint &letter,unsigned int style) const
    {
    	if (!m_renderer)
			return Glyph();

		// try finding it in the dictionary
		const TextureAtlas<Identifier> &texAtlas = (*m_TexAtlases)[m_renderer->m_currentSize];
		Glyph glyph = texAtlas.fetch(Identifier(letter,style));
		
		if (glyph!=Glyph())
			return glyph;

		// no match, create it
		fm::vec2 leftdown;
		fg::Image img = renderGlyph(letter,style,&leftdown);
		
		return (*m_TexAtlases)[m_renderer->m_currentSize].upload(img,Identifier(letter,style),leftdown);
    }


	////////////////////////////////////////////////////////////
    Image Font::renderGlyph(const CodePoint &letter,unsigned int style,fm::vec2 *leftDown) const
    {
        if (!m_renderer)
            return Image();
        return m_renderer->renderGlyph(letter,style,leftDown);
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
				if (m_TexAtlases)
					delete m_TexAtlases;
                if (m_renderer)
                    delete m_renderer;
                if (m_refCount)
                    delete m_refCount;
            }
        }

		m_TexAtlases = fm::nullPtr;
		m_renderer   = fm::nullPtr;
		m_refCount   = fm::nullPtr;
	}


	////////////////////////////////////////////////////////////
	void Font::init()
	{
		// first clean up
        cleanUp();
		m_renderer  = new Font::Renderer;
		m_refCount  = new(unsigned int);
		m_TexAtlases = new std::map<unsigned int,TextureAtlas<Identifier> >;
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
    
	/////////////////////////////////////////////////////////////
	int Font::getKerning(const CodePoint &leftCodePoint,const CodePoint &rightCodePoint) const
	{
		if (m_renderer)
			return m_renderer->getKerning(leftCodePoint,rightCodePoint);
		
		return 0;
	}

	////////////////////////////////////////////////////////////
	const Texture &Font::getTexture() const
	{
	    return (*m_TexAtlases)[m_renderer->m_currentSize].getTexture();
	}
	
	/////////////////////////////////////////////////////////////
	Glyph Font::upload(const fg::Image &img,const CodePoint &letter,unsigned int type,const fm::vec2s &leftdown,unsigned int characterSize)
	{
		return (*m_TexAtlases)[characterSize ? characterSize : m_renderer->m_currentSize].upload(img,Identifier(letter,type),leftdown);
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

			// allocate file content buffer
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
		setCharacterSize(size); // set default size

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
		setCharacterSize(size); // set default size

		return true;
	}

	////////////////////////////////////////////////////////////
	Image Font::Renderer::renderGlyph(const CodePoint &letter,unsigned int style,fm::vector2<float> *leftDown) const
	{
		if (!m_loaded)
			return Image();

		// if rendering upper/lower index decrease size
		unsigned int originalSize = m_currentSize;
		if ((style & Font::Subscript) xor (style & Font::Superscript))
			setCharacterSize(m_currentSize*.7);

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
		
		
		// sharpen the small images
		//if (m_lineGap < 20)
		/*
			Cxy(w,h)
			{
				int sum = 0;
				Cf(3,dx)
					Cf(3,dy)
					{
						fm::vec2 p(x,y),d(dx-1.0,dy-1.0);
						int dist = int(dx!=1) + int(dy!=1);
						int val = fm::rect2i(0,0,w-1,h-1).contains(p+d) ? bitmap[(x+d.x)+(y+d.y)*w] : 0;
						sum+=val * (dist==0 ? 1.2 : (dist==1 ? .1 : .05));
					}
				// write it back (check for too big value)
				unsigned char &b = bitmap[x+y*w];
				unsigned char val = std::min(255,sum);
				b = b<30 ? b : (b < 90 ? (b/2+val/2) : val);
			}*/

		// manually embolden if requested
		if (style & Font::Bold)
		{
			// allocate a bigger bitmap
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
						sum+=deltaVal * (fm::vec2(x1,y1).LENGTH()>=1 ? (fm::vec2(x1,y1).LENGTH()==1 ? .22 : .12) : 1.1),
						db++;
					}
				}

				// write it back (check for too big value)
				bitmap[x+y*(w+2)] = std::min(255,sum);
			}

			// update glyph image details
			w+=2,h+=2,xmax++,xmin--,xmax++,ymin--;
		}

		// manually italicise if requested
		if (style & Font::Italic)
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
		if (style & Font::Underlined)
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
		if (style & Font::Crossed)
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
		if (style & Font::Outline)
		{
			// get a bigger bitmap
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
		if (style & Font::Superscript && !(style & Font::Subscript))
		{
			bitmap.resize(w*(h+h/2.f),0);
			ymax+=h/2.f;
			h+=h/2.f;
		}

		// manually create lower index if requested
		if (style & Font::Subscript && !(style & Font::Superscript))
		{
			bitmap.resize(w*(h+h/2.f),0);
			ymin-=h/2.f;
			Cx(w)
				Cy(h)
					std::swap(bitmap[x+int(h*1.5f-y-1)*w],bitmap[x+int(h-y-1)*w]);
			h+=h/2.f;
		}

		// reset the size back if needed
		if ((style & Font::Subscript) xor (style & Font::Superscript))
			setCharacterSize(originalSize);

		// create the image from the monochrome bitmap
		Image ret;
		ret.create(w,h);
		Cxy(w,h)
			ret.setPixel(x,y,Color(255,bitmap[x+y*w]));

		// give the caller the offset of the lower left corner
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
    
	/////////////////////////////////////////////////////////////
	int Font::Renderer::getKerning(const CodePoint &leftCodePoint,const CodePoint &rightCodePoint) const
	{
		if (m_loaded)
			return stbtt_GetCodepointKernAdvance((stbtt_fontinfo*)m_fontInfo,leftCodePoint,rightCodePoint)*m_scale;
		
		return 0;
	}

	////////////////////////////////////////////////////////////
	void Font::Renderer::setCharacterSize(unsigned int size) const
	{
		// avoid setting same size twice
		if (m_loaded && size != m_currentSize)
		{
			m_currentSize = size;

			m_scale = stbtt_ScaleForPixelHeight((stbtt_fontinfo*)m_fontInfo, size);

			stbtt_GetFontVMetrics((stbtt_fontinfo*)m_fontInfo, &m_maxH, &m_minH, &m_lineGap);

			m_maxH 	  *= m_scale;
			m_minH 	  *= m_scale;
			m_lineGap *= m_scale;
			m_lineGap += m_maxH-m_minH;
		}
	}
}
