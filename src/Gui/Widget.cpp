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
                                   m_pressed(false)
    {

    }

    ////////////////////////////////////////////////////////////
    Widget::Widget(const fm::vec2 &size,
                   const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(fm::vec2(),size,id,parent,enabled),
                                   m_mouseIn(false),
                                   m_pressed(false),
                                   m_pressedRight(false)
    {

    }

    ////////////////////////////////////////////////////////////
    Widget::Widget(const fm::String &id,
                   Layout *parent,
                   bool enabled) : GuiElement(id,parent,enabled),
                                   m_mouseIn(false),
                                   m_pressed(false),
                                   m_pressedRight(false)
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

                return m_pressed;
            }
			
			if (m_mouseIn && !inside)
            {
                m_mouseIn = false;
                handleLeave(p);

                return m_pressed;
            }

			if ((m_mouseIn && inside) || m_pressed || m_pressedRight)
            {
                handleHover(p);
				return true;
            }

            return m_pressed;
        }

        if (ev.type == fw::Event::ButtonPressed)
        {
            fm::vec2 p  = fm::vec2i(ev.mouse);
            bool inside = contains(p);

            if (inside)
            {
				if (!m_mouseIn)
				{
					m_mouseIn = true;
                    handleEnter(p);
				}
            	
				if (ev.mouse.button == fw::Mouse::Left)
					m_pressed = true;
					
				if (ev.mouse.button == fw::Mouse::Right)
					m_pressedRight = true;
				
				setActive(true);
                handlePress(p,ev.mouse.button);

                return true;
            }

            return false;
        }

        if (ev.type == fw::Event::ButtonReleased)
        {
            fm::vec2 p  = fm::vec2i(ev.mouse);
            
			if (ev.mouse.button == fw::Mouse::Left)
			{
				if (m_pressed)
				{
					m_pressed = false;
					handleRelease(p,ev.mouse.button);
				}
				
				return true;
			}
			
			if (ev.mouse.button == fw::Mouse::Right)
			{
				if (m_pressedRight)
				{
					m_pressedRight = false;
					handleRelease(p,ev.mouse.button);
				}
				
				return true;
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
	bool Widget::getPressed() const
	{
		return m_pressed;
	}
	
	////////////////////////////////////////////////////////////
	bool Widget::getMouseIn() const
	{
		return m_mouseIn;
	}

    ////////////////////////////////////////////////////////////
    bool Widget::contains(fm::vec2 p) const
    {
        return fm::rect2f(getPos()+fm::vec2(1,1),getSize()-fm::vec2(1,1)).contains(p);
    }
}
