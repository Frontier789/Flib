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
#ifndef FRONTIER_LAYOUT_HPP_INCLUDED
#define FRONTIER_LAYOUT_HPP_INCLUDED
#include <FRONTIER/Gui/GuiElement.hpp>
#define FRONTIER_LAYOUT

namespace fgui
{
    class Layout : public GuiElement
    {
        GuiElement *m_active;
    public:
        Layout();

        /////////////////////////////////////////////////////////////
        Layout(const RelPos &pos,
               const fm::vec2 &size,
               const fm::String &id = "unnamed",
               Layout *parent = fm::nullPtr,
               bool enabled = true);

        /////////////////////////////////////////////////////////////
        explicit Layout(const fm::vec2 &size,
                        const fm::String &id = "unnamed",
                        Layout *parent = fm::nullPtr,
                        bool enabled = true);

        /////////////////////////////////////////////////////////////
        explicit Layout(const fm::String &id,
                        Layout *parent = fm::nullPtr,
                        bool enabled = true);

        /////////////////////////////////////////////////////////////
        virtual ~Layout();

        /////////////////////////////////////////////////////////////
        virtual bool handleEvent(const fw::Event &ev);

        /////////////////////////////////////////////////////////////
        virtual fm::Size          elementCount() const = 0;
        virtual GuiElement       *elementAt(fm::Size index) = 0;
        virtual const GuiElement *elementAt(fm::Size index) const;

        /////////////////////////////////////////////////////////////
		virtual void        prepareElement(GuiElement *newElement);
        virtual fm::Size    findElement(const GuiElement *element) const;
        virtual void        addElement(GuiElement *newElement);
        virtual void        swapElement(fm::Size index0,fm::Size index1);
        virtual void        setElement(fm::Size index,GuiElement *newElement);
        virtual void        insertElement(fm::Size index,GuiElement *newElement) = 0;
        virtual GuiElement *remElement(fm::Size index) = 0;
        fm::Size            remElement(GuiElement *element);
        virtual void        delElement(fm::Size index);
        virtual void        delElement(GuiElement *element);

        /////////////////////////////////////////////////////////////
        virtual void onDraw(fg::ShaderManager &shader);
        virtual void onUpdate(const fm::Time &dt = fm::Time::Zero);
		virtual void setResMan(ResourceManager *resMan);

        /////////////////////////////////////////////////////////////
        Layout &operator<<(GuiElement *newElement);
        GuiElement *operator[](fm::Size index);
        const GuiElement *operator[](fm::Size index) const;

        /////////////////////////////////////////////////////////////
        virtual void childChanged(GuiElement *child);

        /////////////////////////////////////////////////////////////
        virtual void setActive(GuiElement *element);
        const GuiElement *getActive() const;

        /////////////////////////////////////////////////////////////
        virtual GuiElement *findById(const fm::String &id);
        virtual const GuiElement *findById(const fm::String &id) const;
    };

    /////////////////////////////////////////////////////////////
    Layout &operator>>(GuiElement *newElement,Layout &layout);
}

#endif // FRONTIER_LAYOUT_HPP_INCLUDED

