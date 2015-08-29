#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Gui/Div.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string Div::m_className = "Div";

	/////////////////////////////////////////////////////////////
	Div::Div(const std::string &name,
			 const Anchor &anchor,
			 const fm::vec2 &size,
			 Widget *parent,
			 const fm::vec2 &margin,
			 Widget *content) : Widget(name,anchor,size,parent),
								m_margin(margin),
								m_content(content)
	{
		if (m_content)
			m_content->setParent(this);
	}

	/////////////////////////////////////////////////////////////
	Div::~Div()
	{
		delete m_content;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2 Div::getSize() const
	{
		fm::vec2 contentSize = (m_content ? m_content->getSize() : fm::vec2())+m_margin*2;

		return fm::vec2(fm::math::max(contentSize.w,m_size.w),fm::math::max(contentSize.h,m_size.h));
	}

	/////////////////////////////////////////////////////////////
	void Div::setMinSize(const fm::vec2 &size)
	{
		m_size = size;

		if (m_content)
			m_content->onParentChange();
	}

	/////////////////////////////////////////////////////////////
	void Div::setMargin(const fm::vec2 &margin)
	{
		m_margin = margin;

		if (m_content)
			m_content->onParentChange();
	}

	/////////////////////////////////////////////////////////////
	void Div::setParent(Widget *parent)
	{
		Widget::setParent(parent);

		if (m_content)
			m_content->onParentChange();
	}

	/////////////////////////////////////////////////////////////
	void Div::setSize(const fm::vec2 &size)
	{
		m_size = size;

		if (m_content)
			m_content->onParentChange();
	}

	/////////////////////////////////////////////////////////////
	void Div::setAnchor(const Anchor &anchor)
	{
		Widget::setAnchor(anchor);

		if (m_content)
			m_content->onParentChange();
	}

	/////////////////////////////////////////////////////////////
	Widget *Div::findNamed(const std::string &name)
	{
		if (m_name == name)
			return this;

		if (m_content)
			return m_content->findNamed(name);

		return fm::nullPtr;
	}

	/////////////////////////////////////////////////////////////
	void Div::onDraw()
	{
		if (m_content)
			m_content->onDraw();
	}

	/////////////////////////////////////////////////////////////
	void Div::onUpdate()
	{
		if (m_content)
			m_content->onUpdate();
	}

	/////////////////////////////////////////////////////////////
	void Div::onParentChange()
	{
		if (m_content)
			m_content->onParentChange();
	}

	/////////////////////////////////////////////////////////////
	bool Div::handleEvent(const fw::Event &ev)
	{
		if (!getEnabled())
			return false;

		if (m_content)
			return m_content->handleEvent(ev);

		return false;
	}

	/////////////////////////////////////////////////////////////
	Widget *Div::getContent()
	{
		return m_content;
	}

	/////////////////////////////////////////////////////////////
	void Div::setContent(Widget *content)
	{
		delete m_content;
		m_content = content;

		if (m_content)
			m_content->setParent(this);
	}

	/////////////////////////////////////////////////////////////
	const std::string &Div::getRealType() const
	{
		return m_className;
	}
}
