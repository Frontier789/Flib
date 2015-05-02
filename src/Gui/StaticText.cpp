#include <FRONTIER/Gui/StaticText.hpp>
#include <FRONTIER/Graphics.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string StaticText::m_className = "StaticText";

	/////////////////////////////////////////////////////////////
	bool printable(char c)
	{
		return std::string("\n\r\t\b ").find(c) == std::string::npos;
	}
	
	void extendSize(fm::vec2 &size,fm::vec2 p)
	{
		if (p.x > size.w)
			size.w = p.x;
				
		if (p.y > size.h)
			size.h = p.y;
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::buildVertices()
	{
		/** clear vertices **/
		delete[] m_pts;  m_pts  = fm::nullPtr;
		delete[] m_tpts; m_tpts = fm::nullPtr;
		delete[] m_clrs; m_clrs = fm::nullPtr;
		delete[] m_inds; m_inds = fm::nullPtr;
		
		m_tex = fm::nullPtr;
		m_quadCount = 0;
		
		/** check if text is empty **/
		fm::Size textLength = m_text.length();
		
		if (!m_font || !textLength)
			return;
		
		/** count quads **/
		for (fm::Size i=0;i<textLength;i++)
			m_quadCount += printable(m_text[i]);
		
		/** alloc vertices **/
		m_pts  = new fm::vec2[m_quadCount*4];
		m_tpts = new fm::vec2[m_quadCount*4];
		m_clrs = new fm::vec4[m_quadCount*4];
		m_inds = new fm::Uint16[m_quadCount*6];
		
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
					fg::fg_log << "_1" << m_text << std::endl;
					cursor.x = 2;
					cursor.y += metrics.lineGap;
				}
				
				fm::vec2 p = cursor - glyph.leftdown*fm::vec2(0,1); 
				
				if (m_monoSpace)
					p.x += int(monoWidth/2 - glyph.size.w/2);
				else
					p.x += 1;
				
				Cxy(2,2)
					m_pts[index*4+x*2+y] = p+glyph.size*fm::vec2(x,y-1.0),
					m_tpts[index*4+x*2+y] = glyph.pos+glyph.size*fm::vec2(x,y),
					m_clrs[index*4+x*2+y] = m_clr;
				
				extendSize(m_size,m_pts[index*4+3]);

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
		
		setupPosition();
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setupPosition()
	{
		m_transform = fm::MATRIX::translation(getPosInRoot());
	}

	/////////////////////////////////////////////////////////////
	StaticText::StaticText(const std::string &name,
						   const Anchor &anchor,
						   const fm::vec2 &size,
						   Widget *parent,
						   const std::string &text,
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
											m_pts(fm::nullPtr),
											m_tpts(fm::nullPtr),
											m_clrs(fm::nullPtr),
											m_inds(fm::nullPtr),
											m_quadCount(0),
											m_monoSpace(monoSpacing),
											m_maxWidth(maxWidth),
											m_wordWrap(wordWrap)
	{
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	StaticText::~StaticText()
	{
		delete[] m_pts;
		delete[] m_tpts;
		delete[] m_clrs;
		delete[] m_inds;
	}

	/////////////////////////////////////////////////////////////
	void StaticText::setAnchor(const Anchor &anchor)
	{
		m_anchor = anchor;
		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void StaticText::StaticText::setParent(Widget *parent)
	{
		m_parent = parent;
		setupPosition();
	}
	
	
	/////////////////////////////////////////////////////////////
	void StaticText::setText(const std::string &text)
	{
		m_text = text;
		buildVertices();
	}
	
	/////////////////////////////////////////////////////////////
	const std::string &StaticText::getText() const
	{
		return m_text;
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setFont(fg::Font *font)
	{
		m_font = font;
		buildVertices();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font *StaticText::getFont() const
	{
		return m_font;
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setColor(const fm::vec4 &clr)
	{
		m_clr = clr;
		C(m_quadCount*4)
			m_clrs[i] = m_clr;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec4 StaticText::getColor() const
	{
		return m_clr;
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setCharSize(unsigned int charSize)
	{
		m_charSize = charSize;
		buildVertices();
	}
	
	/////////////////////////////////////////////////////////////
	unsigned int StaticText::getCharSize() const
	{
		return m_charSize;
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setMonoSpacing(bool monoSpacing)
	{
		m_monoSpace = monoSpacing;
		buildVertices();
	}
	
	/////////////////////////////////////////////////////////////
	bool StaticText::getMonoSpacing() const
	{
		return m_monoSpace;
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setMaxWidth(float maxWidth)
	{
		m_maxWidth = maxWidth;
		buildVertices();
	}
	
	/////////////////////////////////////////////////////////////
	float StaticText::getMaxWidth() const
	{
		return m_maxWidth;
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setWordWrap(bool wordWrap)
	{
		m_wordWrap = wordWrap;
		buildVertices();
	}
	
	/////////////////////////////////////////////////////////////
	bool StaticText::getWordWrap() const
	{
		return m_wordWrap;
	}

	/////////////////////////////////////////////////////////////
	void StaticText::setSize(const fm::vec2 &size)
	{
		(void)size;
	}

	/////////////////////////////////////////////////////////////
	void StaticText::onDraw()
	{
		if (!m_tex)
			return;
		
		/** setup transformations **/
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&m_transform.transpose()[0][0]);
		
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&m_tex->getPixToUnitMatrix().transpose()[0][0]);
		
		
		/** enable states **/
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_TEXTURE_2D);
		
		/** specify draw data **/
		glVertexPointer(2,GL_FLOAT,0,m_pts);
		glColorPointer(4,GL_FLOAT,0,m_clrs);
		glTexCoordPointer(2,GL_FLOAT,0,m_tpts);
		
		m_tex->bind();
		
		/** issue draw **/
		glDrawElements(fg::Triangles,m_quadCount*6,GL_UNSIGNED_SHORT,m_inds);
		
		
		/** disable states **/
		glDisable(GL_TEXTURE_2D);
		
		glDisable(GL_BLEND);
		
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		
		/** reset transformations **/
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&fm::mat4()[0][0]);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&fm::mat4()[0][0]);
	}

	/////////////////////////////////////////////////////////////
	void StaticText::onParentChange()
	{
		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	const std::string &StaticText::getRealType() const
	{
		return m_className;
	}
}
