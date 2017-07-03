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
#include <FRONTIER/Graphics/FixedShaderManager.hpp>
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>
#include <FRONTIER/Gui/GuiLayout.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Sleep.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	namespace priv
	{
		using namespace fg;
		using namespace fm;
		Image getUglyButton(vec2 size,Color sarok,Color oldal,Color belsarok,Color beloldal,Color interpBeg,Color interpEnd)
		{
			Image img(size);
			
			img.forEach([&](vec2s p,Color &c,Image &img) {
				vec2s s = img.getSize();
				
				if (p.x == 0 || p.y == 0 || p.x+1 == s.w || p.y+1 == s.h)
				{
					if ((p.x == 0) + (p.y == 0) + (p.x+1 == s.w) + (p.y+1 == s.h) >= 2)
					{
						c = sarok;
					}
					else 
						c = oldal;
				}
				else if (p.x == 1 || p.y == 1 || p.x+2 == s.w || p.y+2 == s.h)
				{
					if ((p.x == 1) + (p.y == 1) + (p.x+2 == s.w) + (p.y+2 == s.h) >= 2)
					{
						c = belsarok;
					}
					else 
						c = beloldal;
				}
				else
				{
					float yval = float(p.y) / s.h;
					yval = std::sqrt(1 - yval);
					
					c = interpEnd * yval + interpBeg * (1 - yval);				
				}
			});
			
			return img;
		}

		Image getDefButtonImgNorm()
		{
			return getUglyButton(vec2(128,128),Color(0,0,0,0),Color(124,124,124),Color(32,32,32),Color(240,240,240),Color(193,193,193),Color(243,243,243));
		}

		Image getDefButtonImgHover()
		{
			return getUglyButton(vec2(128,128),Color(0,0,0,0),Color(124,124,124),Color(32,32,32),Color(240,240,240),Color(223,223,223),Color(248,248,248));
		}

		Image getDefButtonImgPress()
		{
			return getUglyButton(vec2(128,128),Color(0,0,0,0),Color(124,124,124),Color(32,32,32),Color(240,240,240),Color(180,180,180),Color(248,248,248));
		}

		void loadDefButtonImg(GuiContext &cont)
		{
			cont.setSprite("Button_Bckg_Norm" ,getDefButtonImgNorm (),vec2(4,4));
			cont.setSprite("Button_Bckg_Hover",getDefButtonImgHover(),vec2(4,4));
			cont.setSprite("Button_Bckg_Press",getDefButtonImgPress(),vec2(4,4));
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::setupShader()
	{
		if (!m_shader) return;
		
		m_shader->getCamera().set2D(getSize());
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext() : GuiContext(fm::vec2s())
	{
		
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext(fm::vec2s size) : m_shader(fg::ShaderManager::getDefaultShader()),
											 m_layout(new GuiLayout(*this)),
											 m_size(size)
	{
		setupShader();
		setMaxFps(60);
		priv::loadDefButtonImg(*this);
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext(GuiContext &&cont) : m_shader(nullptr)
	{
		cont.swap(*this);
	}

	/////////////////////////////////////////////////////////////
	GuiContext::~GuiContext()
	{
		delete m_shader;
		delete m_layout;
	}

	/////////////////////////////////////////////////////////////
	GuiContext &GuiContext::setShader(fg::ShaderManager *newShader,bool delPrevShader)
	{
		if (delPrevShader)
			delete m_shader;
		
		m_shader = newShader;
		setupShader();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	fg::ShaderManager &GuiContext::getShader()
	{
		if (!m_shader)
		{
			m_shader = new fg::ShaderManager;
			setupShader();
		}
		
		return *m_shader;
	}

	/////////////////////////////////////////////////////////////
	const fg::ShaderManager &GuiContext::getShader() const
	{
		return ((GuiContext*)this)->getShader();
	}

	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::draw(const fg::DrawData &data)
	{
		return getShader().draw(data);
	}

	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::draw(const fg::DrawData &data,fm::Size indexSet)
	{
		return getShader().draw(data,indexSet);
	}

	/////////////////////////////////////////////////////////////
	GuiContext &GuiContext::draw(fg::Drawable &drawable)
	{
		drawable.onDraw(getShader());
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2s GuiContext::getSize() const
	{
		return m_size;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::setSize(fm::vec2s size)
	{
		m_size = size;
		
		setupShader();
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::loadFont(const fm::String &fontName)
	{
		fg::Font font;
		fm::Result res = font.loadSysFont(fontName.str());
		
		if (res) res = loadFont(fontName,font);
		
		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::loadFont(const fm::String &fontName,fg::Font font)
	{
		if (font)
			m_fonts[fontName] = font;
		else
			m_fonts.erase(fontName);
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font GuiContext::getFont(const fm::String &fontName)
	{
		if (!m_fonts.count(fontName))
			loadFont(fontName);
			
		if (m_fonts.count(fontName))
			return m_fonts[fontName];
		
		return fg::Font();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font GuiContext::getDefaultFont()
	{
		if (!m_fonts.count("default"))
		{
			fg::Font defFont;
			if (defFont.loadDefSysFont())
			{
				loadFont("default",defFont);
				return defFont;
			}
		}
		
		return m_fonts["default"];
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::setDefaultFont(fg::Font font)
	{
		return loadFont("default",font);
	}
	
	/////////////////////////////////////////////////////////////
	GuiContext &GuiContext::swap(GuiContext &cont)
	{
		m_fonts.swap(cont.m_fonts);
		std::swap(m_shader,cont.m_shader);
		std::swap(m_layout,cont.m_layout);
		
		m_layout->setOwnerContext(*this);
		m_layout->setSize(getSize());
		cont.m_layout->setOwnerContext(cont);
		cont.m_layout->setSize(cont.getSize());
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	GuiContext &GuiContext::operator=(GuiContext &&cont)
	{
		return this->swap(cont);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::setMainLayout(GuiLayout *layout,bool delPrevLayout)
	{
		if (delPrevLayout)
			delete m_layout;
		
		m_layout = layout;
		
		if (!m_layout) m_layout = new GuiLayout(*this);
		
		m_layout->setOwnerContext(*this);
		m_layout->setSize(getSize());
	}
	
	/////////////////////////////////////////////////////////////
	GuiLayout &GuiContext::getMainLayout()
	{
		if (!m_layout->getSize().area())
		{
			m_layout->setSize(getSize());
		}
		
		return *m_layout;
	}
		
	/////////////////////////////////////////////////////////////
	void GuiContext::handleEvent(fw::Event &ev)
	{
		m_layout->handleEvent(ev);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::handlePendingEvents()
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::updateElements()
	{
		m_layout->onUpdate(m_spf);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::drawElements()
	{
		this->draw(getMainLayout());
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::applyFpsLimit()
	{
		fm::Time dt = m_fpsClk.getTime();
		
		fm::Sleep(m_spf - dt);
		
		m_fpsClk.restart();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::setUpdateInterval(fm::Time interval)
	{
		m_spf = interval;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::setMaxFps(float fps)
	{
		if (fps) m_spf = fm::seconds(1.0 / fps);
		
		else m_spf = fm::Time::Zero;
	}
	
	/////////////////////////////////////////////////////////////
	fg::FramedSprite GuiContext::setSprite(const fm::String &name,const fg::Image &spriteImage,const fm::vec2s &frameSize)
	{
		fg::Glyph g = m_texAtlas.upload(spriteImage,name,frameSize);
		
		return fg::FramedSprite(m_texAtlas.getTexture(),fm::rect2s(g.pos,g.size),frameSize);
	}
	
	/////////////////////////////////////////////////////////////
	fg::FramedSprite GuiContext::getSprite(const fm::String &name) const
	{
		fg::Glyph g = m_texAtlas.fetch(name);
		
		if (!g.size.area())
			return fg::FramedSprite();
			
		return fg::FramedSprite(m_texAtlas.getTexture(),fm::rect2s(g.pos,g.size),g.leftdown);
	}
	
	/////////////////////////////////////////////////////////////
	fm::Time GuiContext::getUpdateInterval() const
	{
		return m_spf;
	}
}

