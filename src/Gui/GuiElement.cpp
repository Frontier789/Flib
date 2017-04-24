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
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	GuiElement::GuiElement(GuiContext &owner) : m_context(&owner),
												m_layout(nullptr)
	{
		
	}

	/////////////////////////////////////////////////////////////
	GuiElement::GuiElement(GuiContext &owner,fm::vec2 size) : m_context(&owner),
															  m_layout(nullptr),
															  m_size(size)
	{
		
	}

	/////////////////////////////////////////////////////////////
	void GuiElement::onDraw(fg::ShaderManager &shader)
	{
		(void)shader;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiElement::onDraw()
	{
		onDraw(getOwnerContext().getShader());
	}

	/////////////////////////////////////////////////////////////
	void GuiElement::onUpdate(const fm::Time &dt)
	{
		(void)dt;
	}

	/////////////////////////////////////////////////////////////
	bool GuiElement::onEvent(fw::Event &ev)
	{
		(void)ev;
		
		return false;
	}

	/////////////////////////////////////////////////////////////
	bool GuiElement::handleEvent(fw::Event &ev)
	{
		bool handled = onEvent(ev);
		
		GuiCallback::forwardToHandlers(ev);
		
		return handled;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2s GuiElement::getSize() const
	{
		return m_size;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiElement::setSize(fm::vec2s size)
	{
		m_size = size;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2i GuiElement::getPosition() const
	{
		return m_pos;
	}

	/////////////////////////////////////////////////////////////
	void GuiElement::setPosition(fm::vec2i pos)
	{
		m_pos = pos;
	}

	/////////////////////////////////////////////////////////////
	void GuiElement::setOwnerContext(GuiContext &context)
	{
		m_context = &context;
	}

	/////////////////////////////////////////////////////////////
	GuiContext &GuiElement::getOwnerContext()
	{
		return *m_context;
	}

	/////////////////////////////////////////////////////////////
	const GuiContext &GuiElement::getOwnerContext() const
	{
		return *m_context;
	}

	/////////////////////////////////////////////////////////////
	bool GuiElement::contains(fm::vec2 p) const
	{
		return fm::rect2f(getPosition(),getSize()).contains(p);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiElement::setLayout(GuiLayout *layout)
	{
		m_layout = layout;
	}
	
	/////////////////////////////////////////////////////////////
	GuiLayout *GuiElement::getLayout() const
	{
		return m_layout;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiElement::setId(const fm::String &id)
	{
		m_id = id;
	}
		
	/////////////////////////////////////////////////////////////
	const fm::String &GuiElement::getId() const
	{
		return m_id;
	}
	
	/////////////////////////////////////////////////////////////
	GuiElement *GuiElement::findById(const fm::String &id)
	{
		if (m_id == id) return this;
		
		return nullptr;
	}
}
