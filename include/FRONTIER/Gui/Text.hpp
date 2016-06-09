#ifndef FRONTIER_TEXT_HPP_INCLUDED
#define FRONTIER_TEXT_HPP_INCLUDED
#include <FRONTIER/Graphics/DrawData.hpp>#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#define FRONTIER_TEXT

class Text : public GuiElement
{
protected:
    ////////////////////////////////////////////////////////////
    fg::DrawData m_draw;
    fm::String m_text;
    const fg::Font *m_font;
    const fg::Texture *m_tex;
    fm::Size m_charSize;
    int m_align;
    fm::vec4 m_clr;
    bool m_needRecalc;

    ////////////////////////////////////////////////////////////
    virtual void recalc();

public:
    ////////////////////////////////////////////////////////////
    explicit Text(const fm::vec2 &pos,
                  const fm::String &id = "unnamed",
                  Layout *parent = fm::nullPtr,
                  const fm::String &text = "",
                  const fg::Font *font = fm::nullPtr,
                  fm::Size characterSize = 12,
                  int align = 0,
                  fm::vec4 clr = fm::vec4::Black);

    ////////////////////////////////////////////////////////////
    explicit Text(const fm::String &text,
                  fg::Font *font = fm::nullPtr,
                  fm::Size characterSize = 12,
                  int align = 0,
                  fm::vec4 clr = fm::vec4::Black);

    ////////////////////////////////////////////////////////////
    virtual void onDraw(fg::ShaderManager &shader) const;
    virtual void onUpdate(float dt = 0.f);

    ////////////////////////////////////////////////////////////
    virtual void setSize(const fm::vec2 &size);
    virtual fm::vec2 getSize() const;

    ////////////////////////////////////////////////////////////
    virtual void setText(const fm::String &text);
    const fm::String &getText() const;

    ////////////////////////////////////////////////////////////
    virtual void setFont(const fg::Font *font);
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

#endif // FRONTIER_TEXT_HPP_INCLUDED

