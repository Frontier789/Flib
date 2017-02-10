////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Graphics/FontRenderer.hpp>
#include <FRONTIER/Graphics/TextureAtlas.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	Font::Identifier::Identifier() {}


	////////////////////////////////////////////////////////////
	Font::Identifier::Identifier(const fm::Uint32 &codePoint,unsigned int style) : codePoint(codePoint),
																				   style    (style)
	{

	}

	////////////////////////////////////////////////////////////
	bool Font::Identifier::operator<(const Identifier &other) const
	{
		// used for sorting
		return (codePoint==other.codePoint ? style < other.style : codePoint < other.codePoint);
	}

	////////////////////////////////////////////////////////////
    Font::Font() : m_renderer(nullptr),
                   m_texAtlases(nullptr)
    {

    }

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
    Font::Font(Font::const_reference copy) : m_renderer(nullptr),
                                             m_texAtlases(nullptr)
    {
		(*this) = copy;
    }
#endif

	////////////////////////////////////////////////////////////
	Font::Font(Font &&move) : m_renderer(nullptr),
                              m_texAtlases(nullptr)
    {
		move.swap(*this);
    }

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	Font &Font::operator=(Font::const_reference copy)
    {
		FRONTIER_HEAVYCOPY_NOTE;
		
		delete m_renderer;
		delete m_texAtlases;
		
		m_renderer = nullptr;
		m_renderer = nullptr;
		
		if (copy.m_renderer)
		{
			m_renderer   = new FontRenderer(*copy.m_renderer);
			m_texAtlases = std::map<unsigned int,TextureAtlas<Identifier> >;
		}
    }
#endif

	////////////////////////////////////////////////////////////
	Font &Font::operator=(Font &&move)
    {
		return this->swap(move);
    }

	////////////////////////////////////////////////////////////
    Font::~Font()
    {
    	cleanUp();
    }

	////////////////////////////////////////////////////////////
    fm::Result Font::loadFromFile(const std::string &fileName,unsigned int size)
    {
    	init();
        return m_renderer->loadFromFile(fileName,size);
    }

	////////////////////////////////////////////////////////////
    fm::Result Font::loadFromMemory(const fm::Uint8 *fileContent,fm::Size fileLength,unsigned int size)
    {
        init();
        return m_renderer->loadFromMemory(fileContent,fileLength,size);
    }

	////////////////////////////////////////////////////////////
    fm::Result Font::copyFromMemory(const fm::Uint8 *fileContent,fm::Size fileLength,unsigned int size)
    {
        init();
        return m_renderer->copyFromMemory(fileContent,fileLength,size);
    }

	////////////////////////////////////////////////////////////
    Font::reference Font::setSmooth(bool smooth)
    {
		// set for all the atlases
		if (m_texAtlases)
			for (std::map<unsigned int,TextureAtlas<Identifier> >::iterator it = m_texAtlases->begin();it!=m_texAtlases->end();it++)
				it->second.getTexture().setSmooth(smooth);

		return *this;
    }


	////////////////////////////////////////////////////////////
    void Font::setCharacterSize(unsigned int size) const
    {
        if (m_renderer)
            m_renderer->setCharacterSize(size);
    }

	/////////////////////////////////////////////////////////////
	unsigned int Font::getCharacterSize() const
	{
		if (!m_renderer)
			return 0;

		return m_renderer->getCharacterSize();
	}

	/////////////////////////////////////////////////////////////
	FontRenderer *Font::getRenderer()
	{
		return m_renderer;
	}


	////////////////////////////////////////////////////////////
    Glyph Font::getGlyph(const fm::Uint32 &letter,unsigned int style) const
    {
    	if (!m_renderer)
			return Glyph();

		// try finding it in the dictionary
		Glyph glyph = (*m_texAtlases)[m_renderer->getCharacterSize()].fetch(Identifier(letter,style));
		
		if (glyph != Glyph())
			return glyph;

		// no match, create it
		fm::vec2 leftdown;
		fg::Image img = renderGlyph(letter,style,&leftdown);
		
		return (*m_texAtlases)[m_renderer->getCharacterSize()].upload(img,Identifier(letter,style),leftdown);
    }
    
	/////////////////////////////////////////////////////////////
	void Font::preCache(const fm::String &characters,unsigned int type) const
	{
		if (!m_renderer)
			return;

		TextureAtlas<Identifier> &texAtl = (*m_texAtlases)[m_renderer->getCharacterSize()];
		
		std::vector<TextureAtlas<Identifier>::MapPoint> mapPts;
		
		C(characters.size())
		{
			fm::Uint32 c = characters[i];
			Glyph g = texAtl.fetch(Identifier(c,type));
			
			if (g == Glyph())
			{
				fm::vec2 leftdown;
				fg::Image *img = new fg::Image(renderGlyph(c,type,&leftdown));
				
				mapPts.push_back(TextureAtlas<Identifier>::MapPoint(img,Identifier(c,type),leftdown));
			}
		}
		
		texAtl.upload(&mapPts[0],mapPts.size());
		
		C(mapPts.size())
		{
			delete mapPts[i].img;
		}
	}


	////////////////////////////////////////////////////////////
    Image Font::renderGlyph(const fm::Uint32 &letter,unsigned int style,fm::vec2 *leftDown) const
    {
        if (!m_renderer)
            return Image();

        return m_renderer->renderGlyph(letter,style,leftDown);
    }
    
	/////////////////////////////////////////////////////////////
	bool Font::hasGlyph(const fm::Uint32 &letter,unsigned int style) const
	{
		if (m_texAtlases)
			if ((*m_texAtlases)[m_renderer->getCharacterSize()].isUploaded(Identifier(letter,style)))
				return true;
		
		if (!m_renderer)
			return false;
		
		return m_renderer->hasGlyph(letter);
	}

	////////////////////////////////////////////////////////////
	void Font::cleanUp()
	{
		delete m_texAtlases;
		delete m_renderer;

		m_texAtlases = nullptr;
		m_renderer   = nullptr;
	}


	////////////////////////////////////////////////////////////
	void Font::init()
	{
		// first clean up
        cleanUp();
		m_renderer = new FontRenderer;
		m_texAtlases = new std::map<unsigned int,TextureAtlas<Identifier> >;
	}


	////////////////////////////////////////////////////////////
    Metrics Font::getMetrics() const
    {
        if (!m_renderer)
            return Metrics();
        
        return m_renderer->getMetrics();
    }
    
	/////////////////////////////////////////////////////////////
	int Font::getKerning(const fm::Uint32 &leftCodePoint,const fm::Uint32 &rightCodePoint) const
	{
		if (!m_renderer)
			return 0;

		return m_renderer->getKerning(leftCodePoint,rightCodePoint);
	}

	////////////////////////////////////////////////////////////
	const Texture &Font::getTexture() const
	{
	    return (*m_texAtlases)[m_renderer->getCharacterSize()].getTexture();
	}
	
	/////////////////////////////////////////////////////////////
	Glyph Font::upload(const fg::Image &img,const fm::Uint32 &letter,unsigned int type,const fm::vec2i &leftdown,unsigned int characterSize)
	{
		return (*m_texAtlases)[characterSize ? characterSize : m_renderer->getCharacterSize()].upload(img,Identifier(letter,type),leftdown);
	}
	
	/////////////////////////////////////////////////////////////
	Font::reference Font::swap(Font &font)
	{
		std::swap(m_renderer  ,font.m_renderer  );
		std::swap(m_texAtlases,font.m_texAtlases);
		
		return *this;
	}
}
