#include <FRONTIER/Gui/Widget.hpp>

namespace fgui
{
    Widget::Widget(GuiContext &owner,fm::vec2 size) : GuiElement(owner,size) {}

    /////////////////////////////////////////////////////////////
    void Widget::onDraw(fg::ShaderManager &shader)
    {
        GuiElement::onDraw(shader);
        ondraw(shader);
    }

    /////////////////////////////////////////////////////////////
    void Widget::onMouseMove(fm::vec2 p,fm::vec2 prevP)
    {
        ClickListener::onMouseMove(p,prevP);
        onmousemove(p,prevP);
    }

    /////////////////////////////////////////////////////////////
    void Widget::onScroll(float amount)
    {
        ScrollListener::onScroll(amount);
        onscroll(amount);
    }

    /////////////////////////////////////////////////////////////
    void Widget::onUpdate(const fm::Time &dt)
    {
        GuiElement::onUpdate(dt);
        onupdate(dt);
    }

    /////////////////////////////////////////////////////////////
    bool Widget::onEvent(fw::Event &ev)
    {
        bool handled = GuiElement::onEvent(ev);
        onevent(ev);

        if (ev.type == fw::Event::KeyPressed)
            onkeypress(ev.key.code);

        if (ev.type == fw::Event::KeyReleased)
            onkeyrelease(ev.key.code);

        return handled;
    }
}