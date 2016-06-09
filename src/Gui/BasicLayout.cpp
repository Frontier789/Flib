#include <FRONTIER/Gui/BasicLayout.hpp>


////////////////////////////////////////////////////////////
BasicLayout::BasicLayout() : Layout(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr)
{

}

////////////////////////////////////////////////////////////
BasicLayout::BasicLayout(const fm::vec2 &pos,
                         const fm::String &id,
                         Layout *parent) : Layout(pos,fm::vec2(),id,parent)
{

}

////////////////////////////////////////////////////////////
BasicLayout::BasicLayout(const fm::String &id,
                         Layout *parent) : Layout(fm::vec2(),fm::vec2(),id,parent)
{

}

////////////////////////////////////////////////////////////
BasicLayout::~BasicLayout()
{

}

////////////////////////////////////////////////////////////
void BasicLayout::onContentChanged()
{
	fm::vec2 maxp;

	C(elementCount())
    {
        GuiElement *e = elementAt(i);
        if (e)
        {
            fm::vec2 s = e->getSize();
            fm::vec2 p = e->getPosInParent();

            if (p.x + s.w > maxp.x) maxp.x = p.x + s.w;
            if (p.y + s.h > maxp.y) maxp.y = p.y + s.h;
        }
    }

	if (getSize() != maxp)
		setSize(maxp);
}

////////////////////////////////////////////////////////////
fm::Size BasicLayout::elementCount() const
{
    return m_elements.size();
}

////////////////////////////////////////////////////////////
GuiElement *BasicLayout::elementAt(fm::Size index)
{
    if (index >= elementCount()) return fm::nullPtr;

    return m_elements[index];
}

////////////////////////////////////////////////////////////
void BasicLayout::insertElement(fm::Size index,GuiElement *newElement)
{
    if (m_elements.size() <= index)
    {
        m_elements.resize(index+1,fm::nullPtr);
    }
    else
    {
        m_elements.push_back(fm::nullPtr);
    }

    fm::Size i = m_elements.size()-1;
    while (i > index)
    {
        GuiElement *tmp = m_elements[i];
        m_elements[i]   = m_elements[i-1];
        m_elements[i-1] = tmp;

        --i;
    }

    m_elements[i] = newElement;
    newElement->setParent(this);

    if (newElement && newElement->getParent() != this)
        newElement->setParent(this);

    onContentChanged();
}

////////////////////////////////////////////////////////////
GuiElement *BasicLayout::remElement(fm::Size index)
{
    if (index < m_elements.size())
    {
        GuiElement *ret = m_elements[index];

        while (index+1 < m_elements.size())
        {
            m_elements[index] = m_elements[index+1];
        }

        m_elements.pop_back();

        onContentChanged();

        return ret;
    }

    onContentChanged();

    return fm::nullPtr;
}
