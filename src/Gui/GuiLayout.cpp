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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Gui/GuiLayout.hpp>
#include <FRONTIER/System/util/C.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	GuiLayout::GuiLayout(GuiContext &owner) : GuiElement(owner),
											  m_activeElement(nullptr)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiLayout::~GuiLayout()
	{
		C(getChildCount())
			delete getChildElement(i);
	}

	/////////////////////////////////////////////////////////////
	void GuiLayout::forEach(fm::Delegate<void,GuiElement*,fm::Size,GuiLayout&> func)
	{
		C(getChildCount())
			func(getChildElement(i),i,*this);
	}

	/////////////////////////////////////////////////////////////
	void GuiLayout::forEach(fm::Delegate<void,GuiElement*,fm::Size,const GuiLayout&> func) const
	{
		C(getChildCount())
			func(getChildElement(i),i,*this);
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GuiLayout::removeChild(fm::Size index,bool del)
	{
		GuiElement *element = setChildElement(index, nullptr);
		
		if (del)
		{
			delete element;
			return nullptr;
		}
		
		return element;
	}

	/////////////////////////////////////////////////////////////
	void GuiLayout::clearChildElements(bool del)
	{
		if (!del)
			C(getChildCount())
				setChildElement(i,nullptr);

		setChildCount(0);
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GuiLayout::operator[](fm::Size index) const
	{
		if (index >= getChildCount()) return nullptr;
		
		return getChildElement(index);
	}

	/////////////////////////////////////////////////////////////
	void GuiLayout::onChildSize(GuiElement *)
	{
		
	}

	/////////////////////////////////////////////////////////////
	void GuiLayout::onDraw(fg::ShaderManager &shader)
	{
		C(getChildCount())
		{
			GuiElement *element = getChildElement(i);
			if (element)
				element->onDraw(shader);
		}
		
		GuiElement::onDraw(shader);
	}

	/////////////////////////////////////////////////////////////
	void GuiLayout::onUpdate(const fm::Time &dt)
	{
		C(getChildCount())
		{
			GuiElement *element = getChildElement(i);
			if (element)
				element->onUpdate(dt);
		}
	}

	/////////////////////////////////////////////////////////////
	bool GuiLayout::onEvent(fw::Event &ev)
	{
		if (m_activeElement)
		{
			if (m_activeElement->handleEvent(ev))
				return true;
		}
		
		fm::Size childCount = getChildCount();
		C(childCount)
		{
			GuiElement *element = getChildElement(childCount - 1 - i);
			if (element && element != m_activeElement)
				if (element->handleEvent(ev))
					return true;
		}
		
		return false;
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GuiLayout::findById(const fm::String &id)
	{
		if (getId() == id)
			return this;
		
		C(getChildCount())
		{
			GuiElement *elem = getChildElement(i);
			if (elem)
				elem = elem->findById(id);
			
			if (elem)
				return elem;
		}
		
		return nullptr;
	}
	
	/////////////////////////////////////////////////////////////
	GuiElement *GuiLayout::getChildElement(fm::Size index) const
	{
		if (getChildCount() <= index)
			return nullptr;
		
		return m_elements[index];
	}
	
	/////////////////////////////////////////////////////////////
	GuiElement *GuiLayout::setChildElement(fm::Size index,GuiElement *element)
	{
		if (getChildCount() <= index) setChildCount(index+1);
		
		std::swap(m_elements[index],element);
		
		if (m_elements[index])
			m_elements[index]->setLayout(this);
		
		if (element)
			element->setLayout(nullptr);
		
		return element;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GuiLayout::addChildElement(GuiElement *element)
	{
		fm::Size index = getChildCount(); 
		
		setChildElement(index,element);
		
		return index+1;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GuiLayout::getChildCount() const
	{
		return m_elements.size();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiLayout::setChildCount(fm::Size childCount)
	{
		for (fm::Size i = childCount;i < getChildCount();++i)
			delete getChildElement(i);
		
		m_elements.resize(childCount,nullptr);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiLayout::setOwnerContext(GuiContext &context)
	{
		GuiElement::setOwnerContext(context);
		
		C(getChildCount())
		{
			GuiElement *element = getChildElement(i);
			if (element)
				element->setOwnerContext(context);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiLayout::traverseHierarchy(fm::Delegate<void,GuiElement &> func)
	{
		GuiElement::traverseHierarchy(func);
		
		C(getChildCount())
		{
			GuiElement *element = getChildElement(i);
			if (element)
				element->traverseHierarchy(func);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiLayout::setActiveElement(GuiElement *element)
	{
		m_activeElement = element;
		
		// forward upwards
		if (m_activeElement)
			GuiElement::setActive(true);
	}
	 
	/////////////////////////////////////////////////////////////
	GuiElement *GuiLayout::getActiveElement() const
	{
		return m_activeElement;
	}
}
