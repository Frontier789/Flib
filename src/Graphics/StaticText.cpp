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
#include <FRONTIER/System/Vertex.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Graphics/Font.hpp>

#include <vector>

namespace fg
{
	/////////////////////////////////////////////////////////////
	void StaticText::buildVertices()
	{
		// simply handle empty string
		if (!m_text.length())
			m_boundings = fm::vec2(),
			m_indexCount = 0;
		else
		{
			// we need a font
			if (!m_font)
				return;

			std::vector<fm::vec2> poses;
			std::vector<fm::vec4> clrs;
			std::vector<fm::vec2> texPoses;
			std::vector<unsigned int> indices;

			m_font->setSize(m_characterSize);

			// reset some values
			m_boundings = fm::vec2();

			fm::Size vertexCount=0;

			// get metrics
			fg::Font::Metrics metrics = m_font->getMetrics();
			int height = metrics.maxH-metrics.minH;
			float width = m_font->getGlyph('w').size.w;
			fm::vec2 curPos(0,metrics.maxH);

			std::vector<fm::Size> rowWidths;

			// iterate through the text
			fm::Size length = m_text.length();
			for (fm::Size i=0;i<length;i++)
			{
				// register newline and reset the x position
				if (m_text[i]=='\n' || (m_text[i]=='\r' && (i==length-1 || m_text[i+1]!='\n')))
					rowWidths.push_back(curPos.x),
					curPos = fm::vec2(0,curPos.y+height);
				// simply advance
				else if (m_text[i]==' ' || m_text[i]=='\t')
					curPos += fm::vec2(m_text[i]==' ' ? width : width*4,0);
				else
				{
					fg::Glyph glyph = m_font->getGlyph(m_text[i]);
					if (!m_kerningEnabled)
						curPos.x+=(width-glyph.size.w)/2.f;

					poses.resize(vertexCount+4);
					clrs.resize(vertexCount+4);
					texPoses.resize(vertexCount+4);
					indices.resize(vertexCount/4*6+6);

					indices[vertexCount/4*6+0] = vertexCount+0;
					indices[vertexCount/4*6+1] = vertexCount+1;
					indices[vertexCount/4*6+2] = vertexCount+2;
					indices[vertexCount/4*6+3] = vertexCount+1;
					indices[vertexCount/4*6+4] = vertexCount+2;
					indices[vertexCount/4*6+5] = vertexCount+3;

					clrs[vertexCount+0] = fm::vec4(m_color)/255.f;
					clrs[vertexCount+1] = fm::vec4(m_color)/255.f;
					clrs[vertexCount+2] = fm::vec4(m_color)/255.f;
					clrs[vertexCount+3] = fm::vec4(m_color)/255.f;

					poses[vertexCount+0] = glyph.size*fm::vec2(0, 0)-glyph.leftdown+curPos;
					poses[vertexCount+1] = glyph.size*fm::vec2(1, 0)-glyph.leftdown+curPos;
					poses[vertexCount+2] = glyph.size*fm::vec2(0,-1)-glyph.leftdown+curPos;
					poses[vertexCount+3] = glyph.size*fm::vec2(1,-1)-glyph.leftdown+curPos;

					texPoses[vertexCount+0] = (glyph.pos+glyph.size*fm::vec2(0,1));
					texPoses[vertexCount+1] = (glyph.pos+glyph.size*fm::vec2(1,1));
					texPoses[vertexCount+2] = (glyph.pos+glyph.size*fm::vec2(0,0));
					texPoses[vertexCount+3] = (glyph.pos+glyph.size*fm::vec2(1,0));

					vertexCount+=4;
					if (m_kerningEnabled)
						curPos.x+=glyph.size.w+1;
					else
						curPos.x+=(width+glyph.size.w)/2.f;
				}
				if (m_boundings.w < curPos.x)
					m_boundings.w = curPos.x;
				if (m_boundings.h < curPos.y-metrics.minH)
					m_boundings.h = curPos.y-metrics.minH;
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
						poses[vertexCount+0].x += increase;
						poses[vertexCount+1].x += increase;
						poses[vertexCount+2].x += increase;
						poses[vertexCount+3].x += increase;
						vertexCount+=4;
					}
				}
			}

			m_indexCount = indices.size();
			m_vertexCount = poses.size();

			m_posBuf.setData(&poses[0],poses.size()*sizeof(fm::vec2));
			m_clrBuf.setData(&clrs[0],clrs.size()*sizeof(fm::vec4));
			m_texPosBuf.setData(&texPoses[0],texPoses.size()*sizeof(fm::vec2));
			m_indBuf.setData(&indices[0],indices.size()*sizeof(unsigned int));
		}
	}

	/////////////////////////////////////////////////////////////
	StaticText::StaticText() : m_color(Color::White),
							   m_align(StaticText::Align::Left),
							   m_font(NULL),
							   m_texture(NULL),
							   m_characterSize(64),
							   m_vertexCount(0),
							   m_indexCount(0),
							   m_indBuf(fg::IndexBuffer)
	{

	}

	/////////////////////////////////////////////////////////////
	StaticText::StaticText(const fg::Font &font,const std::string &text,const fg::Color &color,unsigned int characterSize,unsigned int align,bool enableKerning) : m_color(color),
																																								   m_text(text),
																																								   m_align(align),
																																								   m_kerningEnabled(enableKerning),
																																								   m_font(&font),
																																								   m_texture(NULL),
																																								   m_characterSize(characterSize),
																																								   m_vertexCount(0),
																																								   m_indexCount(0),
                                                                                                                                                                   m_indBuf(fg::IndexBuffer)
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
		std::vector<fm::vec4> clrs;
		clrs.resize(m_vertexCount,m_color);

		m_clrBuf.setData(&clrs[0],clrs.size()*sizeof(fm::vec4));
	}

	/////////////////////////////////////////////////////////////
	void StaticText::enableKerning(bool enable)
	{
		m_kerningEnabled = enable;
		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	void StaticText::draw(fg::RenderStates states) const
	{
		states.texture = m_texture;
		states.transform *= getTransform();

		fg::draw(fg::Attr<fm::vec2>(m_posBuf),
				 fg::Attr<fm::vec4>(m_clrBuf),
				 fg::Attr<fm::vec2>(m_texPosBuf),
				 m_indexCount,fg::Triangles,states,fg::IndexPointer(0,sizeof(unsigned int),&m_indBuf));
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
