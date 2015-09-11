#include <FRONTIER/System/macros/OFFSETOF.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/Gui/BasicControl.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/Util/Clipboard.hpp>
#include <FRONTIER/System/Vertex.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/OpenGL.hpp>

#include <cmath>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string BasicControl::m_className = "BasicControl";

	/////////////////////////////////////////////////////////////
	BasicControl::TextPos::TextPos(fm::Size line,fm::Size pos) : line(line),
																 pos(pos)
	{

	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::TextPos::operator<(const TextPos &tp) const
	{
		return (line < tp.line) || (line == tp.line && pos < tp.pos);
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::TextPos::operator>(const TextPos &tp) const
	{
		return (line > tp.line) || (line == tp.line && pos > tp.pos);
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::TextPos::operator<=(const TextPos &tp) const
	{
		return (line < tp.line) || (line == tp.line && pos <= tp.pos);
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::TextPos::operator>=(const TextPos &tp) const
	{
		return (line > tp.line) || (line == tp.line && pos >= tp.pos);
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::TextPos::operator==(const TextPos &tp) const
	{
		return line == tp.line && pos == tp.pos;
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::TextPos::operator!=(const TextPos &tp) const
	{
		return line != tp.line || pos != tp.pos;
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::TextPos::isBetween(const TextPos &p1,const TextPos &p2) const
	{
		if ((*this)==p1 || (*this)==p2)
			return true;

		if (p1<p2)
			return (*this)<p2 && p1<(*this);

		return (*this)<p1 && p2<(*this);
	}

	/////////////////////////////////////////////////////////////
	BasicControl::Caret::Caret(const TextPos &tp,
							   float savedWidth,
							   const TextPos &sel) : tp(tp),
													 savedWidth(savedWidth),
													 sel(sel)
	{

	}

	/////////////////////////////////////////////////////////////
	BasicControl::CharColor::CharColor(fm::vec4 clr,fm::vec4 bkclr) : clr(clr),
																	  bkclr(bkclr)
	{

	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setupMemberData()
	{
		m_vertexCount = 0;
		m_vertexCountBkg = 0;

		// setup font, texture and other members
		m_font->setCharacterSize(m_charSize);
		m_tex = &m_font->getTexture();

		m_metrics = m_font->getMetrics();

		// count index count
		C(m_lines.size())
		{
			float w = 0;

			if ((i+1)*m_metrics.lineGap > getViewPos().y && i*m_metrics.lineGap < m_size.y+getViewPos().y)
				Cx(m_lines[i].size())
				{
					w += getCharWidth(m_lines[i][x]);

					if (w > getViewPos().x)
					{
						m_vertexCount += (m_lines[i][x]!=' ' && m_lines[i][x]!='\t')*4,
						m_vertexCountBkg += 4;
					}

					if (w > m_size.x+getViewPos().x)
						break;
				}
		}
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::buildVertices()
	{
		if (!m_font)
			return;

		setupMemberData();

		// allocate memory
		fm::vertex2f *verts = new fm::vertex2f[m_vertexCount];
		fm::vert2f *vertsBkg = new fm::vert2f[m_vertexCountBkg];
		fm::Uint32 *inds = new fm::Uint32[m_vertexCount/4*6];
		fm::Uint32 *indsBkg = new fm::Uint32[m_vertexCountBkg/4*6];

		// counter variables
		fm::vec2 cursor(0,m_metrics.maxH);

		fm::Uint32 index=0;
		fm::Uint32 indexBkg=0;

		// for all lines
		C(m_lines.size())
		{
			// for all characters in the current line
			if ((i+1)*m_metrics.lineGap > getViewPos().y && i*m_metrics.lineGap < m_size.y+getViewPos().y)
				Cf(m_lines[i].size(),linex)
				{
					fg::Glyph glyph;
					float xadvance = getCharWidth(m_lines[i][linex],&glyph);

					if (cursor.x+xadvance > getViewPos().x)
					{
						// build the quad of the foreground if there is going to be any
						if (m_lines[i][linex]!=' ' && m_lines[i][linex]!='\t')
						{
							// get the lower left corner of the quad
							fm::vec2 p = cursor - glyph.leftdown*fm::vec2(0,1) + fm::vec2i((xadvance-glyph.size.w)/2,0);

							// build quad
							Cxy(2,2)
								verts[index*4+x*2+y].pos =  p + glyph.size * fm::vec2(x,y-1.0),
								verts[index*4+x*2+y].clr =  m_charColors[i][linex].clr,
								verts[index*4+x*2+y].texPos =  glyph.pos + glyph.size * fm::vec2(x,y);

							// calculate indices
							Cx(3)
								inds[index*6+x] = index*4+x,
								inds[index*6+3+x] = index*4+x+1;

							index ++;
						}

						// build the background of the quad
						Cxy(2,2)
							vertsBkg[indexBkg*4+x*2+y].pos = cursor - m_metrics.maxH*fm::vec2(0,1) + fm::vec2(xadvance,m_metrics.lineGap)*fm::vec2(x,y),
							vertsBkg[indexBkg*4+x*2+y].clr = m_charColors[i][linex].bkclr;

							// calculate background indices
						Cx(3)
							indsBkg[indexBkg*6+x] = indexBkg*4+x,
							indsBkg[indexBkg*6+3+x] = indexBkg*4+x+1;

						indexBkg ++;
					}

					cursor.x += xadvance;

					if (cursor.x > m_size.x+getViewPos().x)
						break;
				}

			// advance cursor to the next line
			cursor.y += m_metrics.lineGap;
			cursor.x = 0;
		}

		m_vbo.setData(verts,sizeof(*verts)*m_vertexCount);
		m_ibo.setData(inds,sizeof(*inds)*m_vertexCount/4*6);
		m_vboBkg.setData(vertsBkg,sizeof(*vertsBkg)*m_vertexCountBkg);
		m_iboBkg.setData(indsBkg,sizeof(*indsBkg)*m_vertexCountBkg/4*6);

		delete[] verts;
		delete[] inds;
		delete[] vertsBkg;
		delete[] indsBkg;

		// correct position
		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setupPosition()
	{
		m_transform = fm::MATRIX::translation(fm::vec2i(getPosInRoot()-getViewPos()));
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::recalcCarets()
	{
		if (!m_font)
			return;

		m_font->setCharacterSize(m_charSize);

		m_vertexCountCaret = 0;
		m_vertexCountCaretBkg = 0;

		// count vertices
		C(m_carets.size())
			m_vertexCountCaret += 2,
			m_vertexCountCaretBkg += (fm::math::abs(int(m_carets[i].tp.line) - int(m_carets[i].sel.line))+1)*4;

		fm::vert2f *verts = new fm::vert2f[m_vertexCountCaret + m_vertexCountCaretBkg];
		fm::vert2f *vertsBkg = verts + m_vertexCountCaret;
		fm::Uint32 *inds = new fm::Uint32[m_vertexCountCaret + m_vertexCountCaretBkg/4*6];
		fm::Uint32 *indsBkg = inds + m_vertexCountCaret;

		// calculate caret part
		C(m_carets.size())
		{
			fm::vec2 pixPos;
			pixPos.y = m_carets[i].tp.line*m_metrics.lineGap;

			Cx(m_carets[i].tp.pos)
				pixPos.x += getCharWidth(m_lines[m_carets[i].tp.line][x]);

			fm::vec2 p1 = pixPos+fm::vec2(0,m_metrics.lineGap);
			fm::vec2 p2 = pixPos;
			if (m_insertMode)
			{
				p2 += fm::vec2(0,m_metrics.lineGap);

				if (m_carets[i].tp.pos < m_lines[m_carets[i].tp.line].size())
					p2.x += getCharWidth(m_lines[m_carets[i].tp.line][m_carets[i].tp.pos]);
				else
					p2.x += getCharWidth(' ');
			}

			verts[i*2+0].pos = p1;
			verts[i*2+0].clr = fm::vec4(1,0,0,1);

			verts[i*2+1].pos = p2;
			verts[i*2+1].clr = fm::vec4(1,0,0,1);

			inds[i*2+0] = i*2+0;
			inds[i*2+1] = i*2+1;
		}

		fm::Size offset = 0;

		// calculate bkg part
		C(m_carets.size())
		{
			TextPos tmin = fm::math::min(m_carets[i].tp,m_carets[i].sel);
			TextPos tmax = fm::math::max(m_carets[i].tp,m_carets[i].sel);

			while (tmin.line <= tmax.line)
			{
				fm::vec2 pixPosF,pixPosT;
				pixPosF.y = tmin.line*m_metrics.lineGap;
				pixPosT.y = tmin.line*m_metrics.lineGap + m_metrics.lineGap;

				Cx(tmin.pos)
					pixPosF.x += getCharWidth(m_lines[tmin.line][x]);

				pixPosT.x = pixPosF.x;
				Cx(tmin.line == tmax.line ? tmax.pos-tmin.pos : m_lines[tmin.line].size()-tmin.pos)
					pixPosT.x += getCharWidth(m_lines[tmin.line][tmin.pos+x]);

				if (tmin.line < tmax.line)
					pixPosT.x += getCharWidth(' ');

				vertsBkg[offset + 0].pos = pixPosF;
				vertsBkg[offset + 0].clr = m_selectColor;
				vertsBkg[offset + 1].pos = fm::vec2(pixPosT.x,pixPosF.y);
				vertsBkg[offset + 1].clr = m_selectColor;
				vertsBkg[offset + 2].pos = pixPosT;
				vertsBkg[offset + 2].clr = m_selectColor;
				vertsBkg[offset + 3].pos = fm::vec2(pixPosF.x,pixPosT.y);
				vertsBkg[offset + 3].clr = m_selectColor;

				C(3)
					indsBkg[offset/4*6 + i] = offset+i+m_vertexCountCaret;
				C(3)
					indsBkg[offset/4*6 + i + 3] = offset+(i+2)%4+m_vertexCountCaret;

				tmin.line++;
				tmin.pos=0;
				offset += 4;
			}
		}

		m_caretVbo.setData(verts,sizeof(*verts)*(m_vertexCountCaret + m_vertexCountCaretBkg));
		m_caretIbo.setData(inds,sizeof(*inds)*(m_vertexCountCaret + m_vertexCountCaretBkg/4*6));

		delete[] verts;
		delete[] inds;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onCaretColorUpdate()
	{
		fm::vert2f *ptr = (fm::vert2f *)m_caretVbo.map();

		fm::Time t = m_caretClk.getTime();
		fm::Uint64 ms = t.asMilliseconds();

		ms %= 1600;

		fm::vec4 c(fm::vec3(m_caretColor),/* ms<700 ? ms/700.0 : (1600.0-ms)/900.0 */ std::sin(ms/1600.0*3.14159265358979));

		C(m_carets.size()*2)
			ptr[i].clr = c;

		m_caretVbo.unMap();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::uniquifyCarets()
	{
		for (fm::Size x=0;x<m_carets.size();x++)
		{
			for (fm::Size y=0;y<m_carets.size();y++)
			{
				if (x != y)
				if (m_carets[y].tp.isBetween(m_carets[x].tp,m_carets[x].sel) ||
					m_carets[x].tp.isBetween(m_carets[y].tp,m_carets[y].sel))
				{
					m_needRecalcCarets  = true;

					TextPos mn = m_carets[x].tp;
					TextPos mx = m_carets[x].tp;

					if (m_carets[x].sel < mn) mn = m_carets[x].sel;
					if (mx < m_carets[x].sel) mx = m_carets[x].sel;
					if (m_carets[y].tp  < mn) mn = m_carets[y].tp;
					if (mx < m_carets[y].tp ) mx = m_carets[y].tp;
					if (m_carets[y].sel < mn) mn = m_carets[y].sel;
					if (mx < m_carets[y].sel) mx = m_carets[y].sel;

					m_carets.erase(m_carets.begin()+y);

					if (m_carets[x].tp < m_carets[x].sel)
						m_carets[x].tp  = mn,
						m_carets[x].sel = mx;
					else
						m_carets[x].tp  = mx,
						m_carets[x].sel = mn;

					y = 0;
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////
	float BasicControl::getCharWidth(fm::Uint32 cp,fg::Glyph *retGlyph)
	{
		if (!m_font)
			return 0;

		if (cp==' ')
			return m_font->getGlyph('w').size.w+1;

		if (cp=='\t')
			return m_font->getGlyph('w').size.w*4+1;

		fg::Glyph glyph = m_font->getGlyph(cp);

		if (retGlyph)
			*retGlyph = glyph;

		return m_monoSpace ? m_font->getGlyph('w').size.w+1 : glyph.size.w+2;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::addText(Caret &caret,const fm::String &str)
	{
		if (!getEditable()) return;

		if (str.find("\n") != fm::String::npos)
		{
			fm::String nextStr;

			C(str.size())
			{
				if (str[i] != '\n')
					nextStr += str[i];
				else
					addText(caret,nextStr),
					newLine(caret),
					nextStr = fm::String();
			}

			if (nextStr.size())
				addText(caret,nextStr);

			return;
		}

		m_needRecalcCarets  = true;
		m_needBuildVertices = true;
		m_needSetViewCaret  = true;

		delText(caret,0);

		// get line
		std::deque<fm::String>::iterator line = m_lines.begin()+caret.tp.line;
		std::deque<std::deque<CharColor> >::iterator colorLine = m_charColors.begin()+caret.tp.line;

		// insert string
		C(str.size())
		{
			/// if (!m_font->hasGlyph(str[i])) fg::fg_log << "unknown codepoint "<<str[i]<<std::endl;

			line->insert(caret.tp.pos+i,str[i]);
			colorLine->insert(colorLine->begin()+caret.tp.pos+i,CharColor(m_textColor,m_bckColor));
		}

		// advance cursors
		C(m_carets.size())
		{
			if (m_carets[i].tp.line == caret.tp.line && m_carets[i].tp.pos > caret.tp.pos)
				moveCaretPos(m_carets[i],str.size());
		}

		moveCaretPos(caret,str.size());
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::newLine(Caret &caret)
	{
		if (!getEditable()) return;

		m_needRecalcCarets  = true;
		m_needBuildVertices = true;
		m_needSetViewCaret  = true;

		delText(caret,0);

		// get line
		std::deque<fm::String>::iterator line = m_lines.begin()+caret.tp.line;
		std::deque<std::deque<CharColor> >::iterator lineColor = m_charColors.begin()+caret.tp.line;

		// copy the part of the line that is carried to the new line
		fm::String newLineStr(line->begin()+caret.tp.pos,line->end());
		std::deque<CharColor> newLineColors(lineColor->begin()+caret.tp.pos,lineColor->end());

		// insert new line with its content
		m_lines.insert(m_lines.begin()+caret.tp.line+1,newLineStr);
		m_charColors.insert(m_charColors.begin()+caret.tp.line+1,newLineColors);

		// erase copied content from the previous line
		m_lines[caret.tp.line].erase(caret.tp.pos,fm::String::npos);
		m_charColors[caret.tp.line].erase(m_charColors[caret.tp.line].begin()+caret.tp.pos,m_charColors[caret.tp.line].end());

		// advance carets
		C(m_carets.size())
		{
			if (m_carets[i].tp.line > caret.tp.line)
				m_carets[i].tp.line++,
				m_carets[i].sel.line++;

			if (m_carets[i].tp.line == caret.tp.line && m_carets[i].tp.pos > caret.tp.pos)
				setCaret(m_carets[i],m_carets[i].tp.line+1,m_carets[i].tp.pos-caret.tp.pos,false);
		}

		setCaret(caret,caret.tp.line+1,0);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::delText(Caret &caret,int amount)
	{
		if (!getEditable()) return;

		m_needRecalcCarets  = true;
		m_needBuildVertices = true;
		m_needSetViewCaret  = true;

		if (caret.tp != caret.sel)
			amount = charBetween(caret.tp,caret.sel);

		std::deque<fm::String >::iterator line = m_lines.begin()+caret.tp.line;
		std::deque<std::deque<CharColor> >::iterator lineColor = m_charColors.begin()+caret.tp.line;

		if (amount < 0)
		{
			amount *= -1;
			while (amount)
			{
				int delta = fm::math::min<int>(amount,caret.tp.pos);

				line->erase(caret.tp.pos-delta,delta);
				lineColor->erase(lineColor->begin()+caret.tp.pos-delta,lineColor->begin()+caret.tp.pos);

				amount -= delta;

				C(m_carets.size())
					if (m_carets[i].tp.line == caret.tp.line && m_carets[i].tp.pos > caret.tp.pos)
						setCaret(m_carets[i],m_carets[i].tp.line,m_carets[i].tp.pos-delta,false);

				caret.tp.pos -= delta;

				if (caret.tp.line == 0)
					amount = 0;

				if (amount)
				{
					caret.tp.line--;
					caret.tp.pos = (line-1)->size();
					amount--;

					(line-1)->insert((line-1)->size(),*line);
					(lineColor-1)->insert((lineColor-1)->end(),lineColor->begin(),lineColor->end());

					m_lines.erase(line);
					m_charColors.erase(lineColor);

					line = m_lines.begin()+caret.tp.line;
					lineColor = m_charColors.begin()+caret.tp.line;

					C(m_carets.size())
					{
						if (m_carets[i].tp.line == caret.tp.line+1)
							setCaret(m_carets[i],m_carets[i].tp.line-1,m_carets[i].tp.pos+caret.tp.pos,false);

						if (m_carets[i].tp.line > caret.tp.line+1)
							m_carets[i].tp.line--,
							m_carets[i].sel.line--;
					}
				}
			}

			setCaret(caret,caret.tp,true);
		}
		if (amount > 0)
		{
			while (amount)
			{
				int delta = fm::math::min<int>(amount,line->size()-caret.tp.pos);

				line->erase(caret.tp.pos,delta);
				lineColor->erase(lineColor->begin()+caret.tp.pos,lineColor->begin()+caret.tp.pos+delta);

				amount -= delta;

				C(m_carets.size())
					if (m_carets[i].tp.line == caret.tp.line && m_carets[i].tp.pos > caret.tp.pos)
						setCaret(m_carets[i],m_carets[i].tp.line,m_carets[i].tp.pos-delta,false);

				if (caret.tp.line == m_lines.size()-1)
					amount = 0;

				if (amount)
				{
					amount--;

					line->insert(line->size(),*(line+1));
					lineColor->insert(lineColor->end(),(lineColor+1)->begin(),(lineColor+1)->end());

					m_lines.erase(line+1);
					m_charColors.erase(lineColor+1);

					line = m_lines.begin()+caret.tp.line;
					lineColor = m_charColors.begin()+caret.tp.line;

					C(m_carets.size())
					{
						if (m_carets[i].tp.line == caret.tp.line+1)
							setCaret(m_carets[i],m_carets[i].tp.line-1,m_carets[i].tp.pos+caret.tp.pos,false);

						if (m_carets[i].tp.line > caret.tp.line+1)
							m_carets[i].tp.line--,
							m_carets[i].sel.line--;
					}
				}
			}

			setCaret(caret,caret.tp,true);
		}
	}

	/////////////////////////////////////////////////////////////
	int BasicControl::getCharType(fm::Uint32 cp)
	{
		if (fm::String(".\\(){}[]<>+-*=/&|@%^~\'\"?!$#:;,").find(cp) != fm::String::npos)
			return 1;

		if (fm::String(" \t\n").find(cp) != fm::String::npos)
			return 2;

		return 3;
	}

	/////////////////////////////////////////////////////////////
	fm::Size BasicControl::getWordLength(const TextPos &tp,bool left,bool includeNewLine)
	{
		std::deque<fm::String >::iterator line = m_lines.begin()+tp.line;

		if (tp.pos == 0 && left)
		{
			if (tp.line == 0 || !includeNewLine)
				return 0;
			return 1;
		}

		int type;
        fm::Size ret = 0;
        fm::Size pos = tp.pos;

		if (tp.pos == line->size() && !left)
		{
			if (tp.line+1 == m_lines.size())
				return 0;

			type = getCharType(' ');
			pos = 0;
			line ++;
			ret ++;

			if (getCharType((*line)[0]) != type)
				return ret;
		}
		else
			type = getCharType((*line)[pos+(left?-1:0)]);

		do
		{
			ret++;
			pos += (left ? -1 : 1);

			if (pos == 0)
			{
				if (line == m_lines.begin())
					return ret;

				if (type == getCharType(' ') && includeNewLine)
					return ret+1;

				return ret;
			}

			if (pos == line->size())
				return ret;
		}
		while (getCharType((*line)[pos+(left?-1:0)]) == type);

		return ret;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setCaret(Caret &caret,const TextPos &tp,bool resetSel)
	{
		if (!resetSel && caret.tp == tp) return;

		m_needRecalcCarets = true;
		m_needSetViewCaret = true;

		if (resetSel)
			caret.sel = tp;
		else
			caret.sel.line += tp.line-caret.tp.line,
			caret.sel.pos  += tp.pos -caret.tp.pos;

		caret.tp = tp;
		caret.savedWidth = pixPosFromTextPos(tp).x;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setCaret(Caret &caret,fm::Size line,fm::Size pos,bool resetSel)
	{
		setCaret(caret,TextPos(line,pos),resetSel);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::addCaret(TextPos tp)
	{
		m_needRecalcCarets = true;
		m_needSetViewCaret = true;

		if (tp.line >= m_lines.size())
			tp.line  = m_lines.size();

		if (tp.pos > m_lines[tp.line].size())
			tp.pos = m_lines[tp.line].size();

		m_carets.push_back(Caret(tp,pixPosFromTextPos(tp).x,tp));
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::addCaret(fm::Size line,fm::Size pos)
	{
		addCaret(TextPos(line,pos));
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::delCaret(fm::Size caretIndex)
	{
		m_needRecalcCarets = true;

		m_carets.erase(m_carets.begin() + caretIndex);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::moveCaretPos(Caret &caret,int dpos,bool resetSel)
	{
		m_needRecalcCarets = true;
		m_needSetViewCaret = true;

		TextPos tp = caret.tp;

		if (resetSel && caret.tp != caret.sel)
		{
			if (caret.tp < caret.sel)
			{
				if (dpos < 0)
					tp = caret.tp;
				else
					tp = caret.sel;
			}
			else
			{
				if (dpos < 0)
					tp = caret.sel;
				else
					tp = caret.tp;
			}
		}
		else
		{
			if (dpos < 0)
			{
				dpos *= -1;

				while (dpos)
				{
					int delta = fm::math::min<int>(dpos,tp.pos);

					tp.pos -= delta;
					dpos -= delta;

					if (tp.line == 0)
						dpos = 0;

					if (dpos)
						tp.line --,
						tp.pos = m_lines[tp.line].size(),
						dpos--;
				}
			}
			else if (dpos > 0)
			{
				while (dpos)
				{
					int delta = fm::math::min<int>(dpos,m_lines[tp.line].size()-tp.pos);

					tp.pos += delta;
					dpos -= delta;

					if (tp.line == m_lines.size()-1)
						dpos = 0;

					if (dpos)
						tp.line ++,
						tp.pos = 0,
						dpos--;
				}
			}
		}

		if (resetSel)
			caret.sel = tp;


		caret.tp = tp;
		caret.savedWidth = pixPosFromTextPos(tp).x;

	}

	/////////////////////////////////////////////////////////////
	void BasicControl::moveCaretLine(Caret &caret,int dline,bool resetSel)
	{
		m_needRecalcCarets = true;
		m_needSetViewCaret = true;

		TextPos tp = caret.tp;

		if (dline < 0 && tp.line < fm::Size(-dline))
			tp.line = 0,
			dline = 0;

		if (dline > 0 && tp.line+dline >= m_lines.size())
			tp.line = m_lines.size()-1,
			dline = 0;

		tp.line += dline;

		if (resetSel)
			caret.sel.line = tp.line,
			caret.sel.pos  = posFromPixPos(fm::vec2(caret.savedWidth,tp.line*m_metrics.lineGap));

		caret.tp.line = tp.line;
		caret.tp.pos  = posFromPixPos(fm::vec2(caret.savedWidth,tp.line*m_metrics.lineGap));
	}

	/////////////////////////////////////////////////////////////
	fm::String BasicControl::getLeadingWhiteSpaces(fm::Size lineIndex)
	{
		fm::String ret;

		std::deque<fm::String>::iterator line = m_lines.begin()+lineIndex;

		C(line->size())
		{
			if ( (*line)[i] == ' ' || (*line)[i] == '\t' )
				ret += (*line)[i];
			else
				break;
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
	fm::Size BasicControl::lineFromPixPos(fm::vec2 pixPos)
	{
		if (pixPos.y < 0)
			pixPos.y = 0;

		if (pixPos.y >= m_lines.size()*m_metrics.lineGap)
			pixPos.y  = m_lines.size()*m_metrics.lineGap-1;

		return pixPos.y / m_metrics.lineGap;
	}

	/////////////////////////////////////////////////////////////
	fm::Size BasicControl::posFromPixPos(fm::vec2 pixPos)
	{
		if (pixPos.x < 0)
			pixPos.x = 0;

		if (pixPos.y > getViewPos().h+getSize().h/*  m_lines.size()*m_metrics.lineGap*/)
			return m_lines.back().size();

		std::deque<fm::String >::iterator linei = m_lines.begin() + lineFromPixPos(pixPos);

		float curw = 0;
		fm::Size ret = 0;
		m_font->setCharacterSize(m_charSize);

		while (ret<linei->size())
		{
			float w = getCharWidth((*linei)[ret]);

			if (curw <= pixPos.x && curw+w >= pixPos.x)
			{
				float inCharP = pixPos.x - curw;

				if (inCharP > w/2)
					ret++;

				break;
			}

			curw += w;
			ret ++;
		}

		return ret;
	}

	/////////////////////////////////////////////////////////////
	BasicControl::TextPos BasicControl::textPosFromPixPos(fm::vec2 pixPos)
	{
		return TextPos(lineFromPixPos(pixPos),posFromPixPos(pixPos));
	}

	/////////////////////////////////////////////////////////////
	fm::vec2 BasicControl::pixPosFromTextPos(const TextPos &tp)
	{
		if (m_font)
			m_font->setCharacterSize(m_charSize);

		fm::vec2 ret;

		ret.y = tp.line*m_metrics.lineGap;

		C(fm::math::min(tp.pos,m_lines[tp.line].size()))
			ret.x += getCharWidth(m_lines[tp.line][i]);

		return ret;
	}

	/////////////////////////////////////////////////////////////
	int BasicControl::charBetween(const TextPos &tp1,const TextPos &tp2)
	{
		TextPos tpMin;
		TextPos tpMax;

		if (tp1 < tp2)
			tpMin = tp1,
			tpMax = tp2;
		else
			tpMin = tp2,
			tpMax = tp1;

		int ret = 0;

		while (tpMin.line < tpMax.line)
		{
			ret += m_lines[tpMin.line].size()-tpMin.pos;
			tpMin.pos = 0;

			tpMin.line++;
			ret++; // newline
		}

		ret += tpMax.pos-tpMin.pos;

		if (tp2 < tp1)
		   ret *= -1;

		return ret;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setViewCaret()
	{
		Caret *closest = &m_carets.back();
/*
		fm::Size viewL = (getViewPos().y + getSize().h/2)/m_metrics.lineGap;

		C(m_carets.size())
			if (fm::math::abs(closest->tp.pos-int(viewL)) > fm::math::abs(m_carets[i].tp.pos-int(viewL)))
				closest = &m_carets[i];
*/
		fm::vec2 pixDelta;
		fm::vec2 closePix = pixPosFromTextPos(closest->tp)-getViewPos()+fm::vec2(0,m_metrics.lineGap);
		fm::vec2 s = getSize();
        if (closePix.x > s.w*.9) pixDelta.x += closePix.x-s.w*.9;
        if (closePix.x < s.w*.1) pixDelta.x += closePix.x-s.w*.1;
        if (closePix.y > s.h*.9) pixDelta.y += closePix.y-s.h*.9;
        if (closePix.y < s.h*.1) pixDelta.y += closePix.y-s.h*.1;

        setViewPos(getViewPos()+pixDelta);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setInsertMode(bool insertMode)
	{
		if (m_insertMode != insertMode)
		{
			m_insertMode = insertMode;
			m_needRecalcCarets = true;
		}
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::getInsertMode()
	{
		return m_insertMode;
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::handleShortcut(fw::KeyboardEvent key)
	{
		/* CTRL + C */
		if (key.code == fw::Keyboard::C && key.ctrl && !key.alt && !key.shift)
		{
			if (fm::util::clipboard::openClipboard())
			{
				fm::util::clipboard::emptyClipboard();

				fm::String data;
				C(m_carets.size())
				{
					TextPos tmin = fm::math::min(m_carets[i].tp,m_carets[i].sel);
					TextPos tmax = fm::math::max(m_carets[i].tp,m_carets[i].sel);

					while (tmin.line<tmax.line)
						data += m_lines[tmin.line].substr(tmin.pos) + '\n',
						tmin.pos = 0,
						tmin.line++;

					data += m_lines[tmin.line].substr(tmin.pos,tmax.pos-tmin.pos);

					if (i<m_carets.size()-1)
						data += '\n';
				}

				fm::util::clipboard::setData(data);

				fm::util::clipboard::closeClipboard();
			}
		}

		/* CTRL + V */
		if (key.code == fw::Keyboard::V && key.ctrl && !key.alt && !key.shift)
		{
			if (fm::util::clipboard::openClipboard())
			{
				if (fm::util::clipboard::hasTextData())
				{
					fm::String s = fm::util::clipboard::getData();
					C(m_carets.size())
						addText(m_carets[i],s);
				}

				fm::util::clipboard::closeClipboard();
			}
		}


		/* Escape */
		if (key.code == fw::Keyboard::Escape)
		{
			m_carets.resize(1);
			m_needRecalcCarets = true;
			m_needSetViewCaret = true;

			return true;
		}

		/* PgUp & PgDn */
		if (key.code == fw::Keyboard::PageUp ||
			key.code == fw::Keyboard::PageDown)
		{
			bool up = key.code == fw::Keyboard::PageUp;

			m_carets[0] = m_carets.back();
			m_carets.resize(1);
			moveCaretLine(m_carets[0],getSize().h/m_metrics.lineGap*(up?-1:1),true);

			return true;
		}

		/* Insert */
		if (key.code == fw::Keyboard::Insert)
		{
			setInsertMode(!getInsertMode());

			return true;
		}

		/* del / bksp */
		if (key.code == fw::Keyboard::Delete ||
			key.code == fw::Keyboard::Backspace)
		{
			int ir = ((key.code == fw::Keyboard::Backspace) ? -1 : 1);

			C(m_carets.size())
			{
				if (key.ctrl)
					delText(m_carets[i],ir*getWordLength(m_carets[i].tp,ir==-1));
				else
					delText(m_carets[i],ir);
			}

			uniquifyCarets();

			return true;
		}

		/* enter */
		if (key.code == fw::Keyboard::Enter)
		{
			C(m_carets.size())
			{
				if (key.ctrl)
					setCaret(m_carets[i],m_carets[i].tp.line,m_lines[m_carets[i].tp.line].size());

				newLine(m_carets[i]);
			}

			uniquifyCarets();

			return true;
		}

		/* left / right */
		if (key.code == fw::Keyboard::Left ||
			key.code == fw::Keyboard::Right)
		{
			int delta = ((key.code == fw::Keyboard::Left) ? -1 : 1);

			C(m_carets.size())
			{
				if (key.ctrl)
					moveCaretPos(m_carets[i],delta*getWordLength(m_carets[i].tp,delta==-1),!key.shift);
				else
					moveCaretPos(m_carets[i],delta,!key.shift);
			}

			uniquifyCarets();

			return true;
		}


		/* up / down */
		if (key.code == fw::Keyboard::Up ||
			key.code == fw::Keyboard::Down)
		{
			int delta = ((key.code == fw::Keyboard::Up) ? -1 : 1);

			if (!key.alt && !key.shift && key.ctrl)
			{
				setViewPos(m_pixViewPos+fm::vec2(0,m_metrics.lineGap*delta));

				return true;
			}

			if (!key.alt && key.shift && key.ctrl)
			{
				extendSelsToLine();

				moveLines(delta);

				return true;
			}

			bool multisel = (fw::Keyboard::isKeyHeld(fw::Keyboard::LShift) && fw::Keyboard::isKeyHeld(fw::Keyboard::LAlt));

			std::deque<fm::Size> newCaretsLine;
			std::deque<fm::Size> newCaretsPos;

			C(m_carets.size())
			{
				if (multisel)
				{
					if ((m_carets[i].tp.line>0 || delta>0) && (m_carets[i].tp.line<m_lines.size()-1 || delta<0))
					{
						newCaretsLine.push_back(m_carets[i].tp.line+delta);
						newCaretsPos.push_back(m_carets[i].tp.pos);
					}
				}
				else
					moveCaretLine(m_carets[i],delta,!key.shift);
			}

			C(newCaretsLine.size())
				addCaret(newCaretsLine[i],newCaretsPos[i]);

			uniquifyCarets();

			return true;
		}

		/* end */
		if (key.code == fw::Keyboard::End)
		{
			if (key.ctrl)
			{
				m_carets.resize(1);

				TextPos selP = m_carets[0].sel;
				setCaret(m_carets[0],m_lines.size()-1,m_lines.back().size());

				if (key.shift)
					m_carets[0].sel = selP;
			}
			else
			{
				C(m_carets.size())
					moveCaretPos(m_carets[i],-((int)m_carets[i].tp.pos)+(int)m_lines[m_carets[i].tp.line].size(),!key.shift);

				uniquifyCarets();
			}

			return true;
		}

		/* home */
		if (key.code == fw::Keyboard::Home)
		{
			if (key.ctrl)
			{
				m_carets.resize(1);

				TextPos selP = m_carets[0].sel;
				setCaret(m_carets[0],0,0);

				if (key.shift)
					m_carets[0].sel = selP;
			}
			else
			{
				C(m_carets.size())
				{
					if (m_carets[i].tp.pos)
						moveCaretPos(m_carets[i],-((int)m_carets[i].tp.pos),!key.shift);
					else
						moveCaretPos(m_carets[i],-((int)m_carets[i].tp.pos)+(int)getLeadingWhiteSpaces(m_carets[i].tp.line).size(),!key.shift);
				}

				uniquifyCarets();
			}

			return true;
		}

		if (key.code >= fw::Keyboard::A && key.code <= fw::Keyboard::Z)
		{
			if (key.ctrl && !key.shift && !key.alt)
			{
				if (key.code == fw::Keyboard::A)
				{
					m_carets.resize(1);
					setCaret(m_carets[0],0,0);

					m_carets[0].sel = TextPos(m_lines.size()-1,m_lines.back().size());
				}

				if (key.code == fw::Keyboard::D)
				{
					C(m_carets.size())
					{
						if (m_carets[i].tp == m_carets[i].sel)
							duplicateLine(m_carets[i].tp.line);
						else
							duplicateSelected(m_carets[i]);
					}
				}
			}
		}

		return false;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::duplicateLine(fm::Size line)
	{
		if (!getEditable()) return;

		m_needBuildVertices = true;
		m_needRecalcCarets  = true;
		m_needSetViewCaret  = true;

		m_lines.insert(m_lines.begin()+line,m_lines[line]);
		m_charColors.insert(m_charColors.begin()+line,m_charColors[line]);

		C(m_carets.size())
			if (m_carets[i].tp.line > line)
				m_carets[i].tp.line++,
				m_carets[i].sel.line++;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::duplicateSelected(Caret &caret)
	{
		if (!getEditable()) return;

		m_needBuildVertices = true;
		m_needRecalcCarets  = true;
		m_needSetViewCaret  = true;

		TextPos tpMin;
		TextPos tpMax;

		if (caret.tp < caret.sel)
			tpMin = caret.tp,
			tpMax = caret.sel;
		else
			tpMin = caret.sel,
			tpMax = caret.tp;

		std::deque<fm::String> toAdd;

		while (tpMin.line < tpMax.line)
		{
			toAdd.push_back(fm::String(m_lines[tpMin.line].begin()+tpMin.pos,m_lines[tpMin.line].end()));

			tpMin.pos = 0;
			tpMin.line ++;
		}

		toAdd.push_back(fm::String(m_lines[tpMin.line].begin()+tpMin.pos,m_lines[tpMin.line].begin()+tpMax.pos));

		Caret c;
		c.tp  = tpMax;
		c.sel = tpMax;

		C(toAdd.size())
		{
			addText(c,toAdd[i]);

			if (i+1 < FRONTIER_C_N)
				newLine(c);
		}
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::extendSelsToLine()
	{
		m_needRecalcCarets  = true;
		m_needSetViewCaret  = true;

		C(m_carets.size())
		{
			if (m_carets[i].tp < m_carets[i].sel) std::swap(m_carets[i].tp,m_carets[i].sel);
			m_carets[i].sel.pos = 0;
			m_carets[i].tp.pos = m_lines[m_carets[i].tp.line].size();
		}

		uniquifyCarets();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::moveLines(int dir)
	{
		if (!m_editable)
			return;

		m_needBuildVertices = true;

		std::deque<fm::Size> moveBegs;
		std::deque<fm::Size> moveEnds;

		C(m_carets.size())
		{
			bool needNew = true;
			Cx(moveBegs.size())
				if (moveBegs[x] == m_carets[i].tp.line + 1)
				{
					moveBegs[x] = m_carets[i].sel.line;
					needNew = false;
					break;
				}
				else if (moveEnds[x] + 1 == m_carets[i].sel.line)
				{
					moveEnds[x] = m_carets[i].tp.line;
					needNew = false;
					break;
				}

			if (needNew)
				moveBegs.push_back(m_carets[i].sel.line),
				moveEnds.push_back(m_carets[i].tp.line);
		}

		C(moveBegs.size())
		{
			if (dir == -1)
			{
				if (moveBegs[i] != 0)
				{
					for (fm::Size k = moveBegs[i];k<=moveEnds[i];k++)
						m_lines[k].swap(m_lines[k-1]),
						m_charColors[k].swap(m_charColors[k-1]);
				}
			}
			if (dir == 1)
			{
				if (moveEnds[i] != m_lines.size()-1)
				{
					Cx(moveEnds[i]-moveBegs[i]+1)
						m_lines[moveEnds[i]-x].swap(m_lines[moveEnds[i]-x+1]),
						m_charColors[moveEnds[i]-x].swap(m_charColors[moveEnds[i]-x+1]);
				}
			}
		}

		C(m_carets.size())
		{
			if ((dir != -1 || m_carets[i].sel.line != 0) &&
				(dir !=  1 || m_carets[i].tp.line  != m_lines.size()-1))
					m_carets[i].sel.line += dir,
					m_carets[i].tp.line  += dir;
		}

		uniquifyCarets();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setViewPos(const fm::vec2 &viewPos)
	{
		m_pixViewPos = viewPos;

		if (m_pixViewPos.x < 0) m_pixViewPos.x = 0;
		if (m_pixViewPos.y < 0) m_pixViewPos.y = 0;

		fm::vec2 vp;

		vp.x = (int)m_pixViewPos.x    /(int)getCharWidth(' ') * (int)getCharWidth(' ')  ;

		vp.y = (int)m_pixViewPos.y    /(int)m_metrics.lineGap * (int)m_metrics.lineGap  ;

		if (m_viewPos != vp)
			m_viewPos = vp,
			m_needBuildVertices = true;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2 BasicControl::getViewPos()
	{
		return m_viewPos;
	}

	/////////////////////////////////////////////////////////////
	BasicControl::BasicControl(const std::string &name,
							   const Anchor &anchor,
							   const fm::vec2 &size,
							   Widget *parent,
							   const fm::String &defText,
							   fg::Font *font,
							   fm::Size characterSize,
							   bool editable,
							   bool monoSpace,
							   fm::vec4 textColor,
							   fm::vec4 selectColor,
							   fm::vec4 caretColor,
							   fm::vec4 bckColor) : Widget(name,anchor,size,parent),
													  m_textColor(textColor),
													  m_selectColor(selectColor),
													  m_bckColor(bckColor),
													  m_caretColor(caretColor),
													  m_charSize(characterSize),
													  m_ibo(fg::IndexBuffer),
													  m_iboBkg(fg::IndexBuffer),
													  m_caretIbo(fg::IndexBuffer),
													  m_vertexCount(0),
													  m_vertexCountBkg(0),
													  m_vertexCountCaret(0),
													  m_vertexCountCaretBkg(0),
													  m_font(font),
													  m_needRecalcCarets(false),
													  m_needBuildVertices(false),
													  m_needSetupPosition(false),
													  m_needSetViewCaret(false),
													  m_insertMode(false),
													  m_monoSpace(monoSpace),
													  m_editable(editable),
													  m_dragCaret(0),
													  m_dragMode(DragMode::NoDrag),
													  m_rightDown(false)
	{
		setContent(defText);
	}

	/////////////////////////////////////////////////////////////
	BasicControl::~BasicControl()
	{

	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setAnchor(const Anchor &anchor)
	{
		Widget::setAnchor(anchor);

		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setParent(Widget *parent)
	{
		Widget::setParent(parent);

		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setSize(const fm::vec2 &size)
	{
		Widget::setSize(size);

		buildVertices();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onDrawChars()
	{
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
		glDrawElements(fg::Triangles,m_vertexCount/4*6,GL_UNSIGNED_INT,0);

		fg::Buffer::bind(fm::nullPtr,fg::ArrayBuffer);
		fg::Buffer::bind(fm::nullPtr,fg::IndexBuffer);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_TEXTURE_2D);

		glDisable(GL_BLEND);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onDrawCharsBkg()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		m_vboBkg.bind();
		glVertexPointer(2,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,pos));
		glColorPointer(4,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,clr));

		m_iboBkg.bind();
		glDrawElements(fg::Triangles,m_vertexCountBkg/4*6,GL_UNSIGNED_INT,0);

		fg::Buffer::bind(fm::nullPtr,fg::ArrayBuffer);
		fg::Buffer::bind(fm::nullPtr,fg::IndexBuffer);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glDisable(GL_BLEND);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onDrawCarets()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		m_caretVbo.bind();
		glVertexPointer(2,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,pos));
		glColorPointer(4,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,clr));

		m_caretIbo.bind();
		glDrawElements(fg::Lines,m_vertexCountCaret,GL_UNSIGNED_INT,0);

		fg::Buffer::bind(fm::nullPtr,fg::ArrayBuffer);
		fg::Buffer::bind(fm::nullPtr,fg::IndexBuffer);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glDisable(GL_BLEND);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onDrawCaretsBkg()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		m_caretVbo.bind();
		glVertexPointer(2,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,pos));
		glColorPointer(4,GL_FLOAT,sizeof(fm::vert2f),(void*)fm::offsetOf(fm::vert2f,clr));

		m_caretIbo.bind();
		glDrawElements(fg::Triangles,m_vertexCountCaretBkg/4*6,GL_UNSIGNED_INT,((fm::Uint32*)0+m_vertexCountCaret));

		fg::Buffer::bind(fm::nullPtr,fg::ArrayBuffer);
		fg::Buffer::bind(fm::nullPtr,fg::IndexBuffer);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glDisable(GL_BLEND);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onDraw()
	{
		if (!m_tex)
			return;

		glEnable(GL_SCISSOR_TEST);

		fm::vec2 rs = getRoot()->getSize();
		fm::vec2 s  = getSize();
		fm::vec2 p  = getPosInRoot();

		glScissor(p.x,rs.y-p.y-s.h,s.w,s.h);

		/** setup transformations **/
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&m_transform.transpose()[0][0]);

		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&m_tex->getPixToUnitMatrix().transpose()[0][0]);

		onDrawCharsBkg();
		onDrawCaretsBkg();

		onDrawChars();

		if (getActive() == this)
		{
			onDrawCarets();
		}

		/** reset transformations **/
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&fm::mat4()[0][0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&fm::mat4()[0][0]);

		glDisable(GL_SCISSOR_TEST);
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onParentChange()
	{
		setupPosition();
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setContent(const fm::String &str)
	{
		m_font->setCharacterSize(m_charSize);
		m_metrics = m_font->getMetrics();

		m_lines.resize(1);
		m_charColors.resize(1);
		m_carets.resize(1);

		m_lines.back() = fm::String();
		m_charColors.back() = std::deque<CharColor>();
		m_carets.back() = Caret();

		C(str.length())
		{
			if (str[i]=='\n')
				m_lines.push_back(fm::String()),
				m_charColors.push_back(std::deque<CharColor>());
			else
				m_lines.back() += str[i],
				m_charColors.back().push_back(CharColor(m_textColor,m_bckColor));
		}

		m_needBuildVertices = true;
		m_needRecalcCarets  = true;
		m_needSetViewCaret  = true;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setEditable(bool enable)
	{
		m_editable = enable;
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::getEditable() const
	{
		return m_editable;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::setCharSize(unsigned int charSize)
	{
		if (m_charSize != charSize)
		{
			m_charSize = charSize;
			m_needBuildVertices = true;
			m_needRecalcCarets  = true;
		}
	}

	/////////////////////////////////////////////////////////////
	unsigned int BasicControl::getCharSize() const
	{
		return m_charSize;
	}

	/////////////////////////////////////////////////////////////
	bool BasicControl::handleEvent(const fw::Event &ev)
	{
		if (!getEnabled())
			return false;

		bool handled = false;
		bool active  = (getActive() == this);

        if (active && ev.type == fw::Event::TextEntered)
		{
			fm::Uint32 notPrinted[] = {'\b','\n','\r',127,27,1,19,4,24,3,22};

			bool printed = true;
			C(sizeof(notPrinted)/sizeof(*notPrinted))
				if (notPrinted[i] == ev.text.utf8character)
				{
					printed = false;
					break;
				}

			if (!printed)
				return true;

			fm::String s(ev.text.utf8character);

			C(m_carets.size())
			{
				if (m_insertMode && m_carets[i].tp.pos < m_lines[m_carets[i].tp.line].size())
					delText(m_carets[i],1);

				addText(m_carets[i],s);
			}

			uniquifyCarets();

			handled = true;
		}
		if (active && ev.type == fw::Event::KeyPressed)
		{
			handled = handleShortcut(ev.key);
		}
		if (ev.type == fw::Event::ButtonPressed)
		{
			if (fm::rect2f(getPosInRoot(),getSize()).contains(fm::vec2::loadxy(ev.mouse)))
			{
				if (ev.mouse.button == fw::Mouse::Left)
				{
					fm::vec2 p = fm::vec2::loadxy(ev.mouse)+getViewPos();
					fm::vec2 posInRoot = getPosInRoot();

					// if (fm::rect2f(posInRoot-fm::vec2(30,30),getSize()+fm::vec2(60,60)).contains(p))
					{
						fm::Size line = lineFromPixPos(p-posInRoot);
						fm::Size pos  = posFromPixPos(p-posInRoot);
						TextPos clickTp(line,pos);

						bool ctrl  = fw::Keyboard::isKeyHeld(fw::Keyboard::LCtrl)  || fw::Keyboard::isKeyHeld(fw::Keyboard::RCtrl);
						bool shift = fw::Keyboard::isKeyHeld(fw::Keyboard::LShift) || fw::Keyboard::isKeyHeld(fw::Keyboard::RShift);

						bool midSel = false;

						C(m_carets.size())
							if (clickTp.isBetween(m_carets[i].tp,m_carets[i].sel) && m_carets[i].tp != m_carets[i].sel)
							{
								midSel = true;

								break;
							}

						if (!ctrl && !shift)
						{
							if (!midSel)
							{
								m_carets.resize(1);
								setCaret(m_carets[0],line,pos);

								m_dragCaret = 0;
								m_dragMode  = DragMode::FixedSel;
							}
						}
						if (ctrl && !shift)
						{
							if (!midSel)
							{
								addCaret(clickTp);

								m_dragCaret = m_carets.size()-1;
								m_dragMode  = DragMode::FixedSel;
							}
						}
						if (!ctrl && shift)
						{
							fm::Size careti=0;

							C(m_carets.size())
								if (m_carets[i].tp < clickTp && m_carets[careti].tp < m_carets[i].tp)
									careti = i;

							m_dragCaret = careti;
							m_dragMode  = DragMode::FixedSel;
						}

						if (m_clickClk.getTime() < fm::milliseconds(400) && m_lastClick == clickTp)
						{
							Caret &targetCaret = m_carets[m_dragCaret];
							Caret caret(clickTp,0,clickTp);

							moveCaretPos(caret,getWordLength(caret.tp,true,false)*-1,true);

							moveCaretPos(caret,getWordLength(caret.tp,false,false),false);

							if (caret.tp == targetCaret.tp && caret.sel == m_carets[m_dragCaret].sel)
							{
								if (caret.tp.line != m_lines.size()-1)
								{
									caret.tp.line = clickTp.line+1;
									caret.sel.line = clickTp.line;
									caret.sel.pos = 0;
									caret.tp.pos = 0;
								}
								else
								{
									caret.tp.line = clickTp.line;
									caret.sel.line = clickTp.line;
									caret.sel.pos = 0;
									caret.tp.pos = m_lines.back().size();
								}

							}
							else if (targetCaret.sel.pos == 0 && targetCaret.tp.pos == 0 && targetCaret.sel.line+1 == targetCaret.tp.line)
							{
								caret.tp = clickTp;
								caret.sel = clickTp;
							}

							targetCaret = caret;

							m_dragMode  = DragMode::KeepWord;

							m_wordBeg = fm::math::min(caret.tp,caret.sel);
							m_wordEnd = fm::math::max(caret.tp,caret.sel);

							uniquifyCarets();
						}

						m_lastClick = clickTp;
						m_clickClk.restart();

						handled = true;
					}

					if (!active)
						setActive(this),
						active = true;
				}
				if (active && ev.mouse.button == fw::Mouse::Right)
				{
					m_rightDown = true;
					m_lastRightClick = fm::vec2::loadxy(ev.mouse);
				}
			}
		}
		if (active && ev.type == fw::Event::ButtonReleased)
		{
			if (ev.mouse.button == fw::Mouse::Left)
			{
				if (m_dragMode)
				{
					m_dragMode = DragMode::NoDrag;
					handled = true;

					uniquifyCarets();
				}
				else if (fm::rect2f(getPosInRoot(),getSize()).contains(fm::vec2::loadxy(ev.mouse)))
				{
					fm::vec2 p = fm::vec2::loadxy(ev.mouse)+getViewPos();
					fm::vec2 posInRoot = getPosInRoot();

					fm::Size line = lineFromPixPos(p-posInRoot);
					fm::Size pos  = posFromPixPos(p-posInRoot);

					m_carets.resize(1);
					setCaret(m_carets[0],line,pos,true);
				}
			}
			if (ev.mouse.button == fw::Mouse::Right)
			{
				m_rightDown = false;
			}
		}
		if (active && ev.type == fw::Event::MouseWheelMoved)
		{
			setViewPos(m_pixViewPos+fm::vec2(0,-ev.wheel.delta*m_metrics.lineGap*4));
		}
		if (active && (ev.type == fw::Event::MouseMoved || ev.type == fw::Event::MouseWheelMoved))
		{
			fm::vec2 p;

			if (ev.type == fw::Event::MouseMoved) p = fm::vec2::loadxy(ev.motion);
			else p = fm::vec2::loadxy(ev.wheel);

			if (m_rightDown)
			{
				setViewPos(m_pixViewPos - (p-m_lastRightClick));

				m_lastRightClick = p;
			}
			if (m_dragMode)
			{
				p += getViewPos();
				fm::vec2 posInRoot = getPosInRoot();

				fm::Size line = lineFromPixPos(p-posInRoot);
				fm::Size pos  = posFromPixPos(p-posInRoot);
				TextPos clickTp(line,pos);

				TextPos oldSel = m_carets[m_dragCaret].sel;

				if (m_dragMode == DragMode::FixedSel)
				{
					setCaret(m_carets[m_dragCaret],line,pos);
					m_carets[m_dragCaret].sel = oldSel;
				}
				if (m_dragMode == DragMode::ResetSel)
				{
					setCaret(m_carets[m_dragCaret],line,pos);
				}
				if (m_dragMode == DragMode::KeepWord)
				{
					TextPos minTp = fm::math::min(clickTp,m_wordBeg);
					TextPos maxTp = fm::math::max(clickTp,m_wordEnd);

					if (minTp == clickTp)
					{
						setCaret(m_carets[m_dragCaret],minTp);
						m_carets[m_dragCaret].sel = maxTp;
					}
					else if (maxTp == clickTp)
					{
						setCaret(m_carets[m_dragCaret],maxTp);
						m_carets[m_dragCaret].sel = minTp;
					}
					else
					{
						if (fm::math::abs(charBetween(m_wordBeg,clickTp)) < fm::math::abs(charBetween(m_wordEnd,clickTp)))
							setCaret(m_carets[m_dragCaret],minTp),
							m_carets[m_dragCaret].sel = maxTp;
						else
							setCaret(m_carets[m_dragCaret],maxTp),
							m_carets[m_dragCaret].sel = minTp;
					}
				}
			}
		}

		return handled;
	}

	/////////////////////////////////////////////////////////////
	void BasicControl::onUpdate()
	{
		if (m_needSetViewCaret)
			setViewCaret(),
			m_needSetViewCaret = false;

		if (m_needRecalcCarets)
			recalcCarets(),
			m_needRecalcCarets = false;

		if (m_needBuildVertices)
			buildVertices(),
			m_needBuildVertices = false;

		if (m_needSetupPosition)
			setupPosition(),
			m_needSetupPosition = false;

		onCaretColorUpdate();
	}

	/////////////////////////////////////////////////////////////
	const std::string &BasicControl::getRealType() const
	{
		return m_className;
	}
}
