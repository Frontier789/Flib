#include <FRONTIER/System/macros/OFFSETOF.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Vertex.hpp>
#include <FRONTIER/Gui/BasicText.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string BasicText::m_className = "BasicText";

	/////////////////////////////////////////////////////////////
	bool printable(const fm::Uint32 &c)
	{
		return fm::String("\n\r\t\b ").find(c) == fm::String::npos;
	}

	/////////////////////////////////////////////////////////////
	void extendSize(fm::vec2 &size,fm::vec2 p)
	{
		if (p.x > size.w)
			size.w = p.x;

		if (p.y > size.h)
			size.h = p.y;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::buildVertices()
	{
		m_tex = fm::nullPtr;
		m_quadCount = 0;

		/** check if text is empty **/
		fm::Size textLength = m_text.size();

		if (!m_font || !textLength)
			return;

		/** count quads **/
		for (fm::Size i=0;i<textLength;i++)
			m_quadCount += printable(m_text[i]);

		/** alloc vertices **/
		fm::vertex2f *m_vrts = new fm::vertex2f[m_quadCount*4];
		fm::Uint16 *m_inds = new fm::Uint16[m_quadCount*6];

		/** setup vertices **/
		m_font->setCharacterSize(m_charSize);
		m_tex = &m_font->getTexture();

		fg::Metrics metrics = m_font->getMetrics();

		float monoWidth = m_font->getGlyph('a').size.w+2;

		fm::vec2 cursor = fm::vec2(2,metrics.maxH);

		m_size = fm::vec2();

		fm::Uint16 index=0;
		for (fm::Size i=0;i<textLength;i++)
		{
			if (printable(m_text[i]))
			{
				fg::Glyph glyph = m_font->getGlyph(m_text[i]);

				float xadvance = m_monoSpace ? monoWidth : glyph.size.w+2;

				if (m_maxWidth && cursor.x + xadvance > m_maxWidth && !m_wordWrap)
				{
					cursor.x = 2;
					cursor.y += metrics.lineGap;
				}

				fm::vec2 p = cursor - glyph.leftdown*fm::vec2(0,1);

				if (m_monoSpace)
					p.x += int(monoWidth/2 - glyph.size.w/2);
				else
					p.x += 1;

				Cxy(2,2)
					m_vrts[index*4+x*2+y].pos = p+glyph.size*fm::vec2(x,y-1.0),
					m_vrts[index*4+x*2+y].texPos = glyph.pos+glyph.size*fm::vec2(x,y),
					m_vrts[index*4+x*2+y].clr = m_clr;

				extendSize(m_size,m_vrts[index*4+3].pos);
				extendSize(m_size,cursor+fm::vec2(0,metrics.lineGap-metrics.maxH));

				Cx(3)
					m_inds[index*6+x] = index*4+x,
					m_inds[index*6+3+x] = index*4+x+1;

				cursor.x += xadvance;

				index ++;
			}
			else
			{
				if (m_text[i] == ' ')
				{
					cursor.x+=monoWidth;
				}
				if (m_text[i] == '\t')
				{
					cursor.x+=monoWidth*4;
				}
				if (m_text[i] == '\n')
				{
					cursor.x = 2;
					cursor.y += metrics.lineGap;
				}

				if (m_maxWidth && i+1 < textLength && printable(m_text[i+1]))
				{
					float wordWidth = 0;
					fm::Size wi = i+1;

					do
					{
						wordWidth += m_font->getGlyph(m_text[wi]).size.w;

						wi++;
					} while (wi < textLength && printable(m_text[wi]));

					if (cursor.x + wordWidth > m_maxWidth)
					{
						cursor.x = 2;
						cursor.y += metrics.lineGap;
					}
				}
			}

			extendSize(m_size,cursor);
		}

		m_vbo.setData(m_vrts,sizeof(*m_vrts)*m_quadCount*4);
		m_ibo.setData(m_inds,sizeof(*m_inds)*m_quadCount*6);

		delete[] m_vrts;
		delete[] m_inds;

		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setupPosition()
	{
		m_transform = fm::MATRIX::translation(fm::vec2i(getPosInRoot()));
	}

	/////////////////////////////////////////////////////////////
	BasicText::BasicText(const std::string &name,
						   const Anchor &anchor,
						   const fm::vec2 &size,
						   Widget *parent,
						   const fm::String &text,
						   fg::Font *font,
						   const fm::vec4 &clr,
						   unsigned int charSize,
						   bool monoSpacing,
						   float maxWidth,
						   bool wordWrap) : Widget(name,anchor,size,parent),
											m_tex(fm::nullPtr),
											m_charSize(charSize),
											m_text(text),
											m_font(font),
											m_clr(clr),
											m_vbo(fg::ArrayBuffer),
											m_ibo(fg::IndexBuffer),
											m_quadCount(0),
											m_monoSpace(monoSpacing),
											m_maxWidth(maxWidth),
											m_wordWrap(wordWrap)
	{
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	BasicText::~BasicText()
	{

	}

	/////////////////////////////////////////////////////////////
	void BasicText::setAnchor(const Anchor &anchor)
	{
		m_anchor = anchor;
		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void BasicText::BasicText::setParent(Widget *parent)
	{
		m_parent = parent;
		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setText(const fm::String &text)
	{
		m_text = text;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	const fm::String &BasicText::getText() const
	{
		return m_text;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setFont(fg::Font *font)
	{
		m_font = font;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	fg::Font *BasicText::getFont() const
	{
		return m_font;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setColor(const fm::vec4 &clr)
	{
		m_clr = clr;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	fm::vec4 BasicText::getColor() const
	{
		return m_clr;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setCharSize(unsigned int charSize)
	{
		if (m_charSize != charSize)
		{
			m_charSize = charSize;
			buildVertices();
		}
	}

	/////////////////////////////////////////////////////////////
	unsigned int BasicText::getCharSize() const
	{
		return m_charSize;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setMonoSpacing(bool monoSpacing)
	{
		if (m_monoSpace != monoSpacing)
		{
			m_monoSpace = monoSpacing;
			buildVertices();
		}
	}

	/////////////////////////////////////////////////////////////
	bool BasicText::getMonoSpacing() const
	{
		return m_monoSpace;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setMaxWidth(float maxWidth)
	{
		if (m_maxWidth < maxWidth || m_maxWidth > maxWidth)
		{
			m_maxWidth = maxWidth;
			buildVertices();
		}
	}

	/////////////////////////////////////////////////////////////
	float BasicText::getMaxWidth() const
	{
		return m_maxWidth;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setWordWrap(bool wordWrap)
	{
		if (m_wordWrap != wordWrap)
		{
			m_wordWrap = wordWrap;
			buildVertices();
		}
	}

	/////////////////////////////////////////////////////////////
	bool BasicText::getWordWrap() const
	{
		return m_wordWrap;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::setSize(const fm::vec2 &size)
	{
		(void)size;
	}

	/////////////////////////////////////////////////////////////
	void BasicText::onDraw()
	{
		if (!m_tex)
			return;

		/// setup transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&m_transform.transpose()[0][0]);

		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&m_tex->getPixToUnitMatrix().transpose()[0][0]);


		/// enable states
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);


		m_tex->bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		m_vbo.bind();
		glVertexPointer(2,GL_FLOAT,sizeof(fm::vertex2f),(void*)fm::offsetOf(fm::vertex2f,pos));
		glTexCoordPointer(2,GL_FLOAT,sizeof(fm::vertex2f),(void*)fm::offsetOf(fm::vertex2f,texPos));
		glColorPointer(4,GL_FLOAT,sizeof(fm::vertex2f),(void*)fm::offsetOf(fm::vertex2f,clr));

		m_ibo.bind();
		glDrawElements(fg::Triangles,m_quadCount*6,GL_UNSIGNED_SHORT,fm::nullPtr);

		fg::Buffer::bind(fm::nullPtr,fg::ArrayBuffer);
		fg::Buffer::bind(fm::nullPtr,fg::IndexBuffer);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);


		/// disable states
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_BLEND);

		/// reset transformations
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&fm::mat4()[0][0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&fm::mat4()[0][0]);
	}

	/////////////////////////////////////////////////////////////
	void BasicText::onParentChange()
	{
		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	const std::string &BasicText::getRealType() const
	{
		return m_className;
	}
}
