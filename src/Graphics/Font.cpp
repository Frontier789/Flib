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
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fg
{
	/// FontImpl::GlyphMap::Identifier /////////////////////////////////////////////////////////
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

	/// constructors /////////////////////////////////////////////////////////
    Font::Font() : m_refCount(fm::nullPtr),
                   m_renderer(fm::nullPtr),
                   m_texAtlases(fm::nullPtr)
    {

    }


	////////////////////////////////////////////////////////////
    Font::Font(Font::const_reference copy) : m_refCount(copy.m_refCount),
                                             m_renderer(copy.m_renderer),
                                             m_texAtlases(copy.m_texAtlases)
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
    fm::Result Font::loadFromFile(const std::string &fileName,unsigned int size)
    {
    	init();
        return m_renderer->loadFromFile(fileName,size);
    }


	////////////////////////////////////////////////////////////
    fm::Result Font::loadFromMemory(const void *fileContent,fm::Size fileLength,unsigned int size)
    {
        init();
        return m_renderer->loadFromMemory(fileContent,fileLength,size);
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

		// try finding it in the dictionary
		TextureAtlas<Identifier> &texAtl = (*m_texAtlases)[m_renderer->getCharacterSize()];
		
		C(characters.size())
		{
			fm::Uint32 c = characters[i];
			Glyph g = texAtl.fetch(Identifier(c,type));
			
			if (g == Glyph())
			{
				fm::vec2 leftdown;
				fg::Image img = renderGlyph(c,type,&leftdown);
				
				texAtl.upload(img,Identifier(c,type),leftdown);
			}
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
		// if the class was created successfully
        if (m_refCount)
        {
			// decrease shared count
            (*m_refCount)--;
			if (!(*m_refCount)) // if no more shares left free up resources
            {
				if (m_texAtlases)
					delete m_texAtlases;
                if (m_renderer)
                    delete m_renderer;
                if (m_refCount)
                    delete m_refCount;
            }
        }

		m_texAtlases = fm::nullPtr;
		m_renderer   = fm::nullPtr;
		m_refCount   = fm::nullPtr;
	}


	////////////////////////////////////////////////////////////
	void Font::init()
	{
		// first clean up
        cleanUp();
		m_renderer = new FontRenderer;
		m_refCount = new fm::Size;
		m_texAtlases = new std::map<unsigned int,TextureAtlas<Identifier> >;
        (*m_refCount) = 1;
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
	Glyph Font::upload(const fg::Image &img,const fm::Uint32 &letter,unsigned int type,const fm::vec2s &leftdown,unsigned int characterSize)
	{
		return (*m_texAtlases)[characterSize ? characterSize : m_renderer->getCharacterSize()].upload(img,Identifier(letter,type),leftdown);
	}
	
	/////////////////////////////////////////////////////////////
	Font::reference Font::swap(Font &font)
	{
		fm::swap(m_refCount  ,font.m_refCount  );
		fm::swap(m_renderer  ,font.m_renderer  );
		fm::swap(m_texAtlases,font.m_texAtlases);
		
		return *this;
	}
}
