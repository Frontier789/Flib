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
#ifndef FRONTIER_WIDGET_HPP_INCLUDED
#define FRONTIER_WIDGET_HPP_INCLUDED
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Window/Event.hpp>
#define FRONTIER_WIDGET

namespace fgui
{
    class Widget : public GuiElement
    {
    protected:
        bool m_mouseIn;
        bool m_pressed;
		bool m_pressedRight;
		fw::Mouse::Cursor m_cursor;
		
    public:
        ////////////////////////////////////////////////////////////
        fm::Delegate<void,Widget &,fm::vec2> onEnter;
        fm::Delegate<void,Widget &,fm::vec2> onHover;
        fm::Delegate<void,Widget &,fm::vec2> onLeave;

        ////////////////////////////////////////////////////////////
        fm::Delegate<void,Widget &,fm::vec2,fw::Mouse::Button> onPress;
        fm::Delegate<void,Widget &,fm::vec2,fw::Mouse::Button> onRelease;

        ////////////////////////////////////////////////////////////
		explicit Widget(const RelPos &pos = RelPos(),
						const fm::vec2 &size = fm::vec2(),
						const fm::String &id = "unnamed",
						Layout *parent = fm::nullPtr,
						bool enabled = true);

        ////////////////////////////////////////////////////////////
        explicit Widget(const fm::vec2 &size,
                        const fm::String &id = "unnamed",
                        Layout *parent = fm::nullPtr,
                        bool enabled = true);

        ////////////////////////////////////////////////////////////
        explicit Widget(const fm::String &id,
                        Layout *parent = fm::nullPtr,
                        bool enabled = true);

        ////////////////////////////////////////////////////////////
        virtual ~Widget();

        ////////////////////////////////////////////////////////////
        virtual bool handleEvent(const fw::Event &ev);

        ////////////////////////////////////////////////////////////
        virtual void handleEnter(fm::vec2 p);
        virtual void handleLeave(fm::vec2 p);
        virtual void handleHover(fm::vec2 p);
        virtual void handlePress(fm::vec2 p,fw::Mouse::Button btn);
        virtual void handleRelease(fm::vec2 p,fw::Mouse::Button btn);
        
        ////////////////////////////////////////////////////////////
		bool getPressed() const;
		bool getMouseIn() const;
        
        ////////////////////////////////////////////////////////////
		virtual void setCursor(fw::Mouse::Cursor cursor);
		fw::Mouse::Cursor getCursor() const;

        ////////////////////////////////////////////////////////////
        virtual bool contains(fm::vec2 p) const;
    };
}

#endif // FRONTIER_GUIELEMENT_HPP_INCLUDED
