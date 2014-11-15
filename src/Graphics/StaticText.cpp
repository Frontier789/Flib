////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/StaticText.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Vertex.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	void StaticText::buildVertices()
	{
		// simply handle empty string
		if (!m_text.length())
			m_boundings = fm::vec2(),
			m_vertices.resize(0),
			m_indices.resize(0);
		else
		{
			// we need a font
			if (!m_font)
				return;
			
			m_font->setSize(m_characterSize);
			
			// reset some values
			m_boundings = fm::vec2();

			m_vertices.resize(0);
			m_indices.resize(0);
			fm::Size vertexCount=0;
			
			// get metrics
			fg::Font::Metrics metrics = m_font->getMetrics();
			int height = metrics.maxH-metrics.minH;
			float width = m_font->getGlyph('w').size.w;
			fm::vec2 curPos(0,height*-1);

			std::vector<fm::Size> rowWidths;
			
			// iterate through the text
			fm::Size length = m_text.length();
			for (fm::Size i=0;i<length;i++)
			{
				// register newline and reset the x position
				if (m_text[i]=='\n' || (m_text[i]=='\r' && (i==length-1 || m_text[i+1]!='\n')))
					rowWidths.push_back(curPos.x),
					curPos = fm::vec2(0,curPos.y-=height);
				// simply advance 
				else if (m_text[i]==' ')
					curPos += fm::vec2(width,0);
				else
				{
					fg::Glyph glyph = m_font->getGlyph(m_text[i]);
					if (!m_kerningEnabled)
						curPos.x+=(width-glyph.size.w)/2.f;
					
					m_vertices.resize(vertexCount+4);
					m_indices.resize(vertexCount/4*6+6);

					m_indices[vertexCount/4*6+0] = vertexCount+0;
					m_indices[vertexCount/4*6+1] = vertexCount+1;
					m_indices[vertexCount/4*6+2] = vertexCount+2;
					m_indices[vertexCount/4*6+3] = vertexCount+1;
					m_indices[vertexCount/4*6+4] = vertexCount+2;
					m_indices[vertexCount/4*6+5] = vertexCount+3;

					m_vertices[vertexCount+0].clr = m_color;
					m_vertices[vertexCount+1].clr = m_color;
					m_vertices[vertexCount+2].clr = m_color;
					m_vertices[vertexCount+3].clr = m_color;

					m_vertices[vertexCount+0].pos = glyph.size*fm::vec2(0,1)+glyph.leftdown+curPos;
					m_vertices[vertexCount+1].pos = glyph.size*fm::vec2(1,1)+glyph.leftdown+curPos;
					m_vertices[vertexCount+2].pos = glyph.size*fm::vec2(0,0)+glyph.leftdown+curPos;
					m_vertices[vertexCount+3].pos = glyph.size*fm::vec2(1,0)+glyph.leftdown+curPos;

					m_vertices[vertexCount+0].texPos = (glyph.pos+glyph.size*fm::vec2(0,0));
					m_vertices[vertexCount+1].texPos = (glyph.pos+glyph.size*fm::vec2(1,0));
					m_vertices[vertexCount+2].texPos = (glyph.pos+glyph.size*fm::vec2(0,1));
					m_vertices[vertexCount+3].texPos = (glyph.pos+glyph.size*fm::vec2(1,1));

					vertexCount+=4;
					if (m_kerningEnabled)
						curPos.x+=glyph.size.w+1;
					else
						curPos.x+=(width+glyph.size.w)/2.f;
				}
				if (m_boundings.w < curPos.x)
					m_boundings.w = curPos.x;
				if (m_boundings.h > curPos.y)
					m_boundings.h = curPos.y;
			}
			rowWidths.push_back(curPos.x);
			m_texture = &m_font->getTexture();

			if (m_align != Align::Left)
			{
				float delta = (m_align==Align::Center ? .5 : 1.0);
				fm::Size rowN=0;
				float rowWidth = rowWidths[0];
				vertexCount=0;
				for (fm::Size i=0;i<length;i++)
				{
					if (m_text[i]=='\n')
						rowN++,
						rowWidth = rowWidths[rowN];
					else if (m_text[i]==' ');
					else
					{
						float increase = (m_boundings.w-rowWidth)*delta;
						m_vertices[vertexCount+0].pos.x += increase;
						m_vertices[vertexCount+1].pos.x += increase;
						m_vertices[vertexCount+2].pos.x += increase;
						m_vertices[vertexCount+3].pos.x += increase;
						vertexCount+=4;
					}
				}
			}

		}
	}

	/////////////////////////////////////////////////////////////
	StaticText::StaticText() : m_font(NULL),
							   m_texture(NULL),
							   m_align(StaticText::Align::Left),
							   m_characterSize(64)
	{

	}

	/////////////////////////////////////////////////////////////
	StaticText::StaticText(const fg::Font &font,const std::string &text,const fg::Color &color,unsigned int characterSize,unsigned int align,bool enableKerning) : m_font(&font),
																																								   m_texture(NULL),
																																								   m_text(text),
																																								   m_color(color),
																																								   m_align(align),
																																								   m_characterSize(characterSize),
																																								   m_kerningEnabled(enableKerning)

	{
		buildVertices();
	}
	
	/////////////////////////////////////////////////////////////
	void StaticText::setText(const std::string &text)
	{
		m_text = text;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	void StaticText::setFont(const fg::Font &font)
	{
		m_font = &font;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	void StaticText::setColor(const fg::Color &color)
	{
		m_color = color;
		fm::Size count = m_vertices.size();
		for (fm::Size i=0;i<count;i++)
			m_vertices[i].clr = m_color;
	}

	/////////////////////////////////////////////////////////////
	void StaticText::enableKerning(bool enable)
	{
		m_kerningEnabled = enable;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	void StaticText::draw(const fg::Texture *texture,const fg::Shader *shader) const
	{
		// unused
		(void)texture;
		
		fg::draw(&m_vertices[0],m_indices.size(),fg::Triangles,m_texture,shader,getTransform(),&m_indices[0]);
	}

	/////////////////////////////////////////////////////////////
	const fm::vec2 &StaticText::getBoundings() const
	{
		return m_boundings;
	}

	/////////////////////////////////////////////////////////////
	void StaticText::setAlign(unsigned int align)
	{
		m_align = align;
		buildVertices();
	}


	/////////////////////////////////////////////////////////////
	void StaticText::setCharacterSize(unsigned int size)
	{
		m_characterSize = size;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	const std::string &StaticText::getText() const
	{
		return m_text;
	}

	/////////////////////////////////////////////////////////////
	const fg::Font *StaticText::getFont() const
	{
		return m_font;
	}

	/////////////////////////////////////////////////////////////
	const fg::Color &StaticText::getColor() const
	{
		return m_color;
	}

	/////////////////////////////////////////////////////////////
	unsigned int StaticText::getAlign() const
	{
		return m_align;
	}

	/////////////////////////////////////////////////////////////
	unsigned int StaticText::getCharacterSize() const
	{
		return m_characterSize;
	}

	/////////////////////////////////////////////////////////////
	bool StaticText::isKerningEnabled() const
	{
		return m_kerningEnabled;
	}
}