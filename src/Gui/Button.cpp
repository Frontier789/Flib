#include <FRONTIER/Gui/Button.hpp>
#include <FRONTIER/System/Rect.hpp>

////////////////////////////////////////////////////////////
void Button::onHover(bool on)
{
    (void)on;
}

////////////////////////////////////////////////////////////
void Button::onPress(bool on)
{
    (void)on;
}

////////////////////////////////////////////////////////////
Button::Button(const fm::vec2 &pos,
               const fm::vec2 &size,
               const fm::String &id,
               Layout *parent,
               const fm::Delegate<void,Button*> &callback,
               State state) : GuiElement(pos,size,id,parent),
                              m_state(state),
                              m_callback(callback)
{

}

////////////////////////////////////////////////////////////
Button::Button(const fm::vec2 &size,
               const fm::String &id,
               Layout *parent,
               const fm::Delegate<void,Button*> &callback,
               State state) : GuiElement(fm::vec2(),size,id,parent),
                              m_state(state),
                              m_callback(callback)
{

}

////////////////////////////////////////////////////////////
Button::Button(const fm::vec2 &size,
               const fm::String &id,
               const fm::Delegate<void,Button*> &callback,
               State state) : GuiElement(fm::vec2(),size,id,fm::nullPtr),
                              m_state(state),
                              m_callback(callback)
{

}

////////////////////////////////////////////////////////////
Button::Button(const fm::vec2 &size,
               const fm::Delegate<void,Button*> &callback,
               State state) : GuiElement(fm::vec2(),size,"unnamed",fm::nullPtr),
                              m_state(state),
                              m_callback(callback)
{

}

////////////////////////////////////////////////////////////
Button::Button(const fm::Delegate<void,Button*> &callback,
               State state) : GuiElement(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr),
                              m_state(state),
                              m_callback(callback)
{

}

////////////////////////////////////////////////////////////
bool Button::contains(const fm::vec2 &p) const
{
    return fm::rect2f(getPos()+fm::vec2(1,1),getSize()-fm::vec2(1,1)).contains(p);
}

////////////////////////////////////////////////////////////
bool Button::handleEvent(const fw::Event &ev)
{
    if (GuiElement::handleEvent(ev))
        return true;

    if (ev.type == fw::Event::MouseMoved ||
        ev.type == fw::Event::MouseEntered ||
        ev.type == fw::Event::MouseLeft)
    {
        fm::vec2 p = fm::vec2i(ev.motion);
        bool inside = contains(p);

        if (m_state == MouseOut && inside)
        {
            m_state = MouseOver;
            onHover(true);

            return false;
        }
        if (m_state == MouseOver && !inside)
        {
            m_state = MouseOut;
            onHover(false);

            return false;
        }

        if (m_state == MouseOver && inside)
            return true;

        if (m_state == Pressed)
            return true;
    }

    if (ev.type == fw::Event::ButtonPressed &&
        ev.mouse.button == fw::Mouse::Left)
    {
        if (contains(fm::vec2i(ev.mouse)))
        {
            m_state = Pressed;
            setActive();
            onPress(true);

            m_callback(this);

            return true;
        }
    }

    if (ev.type == fw::Event::ButtonReleased &&
        ev.mouse.button == fw::Mouse::Left)
    {
        if (m_state == Pressed)
        {
            bool inside = contains(fm::vec2i(ev.mouse));

            if (inside)
                m_state = MouseOver;
            else
                m_state = MouseOut;

            onPress(false);
            setActive(false);

            if (!inside)
                onHover(false);

            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////
void Button::setCallback(const fm::Delegate<void,Button*> &callback)
{
    m_callback = callback;
}

////////////////////////////////////////////////////////////
const fm::Delegate<void,Button*> &Button::getCallback() const
{
    return m_callback;
}

