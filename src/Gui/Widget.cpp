#include <FRONTIER/Gui/Widget.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fgui
{
    ////////////////////////////////////////////////////////////
    Widget::Widget(const RelPos &pos,
                   const fm::vec2 &size,
                   const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(pos,size,id,parent,enabled),
                                   m_mouseIn(false),
                                   m_pressed(0)
    {

    }

    ////////////////////////////////////////////////////////////
    Widget::Widget(const fm::vec2 &size,
                   const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(fm::vec2(),size,id,parent,enabled),
                                   m_mouseIn(false),
                                   m_pressed(0)
    {

    }

    ////////////////////////////////////////////////////////////
    Widget::Widget(const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(id,parent,enabled),
                                   m_mouseIn(false),
                                   m_pressed(0)
    {

    }

    ////////////////////////////////////////////////////////////
    Widget::~Widget()
    {

    }

    ////////////////////////////////////////////////////////////
    bool Widget::handleEvent(const fw::Event &ev)
    {
        if (GuiElement::handleEvent(ev))
            return true;

        if (ev.type == fw::Event::MouseMoved ||
            ev.type == fw::Event::MouseEntered ||
            ev.type == fw::Event::MouseLeft)
        {
            fm::vec2 p  = fm::vec2i(ev.motion);
            bool inside = contains(p);

            if (!m_mouseIn && inside)
            {
                m_mouseIn = true;
                handleEnter(p);

                return false;
            }
            if (m_mouseIn && !inside && !m_pressed)
            {
                m_mouseIn = false;
                handleLeave(p);

                return false;
            }

            if (m_mouseIn)
            {
                handleHover(p);
            }

            return true;
        }

        if (ev.type == fw::Event::ButtonPressed)
        {
            fm::vec2 p  = fm::vec2i(ev.mouse);
            bool inside = contains(p);

            if (inside && ev.mouse.button == fw::Mouse::Left)
                setActive(true);

            if (m_pressed)
            {
                m_pressed++;
                handlePress(p,ev.mouse.button);

                return true;
            }
            else
            {
                if (inside)
                {
                    if (!m_mouseIn)
                    {
                        m_mouseIn = true;
                        handleEnter(p);

                        return false;
                    }

                    setActive(true);

                    m_pressed++;
                    handlePress(p,ev.mouse.button);

                    return true;
                }
            }

            return false;
        }

        if (ev.type == fw::Event::ButtonReleased)
        {
            fm::vec2 p  = fm::vec2i(ev.mouse);
            bool inside = contains(p);

            if (m_pressed)
            {
                m_pressed--;
                handleRelease(p,ev.mouse.button);

                if (!m_pressed)
                {
                    if (!inside)
                    {
                        m_mouseIn = false;
                        handleLeave(p);
                    }

                    return false;
                }

                return true;
            }
            else
            {
                if (m_mouseIn && inside)
                {
                    m_mouseIn = true;
                    handleEnter(p);

                    return true;
                }
            }

            return false;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////
    void Widget::handleEnter(fm::vec2 p)
    {
        onEnter(*this,p);
    }

    ////////////////////////////////////////////////////////////
    void Widget::handleLeave(fm::vec2 p)
    {
        onLeave(*this,p);
    }

    ////////////////////////////////////////////////////////////
    void Widget::handleHover(fm::vec2 p)
    {
        onHover(*this,p);
    }

    ////////////////////////////////////////////////////////////
    void Widget::handlePress(fm::vec2 p,fw::Mouse::Button btn)
    {
        onPress(*this,p,btn);
    }

    ////////////////////////////////////////////////////////////
    void Widget::handleRelease(fm::vec2 p,fw::Mouse::Button btn)
    {
        onRelease(*this,p,btn);
    }

    ////////////////////////////////////////////////////////////
    bool Widget::contains(fm::vec2 p) const
    {
        return fm::rect2f(getPos()+fm::vec2(1,1),getSize()-fm::vec2(1,1)).contains(p);
    }
}
