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
#include <FRONTIER/Graphics/SpriteManager.hpp>
#include <FRONTIER/Graphics/FontRenderer.hpp>
#include <FRONTIER/Graphics/TextureAtlas.hpp>
#include <FRONTIER/Graphics/Sprite.hpp>
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
	TextureAtlas<Font::Identifier> &Font::getTexAtl(unsigned int charSize) const
	{
		if (!charSize)
			charSize = m_renderer->getCharacterSize();
		
		return (*m_spriteManagers)[charSize].getAtlas();
	}

	////////////////////////////////////////////////////////////
    Font::Font() : m_spriteManagers(nullptr),
                   m_renderer(nullptr),
                   m_refCount(nullptr)
    {

    }

	////////////////////////////////////////////////////////////
    Font::Font(Font::const_reference copy) : m_spriteManagers(nullptr),
											 m_renderer(nullptr),
											 m_refCount(nullptr)
    {
		(*this) = copy;
    }

	////////////////////////////////////////////////////////////
	Font::Font(Font &&move) : m_spriteManagers(nullptr),
							  m_renderer(nullptr),
							  m_refCount(nullptr)
    {
		move.swap(*this);
    }

	////////////////////////////////////////////////////////////
	Font &Font::operator=(Font::const_reference copy)
    {
		cleanUp();
		
		if (copy.isLoaded())
		{
			m_renderer       = copy.m_renderer;
			m_spriteManagers = copy.m_spriteManagers;
			m_refCount       = copy.m_refCount;
			
			(*m_refCount)++;
		}
		
		return *this;
    }

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
    fm::Result Font::loadFromFile(const std::string &fileName,unsigned int characterSize)
    {
    	init();
        return m_renderer->loadFromFile(fileName,characterSize);
    }

	////////////////////////////////////////////////////////////
    fm::Result Font::loadSysFont(const std::string &fileName,unsigned int characterSize)
    {
    	init();
        return m_renderer->loadSysFont(fileName,characterSize);
    }

	////////////////////////////////////////////////////////////
    fm::Result Font::loadDefSysFont(unsigned int characterSize)
    {
    	init();
        return m_renderer->loadDefSysFont(characterSize);
    }

	////////////////////////////////////////////////////////////
    fm::Result Font::loadFromMemory(const fm::Uint8 *fileContent,fm::Size fileLength,unsigned int characterSize)
    {
        init();
        return m_renderer->loadFromMemory(fileContent,fileLength,characterSize);
    }

	////////////////////////////////////////////////////////////
    fm::Result Font::copyFromMemory(const fm::Uint8 *fileContent,fm::Size fileLength,unsigned int size)
    {
        init();
        return m_renderer->copyFromMemory(fileContent,fileLength,size);
    }
    
	/////////////////////////////////////////////////////////////
	Font Font::createHardCopy() const
	{
		Font ret;
		
		if (isLoaded())
		{
			ret.m_spriteManagers = new std::map<unsigned int,SpriteManagerBase<Identifier> >;
			ret.m_renderer       = new FontRenderer();
			ret.m_refCount       = new fm::Size(1);
			
			*ret.m_renderer  = m_renderer->createHardCopy(); 
		}
		
		return ret;
	}

	////////////////////////////////////////////////////////////
    Font::reference Font::setSmooth(bool smooth)
    {
		if (isLoaded())
			for (auto it = m_spriteManagers->begin();it != m_spriteManagers->end();++it)
				it->second.getAtlas().getTexture().setSmooth(smooth);

		return *this;
    }


	////////////////////////////////////////////////////////////
    void Font::setCharacterSize(unsigned int characterSize) const
    {
        if (isLoaded())
            m_renderer->setCharacterSize(characterSize);
    }

	/////////////////////////////////////////////////////////////
	unsigned int Font::getCharacterSize() const
	{
		if (!isLoaded())
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
    	if (!isLoaded())
			return Glyph();

		// try finding it in the dictionary
		Glyph glyph = getTexAtl().fetch(Identifier(letter,style));
		
		if (glyph != Glyph())
			return glyph;

		// no match, create it
		fm::vec2 leftdown;
		fg::Image img = renderGlyph(letter,style,&leftdown);
		
		return getTexAtl().upload(img,Identifier(letter,style),leftdown);
    }
    
	/////////////////////////////////////////////////////////////
	void Font::preCache(const fm::String &characters,unsigned int type) const
	{
		if (!isLoaded())
			return;

		TextureAtlas<Identifier> &texAtl = getTexAtl();
		
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
        if (!isLoaded())
            return Image();

        return m_renderer->renderGlyph(letter,style,leftDown);
    }
    
	/////////////////////////////////////////////////////////////
	bool Font::hasGlyph(const fm::Uint32 &letter,unsigned int style) const
	{
		if (!isLoaded())
			return false;
			
		if (getTexAtl().isUploaded(Identifier(letter,style)))
			return true;
		
		return m_renderer->hasGlyph(letter);
	}

	////////////////////////////////////////////////////////////
	void Font::cleanUp()
	{
		if (m_refCount)
		{
			(*m_refCount)--;
			
			if (*m_refCount == 0)
			{
				delete m_spriteManagers;
				delete m_renderer;
				delete m_refCount;
			}
		}
		
		m_spriteManagers = nullptr;
		m_renderer       = nullptr;
		m_refCount       = nullptr;
	}


	////////////////////////////////////////////////////////////
	void Font::init()
	{
		// first clean up
        cleanUp();
		m_spriteManagers = new std::map<unsigned int,SpriteManagerBase<Identifier> >;
		m_renderer       = new FontRenderer;
		m_refCount       = new fm::Size(1);
	}


	////////////////////////////////////////////////////////////
    Metrics Font::getMetrics() const
    {
        if (!isLoaded())
            return Metrics();
        
        return m_renderer->getMetrics();
    }
    
	/////////////////////////////////////////////////////////////
	int Font::getKerning(const fm::Uint32 &leftCodePoint,const fm::Uint32 &rightCodePoint) const
	{
		if (!isLoaded())
			return 0;

		return m_renderer->getKerning(leftCodePoint,rightCodePoint);
	}

	////////////////////////////////////////////////////////////
	const Texture &Font::getTexture() const
	{
	    return getTexAtl().getTexture();
	}
	
	/////////////////////////////////////////////////////////////
	const SpriteManagerBase<Font::Identifier> &Font::getSpriteManager() const
	{
		return (*m_spriteManagers)[m_renderer->getCharacterSize()];
	}

	/////////////////////////////////////////////////////////////
	SpriteManagerBase<Font::Identifier> &Font::getSpriteManager()
	{
		return (*m_spriteManagers)[m_renderer->getCharacterSize()];
	}
	
	/////////////////////////////////////////////////////////////
	SpriteBase<Font::Identifier> Font::getSprite(fm::Uint32 letter,unsigned int style)
	{
		if (!getTexAtl().isUploaded(Identifier(letter,style)))
			getGlyph(letter,style);
		
		return getSpriteManager().getSprite(Font::Identifier(letter,style));
	}
	
	/////////////////////////////////////////////////////////////
	Glyph Font::upload(const fg::Image &img,const fm::Uint32 &letter,unsigned int type,const fm::vec2i &leftdown,unsigned int characterSize)
	{
		if (!isLoaded()) return Glyph();
		
		return getTexAtl(characterSize).upload(img,Identifier(letter,type),leftdown);
	}
	
	/////////////////////////////////////////////////////////////
	Font::reference Font::swap(Font &font)
	{
		std::swap(m_spriteManagers,font.m_spriteManagers);
		std::swap(m_renderer      ,font.m_renderer  );
		std::swap(m_refCount      ,font.m_refCount  );
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	bool Font::isLoaded() const
	{
		return m_renderer;
	}
	
	/////////////////////////////////////////////////////////////
	Font::operator bool() const
	{
		return isLoaded();
	}
	
	/////////////////////////////////////////////////////////////
	bool Font::operator!() const
	{
		return !isLoaded();
	}
}
