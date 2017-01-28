#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/FramedSprite.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Vector4.hpp>

namespace fg
{
    void FramedSprite::buildVertices()
    {
        if (!m_tex) return;

        if (!m_texRect.size.area()) m_texRect.size = m_tex->getRealSize();

        if (!m_size.area()) m_size = m_texRect.size;

        if (m_frameSize.w*2 > m_texRect.size.w) m_frameSize.w = m_texRect.size.w/2;
        if (m_frameSize.h*2 > m_texRect.size.h) m_frameSize.h = m_texRect.size.h/2;

        if (m_size.w < m_frameSize.w*2) m_size.w = m_frameSize.w*2;
        if (m_size.h < m_frameSize.h*2) m_size.h = m_frameSize.h*2;

        fm::vec2 pts[4*4];
        fm::vec2 uvs[4*4];
        fm::vec4 clr[4*4];
        fm::Uint16 inds[9*2*3];

        Cx(4)Cy(4)
        {
            pts[4*x+y] = (fm::vec2(x,y)-fm::vec2i(x,y)/2) * m_frameSize + fm::vec2i(x,y)/2 * (m_size - m_frameSize*2);

            uvs[4*x+y] = (fm::vec2(x,y)-fm::vec2i(x,y)/2) * m_frameSize + fm::vec2i(x,y)/2 * (m_texRect.size - m_frameSize*2) + m_texRect.pos;

            clr[4*x+y] = fm::vec4::White;

            if (x<3 && y<3)
            {
                fm::Size offset = 6*(3*x+y);

                inds[offset + 0] = 4*x+y;
                inds[offset + 1] = 4*x+y+1;
                inds[offset + 2] = 4*x+y+4;
                inds[offset + 3] = 4*x+y+4;
                inds[offset + 4] = 4*x+y+1;
                inds[offset + 5] = 4*x+y+5;
            }
        }

        m_draw.reset();

        m_draw[fg::Assoc::Position]  = pts;
        m_draw[fg::Assoc::TextureUV] = uvs;
        m_draw[fg::Assoc::Color]     = clr;
        m_draw.addDraw(inds,fg::Triangles);
    }


    /////////////////////////////////////////////////////////////
    FramedSprite::FramedSprite(fm::Ref<const fg::Texture> tex,
                               const fm::rect2s &texRect,
                               const fm::vec2s &frameSize,
                               const fm::vec2 &pos,
                               const fm::vec2 &size) : m_tex(tex),
                                                       m_frameSize(frameSize),
                                                       m_texRect(texRect),
                                                       m_size(size),
                                                       m_pos(pos)
    {
        buildVertices();
    }

    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setPos(const fm::vec2 &pos)
    {
        m_pos = pos;
        return *this;
    }

    const fm::vec2 &FramedSprite::getPos() const
    {
        return m_pos;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setSize(const fm::vec2 &size)
    {
		if (m_size != size)
		{			
			m_size = size;
			buildVertices();
		}
        return *this;
    }

    const fm::vec2 &FramedSprite::getSize() const
    {
        return m_size;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setTexRect(const fm::rect2s &texRect)
    {
        m_texRect = texRect;
        m_size = texRect.size;
        buildVertices();
        return *this;
    }

    const fm::rect2s &FramedSprite::getTexRect() const
    {
        return m_texRect;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setTexture(fm::Ref<const fg::Texture> tex,const fm::rect2s &texRect)
    {
        m_tex = tex;
        setTexRect(texRect);
        return *this;
    }

    const fg::Texture *FramedSprite::getTexture() const
    {
        return m_tex;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setFrameSize(const fm::vec2s &frameSize)
    {
        m_frameSize = frameSize;
        buildVertices();
        return *this;
    }

    const fm::vec2s &FramedSprite::getFrameSize() const
    {
        return m_frameSize;
    }


    /////////////////////////////////////////////////////////////
    void FramedSprite::onDraw(ShaderManager &shader)
    {
        if (!m_tex) return;

        shader.getModelStack().push().mul(fm::MATRIX::translation(fm::vec2i(m_pos)));
        shader.getTexUVStack().push().mul(m_tex->getPixToUnitMatrix());
        shader.useTexture(m_tex);
        shader.draw(m_draw);
        shader.useTexture(nullptr);
        shader.getModelStack().pop();
        shader.getTexUVStack().pop();
    }


    /////////////////////////////////////////////////////////////
    void FramedSprite::onUpdate(const fm::Time &dt)
    {
        (void)dt;
    }
}
