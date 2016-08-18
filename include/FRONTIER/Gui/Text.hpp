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
#ifndef FRONTIER_TEXT_HPP_INCLUDED
#define FRONTIER_TEXT_HPP_INCLUDED
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Ref.hpp>
#define FRONTIER_TEXT

namespace fgui
{
    class Text : public GuiElement
    {
    protected:
        ////////////////////////////////////////////////////////////
        const fg::Texture *m_tex;
        const fg::Font *m_font;
        fg::DrawData m_draw;
        fm::Size m_charSize;
        bool m_needRecalc;
        fm::String m_text;
        fm::vec4 m_clr;
        int m_align;

        ////////////////////////////////////////////////////////////
        virtual void recalc();

    public:
        ////////////////////////////////////////////////////////////
        explicit Text(const RelPos &pos,
                      const fm::String &id,
                      Layout *parent,
                      const fm::String &text = "",
                      fm::Ref<const fg::Font> font = fm::nullPtr,
                      fm::Size characterSize = 12,
                      int align = 0,
                      fm::vec4 clr = fm::vec4::Black);

        ////////////////////////////////////////////////////////////
        explicit Text(const fm::String &text,
                      fm::Ref<const fg::Font> font = fm::nullPtr,
                      fm::Size characterSize = 12,
                      int align = 0,
                      fm::vec4 clr = fm::vec4::Black);

        ////////////////////////////////////////////////////////////
        explicit Text(const RelPos &pos,
                      const fm::String &text,
                      fm::Ref<const fg::Font> font = fm::nullPtr,
                      fm::Size characterSize = 12,
                      int align = 0,
                      fm::vec4 clr = fm::vec4::Black);

        ////////////////////////////////////////////////////////////
        virtual void onDraw(fg::ShaderManager &shader);
		virtual void onUpdate(const fm::Time &dt = fm::Time::Zero);
		virtual void setResMan(ResourceManager *resMan);

        ////////////////////////////////////////////////////////////
        virtual void setSize(const fm::vec2 &size);
        virtual fm::vec2 getSize() const;

        ////////////////////////////////////////////////////////////
        virtual void setText(const fm::String &text);
        const fm::String &getText() const;

        ////////////////////////////////////////////////////////////
        virtual void setFont(fm::Ref<const fg::Font> font);
        const fg::Font *getFont() const;

        ////////////////////////////////////////////////////////////
        virtual void setCharSize(fm::Size charSize);
        fm::Size getCharSize() const;

        ////////////////////////////////////////////////////////////
        virtual void setAlign(int align);
        int getAlign() const;

        ////////////////////////////////////////////////////////////
        virtual void setColor(fm::vec4 clr);
        fm::vec4 getColor() const;

    };
}

#endif // FRONTIER_TEXT_HPP_INCLUDED

