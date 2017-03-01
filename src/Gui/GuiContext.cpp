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
#include <FRONTIER/Graphics/Font.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	void GuiContext::setupShader()
	{
		if (!m_shader) return;
		
		m_shader->getCamera().set2D(getSize());
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext() : m_shader(new fg::FixedShaderManager)
	{
		setupShader();
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext(fm::vec2s size) : m_shader(new fg::FixedShaderManager),
											 m_size(size)
	{
		setupShader();
	}

	/////////////////////////////////////////////////////////////
	GuiContext::GuiContext(GuiContext &&cont) : m_shader(nullptr)
	{
		cont.swap(*this);
	}

	/////////////////////////////////////////////////////////////
	GuiContext::~GuiContext()
	{
		for (const auto &it : m_fonts)
			delete it.second;
		
		delete m_shader;
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
		fg::Font *font = new fg::Font;
		fm::Result res = font->loadSysFont(fontName.str());
		
		if (res)
		{
			res = loadFont(fontName,font);
		}
		
		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::loadFont(const fm::String &fontName,fg::Font *newFont)
	{
		m_fonts[fontName] = newFont;
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font *GuiContext::getFont(const fm::String &fontName)
	{
		if (m_fonts[fontName] == nullptr)
			loadFont(fontName);
		
		return m_fonts[fontName];
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font *GuiContext::getDefaultFont()
	{
		if (m_fonts["default"] == nullptr)
		{
			fg::Font *defFont = new fg::Font();
			if (defFont->loadDefSysFont())
			{
				loadFont("default",defFont);
			}
			else delete defFont;
		}
		
		return m_fonts["default"];
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result GuiContext::setDefaultFont(fg::Font *newFont)
	{
		delete m_fonts["default"];
		m_fonts["default"] = newFont;
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	GuiContext &GuiContext::swap(GuiContext &cont)
	{
		m_fonts.swap(cont.m_fonts);
		return *this;
	}

	/////////////////////////////////////////////////////////////
	GuiContext &GuiContext::operator=(GuiContext &&cont)
	{
		return this->swap(cont);
	}
}

