////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Gui/GuiDrawable.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>
#include <FRONTIER/Gui/GuiLayout.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Sleep.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	namespace
	{
		using namespace fg;
		using namespace fm;
		
		Image getStdButtonImage(vec2 size,Color sarok,Color oldal,Color belsarok,Color beloldal,Color interpBeg,Color interpEnd)
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
			return getStdButtonImage(vec2(128,128),Color(0,0),Color(124),Color(180),Color(240),Color(193),Color(243));
		}

		Image getDefButtonImgHover()
		{
			return getStdButtonImage(vec2(128,128),Color(0,0),Color(124),Color(180),Color(240),Color(223),Color(248));
		}

		Image getDefButtonImgPress()
		{
			return getStdButtonImage(vec2(128,128),Color(0,0),Color(124),Color(180),Color(240),Color(180),Color(248));
		}
		/*
		template<class T>
		T mix(T a,T b,float r)
		{
			r = std::max<float>(std::min<float>(r,1),0);
			
			return a * (1 - r) + b * r;
		}
		
		Image getSqrBtn(vec2 size,vec4 upperCorner,
								  vec4 lowerCorner,
								  vec4 upperSide,
								  vec4 lowerSide,
								  vec4 upperRow,
								  vec4 lowerRow,
								  vec4 upperBody,
								  vec4 lowerBody)
		{
			Image img(size);
			
			img.forEach([&](vec2s p,Color &c,Image &img) {
				vec2s s = img.getSize();
				
				if (p.x < 1 || p.x >= s.w-1)
				{
						 if (p.y < 1)
						c = upperCorner;
					else if (p.y >= s.h-1)
						c = lowerCorner;
					else
						c = mix(upperSide,lowerSide,float(p.y-1)/(s.h-2));
				}
				else
				{
						 if (p.y < 1)
						c = upperRow;
					else if (p.y >= s.h-1)
						c = lowerRow;
					else
						c = mix(upperBody,lowerBody,float(p.y-1)/(s.h-2));
				}
			});
			
			return img;
		}
		
		Image getDefButtonImgNorm2()
		{
			return getSqrBtn(vec2(128,128),vec4(200,255),
										   vec4(183,255),
										   vec4(164,255),
										   vec4(148,255),
										   vec4(165,255),
										   vec4(147,255),
										   vec4(245,255),
										   vec4(222,255));
		}

		Image getDefButtonImgHover2()
		{
			return getSqrBtn(vec2(128,128),vec4(141,255),
										   vec4(126,255),
										   vec4(123,255),
										   vec4(111,255),
										   vec4( 88,255),
										   vec4( 75,255),
										   vec4(237,255),
										   vec4(213,255));
		}

		Image getDefButtonImgPress2()
		{
			return getSqrBtn(vec2(128,128),vec4(184,255),
										   vec4(200,255),
										   vec4(149,255),
										   vec4(165,255),
										   vec4(148,255),
										   vec4(165,255),
										   vec4(223,255),
										   vec4(247,255));
		}
*/
		void loadDefButtonImg(GuiContext &cont)
		{
			cont.addImage("Button_Bckg_Norm" ,getDefButtonImgNorm (),vec2(4,3));
			cont.addImage("Button_Bckg_Hover",getDefButtonImgHover(),vec2(4,3));
			cont.addImage("Button_Bckg_Press",getDefButtonImgPress(),vec2(4,3));
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::setupShader()
	{
		if (!m_shader) return;
		
		m_shader->getCamera().set2D(getSize());
	}

	/////////////////////////////////////////////////////////////
	void GuiContext::drawElementsGuiLoop()
	{
		drawElements();
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext() : 
		m_spriteManager(nullptr),
		m_shader(nullptr),
		m_defCharSize(13),
		m_layout(nullptr),
		m_guiLoop(false)
	{
		
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext(fm::vec2s size) : GuiContext()
	{
		initGL(size);
	}

	/////////////////////////////////////////////////////////////
	void GuiContext::initGL(fm::vec2s size) {
		freeGL();
		
		m_spriteManager = new fg::SpriteManager(true);
		m_shader = fg::ShaderManager::getDefaultShader();
		m_layout = new GuiLayout(*this);
		m_size = size;
		
		if (size.area())
			setupShader();
		
		setMaxFps(60);
		loadDefButtonImg(*this);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::freeGL()
	{
		delete m_spriteManager;
		delete m_shader;
		delete m_layout;
		
		m_fonts.clear();
		
		m_spriteManager = nullptr;
		m_shader = nullptr;
		m_layout = nullptr;
	}
	
	/////////////////////////////////////////////////////////////
	GuiContext::~GuiContext()
	{
		delete m_spriteManager;
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
	fm::Result GuiContext::loadFont(const fm::String &fontName) const
	{
		fg::Font font;
		fm::Result res = font.loadSysFont(fontName.str());
		
		if (res) res = loadFont(fontName,font);
		
		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::loadFont(const fm::String &fontName,fg::Font font) const
	{
		if (font)
			m_fonts[fontName] = font;
		else
			m_fonts.erase(fontName);
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font GuiContext::getFont(const fm::String &fontName) const
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
	void GuiContext::addElement(fgui::GuiElement *elem)
	{
		getMainLayout().addChildElement(elem);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::addDrawable(fg::Drawable &object)
	{
		addElement(new fgui::GuiDrawable(*this,object));
	}
	
	/////////////////////////////////////////////////////////////
	void GuiContext::handleEvent(fw::Event &ev)
	{
		(void)m_layout->handleEvent(ev);
		
		if (m_guiLoop)
		{
			if (ev.type == fw::Event::Closed) m_guiLoop = false;
			if (ev.type == fw::Event::KeyPressed)
				if (ev.key.code == fw::Keyboard::F4 && ev.key.alt) m_guiLoop = false;
		}
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
		this->draw(getSpriteManager());
		this->draw(getMainLayout());
		
		for (auto &it : m_fonts)
			it.second.onDrawSprites(getShader());
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
	void GuiContext::addImage(const std::string &id,const fg::Image &spriteImage,const fm::vec2s &frameSize)
	{
		m_spriteManager->addImage(spriteImage,id,frameSize);
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::addImage(const std::string &id,const std::string &imgFile,const fm::vec2s &frameSize)
	{
		Image img;
		auto res = img.loadFromFile(imgFile);
		if (!res) return res;
		
		m_spriteManager->addImage(img,id,frameSize);
		
		return {};
	}
	
	/////////////////////////////////////////////////////////////
	fg::Sprite GuiContext::getSprite(const std::string &id,fm::Ref<fm::Result> result)
	{
		if (!m_spriteManager->hasImage(id)) {
			fg::Image img;
			auto res = img.loadFromFile(id);
			
			if (result) *result = res;
			
			if (res) {
				addImage(id,img);
			}
		}
		
		return m_spriteManager->getSprite(id);
	}
	
	/////////////////////////////////////////////////////////////
	fg::SpriteManager &GuiContext::getSpriteManager()
	{
		return *m_spriteManager;
	}
	
	/////////////////////////////////////////////////////////////
	const fg::SpriteManager &GuiContext::getSpriteManager() const
	{
		return *m_spriteManager;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Time GuiContext::getUpdateInterval() const
	{
		return m_spf;
	}
	
	/////////////////////////////////////////////////////////////
	int GuiContext::runGuiLoop(fm::Delegate<void> user_update)
	{
		for (m_guiLoop = true;m_guiLoop;)
		{
			handlePendingEvents();

			updateElements();
			
			user_update();
			
			drawElementsGuiLoop();
			
			applyFpsLimit();
		}

		return 0;
	}

	/////////////////////////////////////////////////////////////
	void GuiContext::stopGuiLoop()
	{
		m_guiLoop = false;
	}

	/////////////////////////////////////////////////////////////
	void GuiContext::setDefCharSize(fm::Size defSize)
	{
		m_defCharSize = defSize;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GuiContext::getDefCharSize() const
	{
		return m_defCharSize;
	}
	
	/////////////////////////////////////////////////////////////
	GuiContext &GuiContext::swap(GuiContext &cont)
	{
		m_fonts.swap(cont.m_fonts);
		std::swap(m_spriteManager,cont.m_spriteManager);
		std::swap(m_shader,cont.m_shader);
		std::swap(m_defCharSize,cont.m_defCharSize);
		std::swap(m_layout,cont.m_layout);
		std::swap(m_fpsClk,cont.m_fpsClk);
		std::swap(m_size,cont.m_size);
		std::swap(m_spf,cont.m_spf);
		std::swap(m_guiLoop,cont.m_guiLoop);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	bool GuiContext::hasImage(const std::string &id) const
	{
		return getSpriteManager().hasImage(id);
	}
}

