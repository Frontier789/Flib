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
#include <FRONTIER/Gui/ResourceManager.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/Gui/Text.hpp>
#include "TextToDraw.hpp"

namespace fgui
{
    ////////////////////////////////////////////////////////////
    Text::Text(const RelPos &pos,
               const fm::String &id,
               Layout *parent,
               const fm::String &text,
               fm::Ref<const fg::Font> font,
               fm::Size characterSize,
               int align,
               fm::vec4 clr) : GuiElement(pos,fm::vec2(),id,parent),
                               m_tex(fm::nullPtr),
                               m_font(font),
                               m_charSize(characterSize),
                               m_needRecalc(true),
                               m_text(text),
                               m_clr(clr),
                               m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    Text::Text(const fm::String &text,
               fm::Ref<const fg::Font> font,
               fm::Size characterSize,
               int align,
               fm::vec4 clr) : GuiElement(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr),
                               m_tex(fm::nullPtr),
                               m_font(font),
                               m_charSize(characterSize),
                               m_needRecalc(true),
                               m_text(text),
                               m_clr(clr),
                               m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    Text::Text(const RelPos &pos,
               const fm::String &text,
               fm::Ref<const fg::Font> font,
               fm::Size characterSize,
               int align,
               fm::vec4 clr) : GuiElement(pos,fm::vec2(),"unnamed",fm::nullPtr),
                               m_tex(fm::nullPtr),
                               m_font(font),
                               m_charSize(characterSize),
                               m_needRecalc(true),
                               m_text(text),
                               m_clr(clr),
                               m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    void Text::recalc()
    {
        const fm::String *line = &m_text;
        
        m_draw.reset();

        priv::getDrawFromText(m_draw,&line,1,m_font,m_charSize,&m_tex,&m_realSize,fm::rect2i(),m_clr);
    }

    ////////////////////////////////////////////////////////////
    void Text::onDraw(fg::ShaderManager &shader)
    {
        if (!m_tex)
            return;

        GuiElement::onDraw(shader);

        fw::Window::setBlend(fg::Alpha);

        shader.getModelStack().push().mul(fm::MATRIX::translation(fm::vec2i(getPos())));
        shader.getColorStack().push(fm::MATRIX::scaling(fm::vec3(m_clr)));
        shader.getTexUVStack().push(m_tex->getPixToUnitMatrix());
        shader.useTexture(m_tex);
        shader.draw(m_draw);
        shader.useTexture(fm::nullPtr);
        shader.getTexUVStack().pop();
        shader.getColorStack().pop();
        shader.getModelStack().pop();
    }

    ////////////////////////////////////////////////////////////
	void Text::onUpdate(const fm::Time &dt)
    {
        (void)dt;

        if (m_needRecalc)
        {
            m_needRecalc = false;
            recalc();
        }
    }
        
    ////////////////////////////////////////////////////////////
	void Text::setResMan(ResourceManager *resMan)
	{
		GuiElement::setResMan(resMan);
		
		if (resMan)
		{
			fg::Font *font = (fg::Font*)resMan->get("defFont");
			if (font) setFont(font);
		}
	}

    ////////////////////////////////////////////////////////////
    void Text::setSize(const fm::vec2 &size)
    {
        (void)size;
    }

    ////////////////////////////////////////////////////////////
    fm::vec2 Text::getSize() const
    {
        if (m_needRecalc)
        {
            ((Text*)this)->m_needRecalc = false;
            ((Text*)this)->recalc();
        }

        return GuiElement::getSize();
    }

    ////////////////////////////////////////////////////////////
    void Text::setText(const fm::String &text)
    {
        m_text = text;
        m_needRecalc = true;
    }

    ////////////////////////////////////////////////////////////
    const fm::String &Text::getText() const
    {
        return m_text;
    }

    ////////////////////////////////////////////////////////////
    void Text::setFont(fm::Ref<const fg::Font> font)
    {
        if (m_font == font) return;

        m_font = font;
        m_needRecalc = true;
    }

    ////////////////////////////////////////////////////////////
    const fg::Font *Text::getFont() const
    {
        return m_font;
    }

    ////////////////////////////////////////////////////////////
    void Text::setCharSize(fm::Size charSize)
    {
        if (m_charSize == charSize) return;

        m_charSize = charSize;
        m_needRecalc = true;
    }

    ////////////////////////////////////////////////////////////
    fm::Size Text::getCharSize() const
    {
        return m_charSize;
    }

    ////////////////////////////////////////////////////////////
    void Text::setAlign(int align)
    {
        if (m_align == align) return;

        m_align = align;
        m_needRecalc = true;
    }

    ////////////////////////////////////////////////////////////
    int Text::getAlign() const
    {
        return m_align;
    }

    ////////////////////////////////////////////////////////////
    void Text::setColor(fm::vec4 clr)
    {
        if (m_clr == clr) return;

        m_clr = clr;
        m_needRecalc = true;
    }

    ////////////////////////////////////////////////////////////
    fm::vec4 Text::getColor() const
    {
        return m_clr;
    }
}
