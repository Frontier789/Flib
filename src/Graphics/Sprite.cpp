#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Sprite.hpp>

namespace fg
{
    /////////////////////////////////////////////////////////////
    Sprite::Sprite(fm::Ref<const fg::Texture> tex,
                   const fm::rect2s &texRect,
                   const fm::vec2 &pos,
                   const fm::vec2 &size) :  m_tex(tex),
                                            m_pos(pos)
    {
        setTexRect(texRect);
        setSize(size);
    }

    /////////////////////////////////////////////////////////////
    void Sprite::init()
    {
		if (!m_tex) return;

		if (m_draw.getDrawCount() == 0)
        {
            fm::vec2 pts[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,1)};
            fm::vec4 clr[] = {fm::vec4::White,fm::vec4::White,fm::vec4::White,fm::vec4::White};

            m_draw[fg::Assoc::Position]  = pts;
            m_draw[fg::Assoc::TextureUV] = pts;
            m_draw[fg::Assoc::Color]     = clr;

            m_draw.addDraw(0,4,fg::TriangleFan);
        }
    }

    /////////////////////////////////////////////////////////////
    Sprite &Sprite::setPos(const fm::vec2 &pos)
    {
        init();
        m_pos = pos;
        return *this;
    }

    const fm::vec2 &Sprite::getPos() const
    {
        return m_pos;
    }


    /////////////////////////////////////////////////////////////
    Sprite &Sprite::setSize(const fm::vec2 &size)
    {
        init();

        m_size = size;

        if (m_size.area() == 0)
        {
            if (m_tex)
                m_size = m_tex->getRealSize();
        }

        return *this;
    }

    const fm::vec2 &Sprite::getSize() const
    {
        return m_size;
    }


    /////////////////////////////////////////////////////////////
    Sprite &Sprite::setTexRect(const fm::rect2s &texRect)
    {
        init();
        m_texRect = texRect;

        if (m_texRect.area() == 0)
        {
            if (m_tex)
                m_texRect.size = m_tex->getRealSize();
        }

        m_size = texRect.size;

        return *this;
    }

    const fm::rect2s &Sprite::getTexRect() const
    {
        return m_texRect;
    }


    /////////////////////////////////////////////////////////////
    Sprite &Sprite::setTexture(fm::Ref<const fg::Texture> tex,const fm::rect2s &texRect)
    {
        m_tex = tex;
        setTexRect(texRect);
        return *this;
    }

    const fg::Texture *Sprite::getTexture() const
    {
        return m_tex;
    }


    /////////////////////////////////////////////////////////////
    void Sprite::onDraw(ShaderManager &shader) const
    {
        if (!m_tex) return;

        shader.getModelStack().push().mul(fm::MATRIX::scaling(m_size)*fm::MATRIX::translation(m_pos));
        shader.getTexUVStack().push().mul(m_tex->getPixToUnitMatrix()*fm::MATRIX::scaling((fm::vec2)m_texRect.size)*fm::MATRIX::translation((fm::vec2)m_texRect.pos));
        shader.useTexture(m_tex);
        shader.draw(m_draw);
        shader.useTexture(fm::nullPtr);
        shader.getModelStack().pop();
        shader.getTexUVStack().pop();
    }


    /////////////////////////////////////////////////////////////
    void Sprite::onUpdate(const fm::Time &dt)
    {
        (void)dt;
    }
}
