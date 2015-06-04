#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Gui/Widget.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string Widget::m_className = "Widget";
	
	/////////////////////////////////////////////////////////////
	std::deque<Widget *> Widget::allWidgets;

	/////////////////////////////////////////////////////////////
	Widget::Widget(const std::string &name,const Anchor &anchor,const fm::vec2 &size,Widget *parent) : m_name(name),
																									   m_anchor(anchor),
																									   m_size(size),
																									   m_parent(parent),
																									   m_enabled(true)
	{
		allWidgets.push_back(this);
	}
	
	/////////////////////////////////////////////////////////////
	Widget::Widget(const Widget &copy) : m_name(copy.m_name),
										 m_anchor(copy.m_anchor),
										 m_size(copy.m_size),
										 m_parent(copy.m_parent),
										 m_enabled(copy.m_enabled)
	{
		allWidgets.push_back(this);
	}

	/////////////////////////////////////////////////////////////
	Widget::~Widget()
	{
		for (std::deque<Widget*>::iterator it = allWidgets.begin();it!=allWidgets.end();++it)
			if (*it == this)
			{
				allWidgets.erase(it);
				return;
			}
	}

	/////////////////////////////////////////////////////////////
	void Widget::setAnchor(const Anchor &anchor)
	{
		m_anchor = anchor;
	}
	
	/////////////////////////////////////////////////////////////
	const Anchor &Widget::getAnchor() const
	{
		return m_anchor;
	}

	/////////////////////////////////////////////////////////////
	void Widget::setParent(Widget *parent)
	{
		m_parent = parent;
	}
	
	/////////////////////////////////////////////////////////////
	Widget *Widget::getParent() const
	{
		return m_parent;
	}

	/////////////////////////////////////////////////////////////
	void Widget::setSize(const fm::vec2 &size)
	{
		m_size = size;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 Widget::getSize() const
	{
		return m_size;
	}
	
	/////////////////////////////////////////////////////////////
	void Widget::setName(const std::string &name)
	{
		m_name = name;
	}

	/////////////////////////////////////////////////////////////
	std::string Widget::getName() const
	{
		return m_name;
	}
	
	/////////////////////////////////////////////////////////////
	void Widget::setEnabled(bool enabled)
	{
		m_enabled = enabled;
	}
	
	/////////////////////////////////////////////////////////////
	bool Widget::getEnabled() const
	{
		return m_enabled;
	}
	
	/////////////////////////////////////////////////////////////
	void Widget::setActive(Widget *active)
	{
		if (m_parent)
			m_parent->setActive(active);
	}
	
	////////////////////////////////////////////////////////
	Widget *Widget::getActive()
	{
		if (m_parent)
			return m_parent->getActive();
		
		return fm::nullPtr;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 Widget::getParentSize() const
	{
		if (m_parent)
			return m_parent->getSize();
		
		return fm::vec2();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 Widget::getRootSize() const
	{
		if (m_parent)
			return m_parent->getRootSize();
		
		return getSize();
	}

	/////////////////////////////////////////////////////////////
	fm::vec2 Widget::getPosInParent() const
	{
		return m_anchor.getPosInParent(*this);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 Widget::getPosInRoot() const
	{
		return m_anchor.getPosInRoot(*this);
	}
	
	/////////////////////////////////////////////////////////////
	Widget *Widget::getRoot()
	{
		return m_parent ? m_parent->getRoot() : this;
	}
	
	/////////////////////////////////////////////////////////////
	Widget *Widget::findNamed(const std::string &name)
	{
		if (getName() == name)
			return this;
		
		return fm::nullPtr;
	}

	/////////////////////////////////////////////////////////////
	void Widget::onDraw()
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void Widget::onUpdate()
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void Widget::onParentChange()
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	bool Widget::handleEvent(const fw::Event &ev)
	{
		(void)ev;
		
		return false;
	}

	/////////////////////////////////////////////////////////////
	const std::string &Widget::getRealType() const
	{
		return m_className;
	}
}
