////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)		   ///
///																	///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			 ///
/// condition that this disclaimer is not modified/removed.			///
/// You may not misclaim the origin of this software.				  ///
///																	///
/// If you use this software in your program/project a				 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							///
///																	///
/// You should have received a copy of GNU GPL with this software	  ///
///																	///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/SpriteManager.hpp>
#include <FRONTIER/Graphics/FontRenderer.hpp>
#include <FRONTIER/Graphics/TextureAtlas.hpp>
#include <FRONTIER/Graphics/ShaderSource.hpp>
#include <FRONTIER/Graphics/Sprite.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	Font::Identifier::Identifier() : 
		codePoint(0),
		style(Glyph::Regular)
	{

	}

	////////////////////////////////////////////////////////////
	Font::Identifier::Identifier(const fm::Uint32 &codePoint,Glyph::Style style) : codePoint(codePoint),
																				   style	(style)
	{

	}

	////////////////////////////////////////////////////////////
	bool Font::Identifier::operator<(const Identifier &other) const
	{
		// used for sorting
		return (codePoint==other.codePoint ? style < other.style : codePoint < other.codePoint);
	}

	////////////////////////////////////////////////////////////
	bool Font::Identifier::operator==(const Font::Identifier &other) const
	{
		return style == other.style && codePoint == other.codePoint;
	}
	
	////////////////////////////////////////////////////////////
	class Font::SharedData
	{
	public:
		SpriteManagerBase<Identifier> sigDistSpriteManager;
		mutable std::map<unsigned int,SpriteManagerBase<Identifier> > spriteManagers;
		FontRenderer renderer;
		fm::Size refCount;
		bool loaded;
		
		////////////////////////////////////////////////////////////
		SharedData() : refCount(1),
					   loaded(false)
		{
			
		}
		
		////////////////////////////////////////////////////////////
		void incRef() { ++refCount; }
		
		////////////////////////////////////////////////////////////
		bool decRef() { return --refCount; }
	};
		
	////////////////////////////////////////////////////////////
	TextureAtlas<Font::Identifier> &Font::getTexAtl(unsigned int charSize,Glyph::Style style) const
	{
		if (style & Glyph::SigDistField)
			return m_sharedData->sigDistSpriteManager.getAtlas();
		
		if (!charSize)
			charSize = getCharacterSize();
		
		return m_sharedData->spriteManagers[charSize].getAtlas();
	}
	
	////////////////////////////////////////////////////////////
	Font::Font() : m_sharedData(new SharedData)
	{

	}

	////////////////////////////////////////////////////////////
	Font::Font(Font::const_reference copy) : m_sharedData(new SharedData)
	{
		(*this) = copy;
	}

	////////////////////////////////////////////////////////////
	Font::Font(Font &&move) : m_sharedData(new SharedData)
	{
		move.swap(*this);
	}

	////////////////////////////////////////////////////////////
	Font &Font::operator=(Font::const_reference copy)
	{
		cleanUp();
		
		m_sharedData = copy.m_sharedData;
		m_sharedData->incRef();
		
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
		fm::Result res = m_sharedData->renderer.loadFromFile(fileName,characterSize);
		m_sharedData->loaded = res;
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::Result Font::loadSysFont(const std::string &fileName,unsigned int characterSize)
	{
		init();
		fm::Result res = m_sharedData->renderer.loadSysFont(fileName,characterSize);
		m_sharedData->loaded = res;
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::Result Font::loadDefSysFont(unsigned int characterSize)
	{
		init();
		fm::Result res = m_sharedData->renderer.loadDefSysFont(characterSize);
		m_sharedData->loaded = res;
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::Result Font::loadFromMemory(const fm::Uint8 *fileContent,fm::Size fileLength,unsigned int characterSize)
	{
		init();
		fm::Result res = m_sharedData->renderer.loadFromMemory(fileContent,fileLength,characterSize);
		m_sharedData->loaded = res;
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::Result Font::copyFromMemory(const fm::Uint8 *fileContent,fm::Size fileLength,unsigned int size)
	{
		init();
		fm::Result res = m_sharedData->renderer.copyFromMemory(fileContent,fileLength,size);
		m_sharedData->loaded = res;
		return res;
	}
	
	/////////////////////////////////////////////////////////////
	void Font::loadSigDistFieldShader(fm::Delegate<void,fg::ShaderSource &> sourceTransformer)
	{
		// setup shader
		fm::Result res = m_sharedData->sigDistSpriteManager.loadShader([&](ShaderSource &source){
			
			if (source.type == fg::FragmentShader)
			{
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_thickness"});
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_pov"});
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_dscale"});
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_coef"});
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_powAlpha"});
				source.globals += R"(
float contour(in float d,in float w) 
{
	return smoothstep(0.5 - w, 0.5 + w, d * u_thickness * 2.0);
}

float samp(in vec2 uv,in float w)
{
	return contour(texture2D(u_tex, uv).a, w);
})";
				source.mainBody = R"(
	vec4 c = texture2D(u_tex,va_texp);
	
    float dist = c.a;

	float width = pow(fwidth(dist),0.2 / u_pov);
	
    float alpha = contour( dist, width );

    vec2 duv = vec2(u_dscale) * (dFdx(va_texp) + dFdy(va_texp));
    vec4 box = vec4(va_texp-duv, va_texp+duv);

    float asum = samp( box.xy, width )
               + samp( box.zw, width )
               + samp( box.xw, width )
               + samp( box.zy, width );

	alpha = (6 * u_coef * alpha + 0.4 * asum) / (6 * u_coef + 0.4*4.0);
	
	alpha = clamp(alpha,0,1);
	
	out_color = vec4(va_clr.xyz, va_clr.w * pow(alpha,u_powAlpha));)";
			}
			
			sourceTransformer(source);
		});
		
		m_sharedData->sigDistSpriteManager.getShader().setUniform("u_thickness",.8011111f); // .5222222f
		m_sharedData->sigDistSpriteManager.getShader().setUniform("u_pov",      .5777778f); // .255556f 
		m_sharedData->sigDistSpriteManager.getShader().setUniform("u_dscale",   .1555556f); // .2666667f
		m_sharedData->sigDistSpriteManager.getShader().setUniform("u_coef",     .0333333f); // .1333333f
		m_sharedData->sigDistSpriteManager.getShader().setUniform("u_powAlpha", 2.4f); // .1333333f
		
		(void)res;
	}
	
	/////////////////////////////////////////////////////////////
	Font Font::createHardCopy() const
	{
		Font ret;
		
		ret.loadSigDistFieldShader();
		ret.m_sharedData->renderer = m_sharedData->renderer.createHardCopy();
		ret.m_sharedData->loaded   = m_sharedData->loaded;
		
		return ret;
	}

	////////////////////////////////////////////////////////////
	Font::reference Font::setSmooth(bool smooth)
	{
		for (auto &it : m_sharedData->spriteManagers)
			it.second.getAtlas().getTexture().setSmooth(smooth);

		return *this;
	}


	////////////////////////////////////////////////////////////
	void Font::setCharacterSize(unsigned int characterSize) const
	{
		m_sharedData->renderer.setCharacterSize(characterSize);
	}

	/////////////////////////////////////////////////////////////
	unsigned int Font::getCharacterSize() const
	{
		return m_sharedData->renderer.getCharacterSize();
	}

	/////////////////////////////////////////////////////////////
	FontRenderer &Font::getRenderer()
	{
		return m_sharedData->renderer;
	}

	////////////////////////////////////////////////////////////
	Glyph Font::getGlyph(const fm::Uint32 &letter,Glyph::Style style) const
	{
		if (!isLoaded())
			return Glyph();
		
		auto &texAtlas = getTexAtl(getCharacterSize(),style);
		
		// try finding it in the dictionary
		Glyph glyph = texAtlas.fetch(Identifier(letter,style));
		
		if (glyph != Glyph())
			return glyph;

		// no match, create it
		fm::vec2 leftdown;
		fg::Image img = renderGlyph(letter,style,&leftdown);
		
		return texAtlas.upload(img,Identifier(letter,style),leftdown);
	}
	
	/////////////////////////////////////////////////////////////
	void Font::preCache(const fm::String &characters,Glyph::Style style) const
	{
		if (!isLoaded())
			return;

		auto &texAtlas = getTexAtl();
		
		std::vector<TextureAtlas<Identifier>::MapPoint> mapPts;
		
		C(characters.size())
		{
			fm::Uint32 c = characters[i];
			Glyph g = texAtlas.fetch(Identifier(c,style));
			
			if (g == Glyph())
			{
				fm::vec2 leftdown;
				fg::Image *img = new fg::Image(renderGlyph(c,style,&leftdown));
				
				mapPts.push_back(TextureAtlas<Identifier>::MapPoint(img,Identifier(c,style),leftdown));
			}
		}
		
		texAtlas.upload(&mapPts[0],mapPts.size());
		
		C(mapPts.size())
		{
			delete mapPts[i].img;
		}
	}


	////////////////////////////////////////////////////////////
	Image Font::renderGlyph(const fm::Uint32 &letter,Glyph::Style style,fm::vec2 *leftDown) const
	{
		if (style & Glyph::SigDistField)
		{
			auto charSize = getCharacterSize();
			
			setCharacterSize(200);
			Image renderImg = m_sharedData->renderer.renderGlyph(letter,style,leftDown);
			Image sdfImg = renderImg.convertToSDF(10,[](Color c){return c.a > 127;},0);
			Image scaledSDFImg = sdfImg.scale(sdfImg.getSize()*.3);
			
			setCharacterSize(charSize);
			
			return scaledSDFImg;
		}
		else
		{
			return m_sharedData->renderer.renderGlyph(letter,style,leftDown);
		}
	}
	
	/////////////////////////////////////////////////////////////
	fm::rect2i Font::getGlyphRect(const fm::Uint32 &letter,Glyph::Style style) const
	{
		return m_sharedData->renderer.getGlyphRect(letter,style);
	}
	
	
	/////////////////////////////////////////////////////////////
	bool Font::hasGlyph(const fm::Uint32 &letter,Glyph::Style style) const
	{
		if (!isLoaded())
			return false;
			
		if (getTexAtl(getCharacterSize(),style).isUploaded(Identifier(letter,style)))
			return true;
		
		return m_sharedData->renderer.hasGlyph(letter);
	}

	////////////////////////////////////////////////////////////
	void Font::cleanUp()
	{
		if (!m_sharedData->decRef())
			delete m_sharedData;
		
		m_sharedData = nullptr;
	}


	////////////////////////////////////////////////////////////
	void Font::init()
	{
		cleanUp();
		m_sharedData = new SharedData;
		loadSigDistFieldShader();
	}


	////////////////////////////////////////////////////////////
	Metrics Font::getMetrics() const
	{
		return m_sharedData->renderer.getMetrics();
	}
	
	/////////////////////////////////////////////////////////////
	int Font::getKerning(const fm::Uint32 &leftCodePoint,const fm::Uint32 &rightCodePoint) const
	{
		return m_sharedData->renderer.getKerning(leftCodePoint,rightCodePoint);
	}

	////////////////////////////////////////////////////////////
	const Texture &Font::getTexture(Glyph::Style style) const
	{
		return getTexAtl(getCharacterSize(),style).getTexture();
	}
	
	/////////////////////////////////////////////////////////////
	const SpriteManagerBase<Font::Identifier> &Font::getSpriteManager(Glyph::Style style) const
	{
		if (style & Glyph::SigDistField)
			return m_sharedData->sigDistSpriteManager;
		
		return m_sharedData->spriteManagers[getCharacterSize()];
	}

	/////////////////////////////////////////////////////////////
	Font::reference Font::forEachSpriteManager(fm::Delegate<void,SpriteManagerBase<Identifier> &,unsigned int,Glyph::Style> func)
	{
		if (m_sharedData->loaded)
		{
			func(m_sharedData->sigDistSpriteManager,0,Glyph::SigDistField);
			
			for (auto &it : m_sharedData->spriteManagers)
				func(it.second,it.first,Glyph::Regular);
		}
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	SpriteManagerBase<Font::Identifier> &Font::getSpriteManager(Glyph::Style style)
	{
		if (style & Glyph::SigDistField)
			return m_sharedData->sigDistSpriteManager;
		
		return m_sharedData->spriteManagers[getCharacterSize()];
	}
	
	/////////////////////////////////////////////////////////////
	Font::Sprite Font::getSprite(fm::Uint32 letter,Glyph::Style style)
	{
		getGlyph(letter,style);
		
		Font::Sprite sprite = getSpriteManager(style).getSprite(Font::Identifier(letter,style));
		
		if (style & Glyph::SigDistField)
		{
			fm::vec2 size = m_sharedData->renderer.getGlyphRect(letter,style).size;
			
			sprite.setSize(size);
		}
		
		return sprite;
	}
	
	/////////////////////////////////////////////////////////////
	std::vector<Font::Sprite> Font::getSprites(fm::String letters,Glyph::Style style)
	{
		for (auto c : letters)
			getGlyph(c,style);
		
		std::vector<Font::Sprite> sprites = getSpriteManager(style).getSprites(Font::Identifier(letters[0],style),letters.size());
		
		C(letters.size())
		{
			fm::vec2 size = m_sharedData->renderer.getGlyphRect(letters[i],style).size;
			
			sprites[i].setImageID(Font::Identifier(letters[i],style));
			sprites[i].setSize(size);
		}
		
		return sprites;
	}
	
	/////////////////////////////////////////////////////////////
	Glyph Font::upload(const fg::Image &img,const fm::Uint32 &letter,Glyph::Style style,const fm::vec2i &leftdown,unsigned int characterSize)
	{
		return getTexAtl(characterSize,style).upload(img,Identifier(letter,style),leftdown);
	}
	
	/////////////////////////////////////////////////////////////
	Font::reference Font::swap(Font &font)
	{
		std::swap(m_sharedData,font.m_sharedData);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	bool Font::isLoaded() const
	{
		return m_sharedData->loaded;
	}
	
	/////////////////////////////////////////////////////////////
	Font::operator bool() const
	{
		return isLoaded();
	}

	/////////////////////////////////////////////////////////////
	bool Font::operator==(const Font &font) const
	{
		return m_sharedData == font.m_sharedData;
	}
	
	/////////////////////////////////////////////////////////////
	bool Font::operator!() const
	{
		return !isLoaded();
	}
	
	/////////////////////////////////////////////////////////////
	void Font::onDrawSprites(ShaderManager &shader)
	{
		m_sharedData->sigDistSpriteManager.onDraw(shader);
		
		for (auto &it : m_sharedData->spriteManagers)
			it.second.onDraw(shader);
	}
}
