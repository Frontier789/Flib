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
#ifndef FRONTIER_LINEARLAYOUT_HPP_INCLUDED
#define FRONTIER_LINEARLAYOUT_HPP_INCLUDED
#include <FRONTIER/Gui/Layout.hpp>
#define FRONTIER_LINEARLAYOUT
#include <vector>

namespace fgui
{
    class LinearLayout : public Layout
    {
    public:
        enum Orientation {
            Vertical,
            Horizontal
        };
        
		enum Align {
			Left   = 0,
			Middle = 1,
			Right  = 2,
			Up     = 0,
			Down   = 2
		};

    protected:
        std::vector<GuiElement*> m_elements;
		std::vector<int> m_elemAligns;
        Orientation m_ori;
        bool m_needRecalc;
		int m_align;

        /////////////////////////////////////////////////////////////
        void recalc();

    public:
        LinearLayout();

        /////////////////////////////////////////////////////////////
        LinearLayout(const RelPos &pos,
                     const fm::vec2 &size,
                     Orientation ori = Vertical,
                     const fm::String &id = "unnamed",
                     Layout *parent = fm::nullPtr,
                     int align = Left);

        /////////////////////////////////////////////////////////////
        explicit LinearLayout(const fm::vec2 &size,
                              Orientation ori = Vertical,
                              const fm::String &id = "unnamed",
                              Layout *parent = fm::nullPtr,
                              int align = Left);

        /////////////////////////////////////////////////////////////
        explicit LinearLayout(Orientation ori,
                              const fm::String &id = "unnamed",
                              Layout *parent = fm::nullPtr,
                              int align = Left);

        /////////////////////////////////////////////////////////////
        explicit LinearLayout(const fm::String &id,
                              Layout *parent = fm::nullPtr,
                              int align = Left);

        /////////////////////////////////////////////////////////////
        explicit LinearLayout(Orientation ori,
                              int align = Left);

        /////////////////////////////////////////////////////////////
        virtual ~LinearLayout();

        /////////////////////////////////////////////////////////////
        virtual fm::Size    elementCount() const;
        virtual GuiElement *elementAt(fm::Size index);
        virtual void        insertElement(fm::Size index,GuiElement *newElement);
        virtual GuiElement *remElement(fm::Size index);
        
        /////////////////////////////////////////////////////////////
		virtual void setAlign(int align);
		virtual void setAlign(fm::Size id,int align);
		int getAlign(fm::Size id) const;
		int getAlign() const;

        /////////////////////////////////////////////////////////////
        virtual void setSize(const fm::vec2 &size);
        virtual fm::vec2 getSize() const;

        /////////////////////////////////////////////////////////////
        virtual void onUpdate(const fm::Time &dt = fm::Time::Zero);

        /////////////////////////////////////////////////////////////
        virtual void childChanged(GuiElement *child);

        /////////////////////////////////////////////////////////////
        virtual void setOrientation(Orientation ori);
        Orientation getOrientation() const;
    };
}

#endif // FRONTIER_LINEARLAYOUT_HPP_INCLUDED

