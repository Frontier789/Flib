#include <FRONTIER/Gui/Layout.hpp>

namespace fgui
{
    ////////////////////////////////////////////////////////////
    Layout::Layout() : GuiElement(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr),
                       m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::Layout(const fm::vec2 &pos,
                   const fm::vec2 &size,
                   const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(pos,size,id,parent,enabled),
                   m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::Layout(const fm::vec2 &size,
                   const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(fm::vec2(),size,id,parent,enabled),
                   m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::Layout(const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(fm::vec2(),fm::vec2(),id,parent,enabled),
                   m_active(fm::nullPtr)
    {

    }

    ////////////////////////////////////////////////////////////
    Layout::~Layout()
    {

    }

    ////////////////////////////////////////////////////////////
    bool Layout::handleEvent(const fw::Event &ev)
    {
        if (!isEnabled())
            return false;

        if (GuiElement::handleEvent(ev))
            return true;

        if (m_active && m_active->handleEvent(ev))
            return true;

        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e && e != m_active)
                if (e->handleEvent(ev))
                    return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::elementAt(fm::Size index) const
    {
       return ((Layout*)this)->elementAt(index);
    }

    ////////////////////////////////////////////////////////////
    fm::Size Layout::findElement(const GuiElement *element) const
    {
        C(elementCount())
            if (elementAt(i) == element)
                return i;

        return fm::Size(-1);
    }

    ////////////////////////////////////////////////////////////
    void Layout::addElement(GuiElement *newElement)
    {
        insertElement(elementCount(),newElement);
    }

    ////////////////////////////////////////////////////////////
    void Layout::swapElement(fm::Size index0,fm::Size index1)
    {
        if (index0 > index1)
        {
            fm::Size tmp = index0;
            index0 = index1;
            index1 = tmp;
        }

        fm::Size N = elementCount();

        if (index1 >= N) return;

        GuiElement *e0 = remElement(index0);
        GuiElement *e1 = remElement(index1);

        insertElement(index0,e1);
        insertElement(index1,e0);
    }

    ////////////////////////////////////////////////////////////
    void Layout::setElement(fm::Size index,GuiElement *newElement)
    {
        delElement(index);
        insertElement(index,newElement);
    }

    ////////////////////////////////////////////////////////////
    fm::Size Layout::remElement(GuiElement *element)
    {
        fm::Size index = findElement(element);

        if (index != fm::Size(-1))
            remElement(index);

        return index;
    }

    ////////////////////////////////////////////////////////////
    void Layout::delElement(fm::Size index)
    {
        delete remElement(index);
    }

    ////////////////////////////////////////////////////////////
    void Layout::delElement(GuiElement *element)
    {
        remElement(element);
        delete element;
    }

    ////////////////////////////////////////////////////////////
    void Layout::onDraw(fg::ShaderManager &shader)
    {
        GuiElement::onDraw(shader);

        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
                e->onDraw(shader);
        }
    }

    ////////////////////////////////////////////////////////////
    void Layout::onUpdate(const fm::Time &dt)
    {
        GuiElement::onUpdate(dt);

        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
                e->onUpdate(dt);
        }
    }

    ////////////////////////////////////////////////////////////
    Layout &Layout::operator<<(GuiElement *newElement)
    {
        addElement(newElement);
        return *this;
    }

    ////////////////////////////////////////////////////////////
    Layout &operator>>(GuiElement *newElement,Layout &layout)
    {
        layout.insertElement(0,newElement);
        return layout;
    }

    ////////////////////////////////////////////////////////////
    GuiElement *Layout::operator[](fm::Size index)
    {
        return elementAt(index);
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::operator[](fm::Size index) const
    {
        return elementAt(index);
    }

    ////////////////////////////////////////////////////////////
    void Layout::childChanged(GuiElement *child)
    {
        (void)child;
    }

    ////////////////////////////////////////////////////////////
    void Layout::setActive(GuiElement *element)
    {
        m_active = element;
        GuiElement::setActive(m_active != fm::nullPtr);
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::getActive() const
    {
        return m_active;
    }

    ////////////////////////////////////////////////////////////
    GuiElement *Layout::findById(const fm::String &id)
    {
        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
            {
                if (e->getId() == id)
                    return e;
            }
        }

        return fm::nullPtr;
    }

    ////////////////////////////////////////////////////////////
    const GuiElement *Layout::findById(const fm::String &id) const
    {
        C(elementCount())
        {
            const GuiElement *e = elementAt(i);
            if (e)
            {
                if (e->getId() == id)
                    return e;
            }
        }

        return fm::nullPtr;
    }
}
