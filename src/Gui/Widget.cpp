#include <FRONTIER/Gui/Widget.hpp>
#include <iostream>
using namespace std;

namespace fgui
{
    Widget::Widget(GuiContext &owner,fm::vec2 size) :
        GuiElement(owner,size)
    {}

    /////////////////////////////////////////////////////////////
    void Widget::onDraw(fg::ShaderManager &shader)
    {
        GuiElement::onDraw(shader);
        ondraw(shader);
    }

    /////////////////////////////////////////////////////////////
    void Widget::onMouseMove(fm::vec2 p,fm::vec2 prevP)
    {
        MouseMoveListener::onMouseMove(p,prevP);
        onmousemove(p,prevP);
    }

    /////////////////////////////////////////////////////////////
    void Widget::onScroll(float amount,bool horizontal)
    {
        ScrollListener::onScroll(amount,horizontal);
        onscroll(amount,horizontal);
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

        if (ev.type == fw::Event::ButtonPressed)
            onbuttonpress(fm::vec2i(ev.mouse), ev.mouse.button);

        if (ev.type == fw::Event::ButtonReleased)
            onbuttonrelease(fm::vec2i(ev.mouse), ev.mouse.button);

        if (ev.type == fw::Event::Resized)
            onresize(ev.size);
        
        return handled;
    }
}