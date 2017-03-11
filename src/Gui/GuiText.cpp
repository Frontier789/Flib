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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/Gui/GuiText.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	GuiFont::GuiFont() : font(fg::Font())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiFont::GuiFont(fg::Font font) : font(font)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiFont::GuiFont(fm::String fontName) : fontName(fontName)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font GuiFont::getFont(fm::Ref<GuiContext> owner)
	{
		if (font) return font;
		
		if (owner && fontName.size())
		{
			return font = owner->getFont(fontName);
		}
		
		return fg::Font();
	}
	
	class DrawDataFromText
	{
	public:
		const fm::String &str;
		fg::Font font;
		fm::Size charSize;
		TextAlign align;
		TextWrap wrap;
		fm::vec2 &viewSize;
		fm::vec2 viewOffset;
		bool monospacing;
		fm::vec4 color;
		
		std::vector<fm::Size> wordBegs,wordEnds;
		std::vector<float> wordPixDifBegs,wordPixDifEnds;
		std::vector<fm::Size> newLinePoses;
		std::vector<fm::Size> lineWidths;
		fm::Size maxLineWidth;
		
		DrawDataFromText(const fm::String &str,
						 fg::Font font,
						 fm::Size charSize,
						 TextAlign align,
						 TextWrap wrap,
						 fm::vec2 &viewSize,
						 fm::vec2 viewOffset,
						 fm::vec4 color) : str(str), font(font), charSize(charSize), align(align), wrap(wrap), viewSize(viewSize), viewOffset(viewOffset), color(color)
		{
			monospacing = false;
		}
		
		static int categorizeCp(fm::Uint32 cp)
		{
			if ((cp >= 'a' && cp <= 'z') || 
				(cp >= 'A' && cp <= 'Z') ||
				(cp >= '0' && cp <= '9') ||
				 cp == '_') return 0;
			
			if (fm::String("<>=!.,-+*/^&{}[]()").find(cp) != fm::String::npos) return 1;
			
			if (fm::String("\t \r\n").find(cp) != fm::String::npos) return 2;
			
			return 3;
		}
		
		static bool isWhiteSpaceCp(fm::Uint32 cp)
		{
			return fm::String("\t \r\n").find(cp) != fm::String::npos;
		}
		
		static float getWidthCp(fm::Uint32 prevcp,fm::Uint32 cp,fg::Font font,bool monospace)
		{
			if (cp == ' ' ) return getWidthCp(0,'m',font,monospace);
			if (cp == '\t') return getWidthCp(0,'m',font,monospace)*4;
			
			fg::Glyph g = font.getGlyph(monospace ? 'm' : cp);
			float width = g.size.w + g.leftdown.x;
			
			if (prevcp)
				width += font.getKerning(prevcp,cp);
			
			return width;
		}
		
		void findWordBoundaries()
		{
			float x = 0;
			
			C(str.size())
			{
				if (!isWhiteSpaceCp(str[i]) && (!i || isWhiteSpaceCp(str[i-1])))
				{
					wordBegs.push_back(i);
					wordPixDifBegs.push_back(x);
					x = 0;
				}
				
				x += getWidthCp((!i ? 0 : str[i-1]),str[i],font,monospacing);
				
				if (!isWhiteSpaceCp(str[i]) && (i+1 == str.size() || isWhiteSpaceCp(str[i+1])))
				{
					wordEnds.push_back(i+1);
					wordPixDifEnds.push_back(x);
					x = 0;
				}
				
				if (str[i] == '\n' || (str[i] == '\r' && (i+1 == str.size() || str[i+1] != '\n')))
					newLinePoses.push_back(i);
			}
		}
		
		float transfViewOffset;
		fm::Size lineWidthId;
		fg::Metrics metrics;
		fm::Size newLineId;
		fm::vec2 curPos;
		fg::Mesh mesh;
		
		void begNewLine(bool render)
		{
			if (!render)
			{
				lineWidths.push_back(curPos.x);
			}
			
			curPos.x  = 0;
			curPos.y += metrics.lineGap;
			lineWidthId++;
		}
		
		fm::vec2 transformToAlign(fm::vec2 leftp)
		{
			if (align == TextAlignLeft)
			{
				return leftp;
			}
			if (align == TextAlignRight)
			{
				float wline = viewSize.w;
				
				if (viewSize.w == 0)
					wline = maxLineWidth;
				
				return leftp + fm::vec2(wline - lineWidths[lineWidthId] - 1,0);
			}
			if (align == TextAlignCenter)
			{
				float wline = viewSize.w;
				
				if (viewSize.w == 0)
					wline = maxLineWidth;
				
				return leftp + fm::vec2((wline - lineWidths[lineWidthId] - 1)/2,0);
			}
			return leftp;
		}
		
		void calcCorrectViewOffset()
		{
			transfViewOffset = 0;
			
			if (align == TextAlignLeft)
			{
				transfViewOffset = viewOffset.x;
			}
			if (align == TextAlignRight)
			{
				transfViewOffset = -viewOffset.x;
			}
			if (align == TextAlignCenter)
			{
				transfViewOffset = std::abs(viewOffset.x) * 2;
			}
		}
		
		void renderCharacters(bool render = true)
		{
			if (render && !lineWidths.size())
				renderCharacters(false);
			
			maxLineWidth = 0;
			for (auto w : lineWidths) maxLineWidth = std::max(maxLineWidth,w);
			
			curPos = fm::vec2();
			lineWidthId = 0;
			newLineId = 0;
			
			calcCorrectViewOffset();
			
			C(wordBegs.size())
			{
				bool whiteSpacesDiscarded = false;
				
				if (!i && align == TextAlignRight)
				{
					whiteSpacesDiscarded = true;
				}
				
				if (newLineId < newLinePoses.size())
				{
					fm::Size newLinePos = newLinePoses[newLineId];
					
					if ((i && newLinePos < wordBegs[i] && newLinePos >= wordEnds[i-1]) ||
					   (!i && newLinePos < wordBegs[i]))
					{
						begNewLine(render);
						whiteSpacesDiscarded = true;
						
						newLineId++;
					}
				}
				
				if (!whiteSpacesDiscarded && wrap == TextWrapWord && viewSize.w != 0)
				{
					if (curPos.x + transfViewOffset + wordPixDifBegs[i] + wordPixDifEnds[i] >= viewSize.w)
					{
						begNewLine(render);
						whiteSpacesDiscarded = true;
					}
				}
				
				float woff = whiteSpacesDiscarded ? 0 : wordPixDifBegs[i];
				
				curPos.x += woff;
				
				for (fm::Size l = wordBegs[i];l != wordEnds[i];++l)
				{
					fm::Uint32 cp = str[l];
					fg::Glyph g = font.getGlyph(cp);
					
					if (viewSize.w != 0 && curPos.x + transfViewOffset + g.size.w >= viewSize.w && wrap != TextWrapOff)
					{
						if (l == wordBegs[i])
						{
							curPos.x -= woff;
						}
						
						begNewLine(render);
					}
					
					if (render)
					{
						bool goodUp    = (lineWidthId+1) * metrics.lineGap + viewOffset.y > 0;
						bool goodDown  = lineWidthId * metrics.lineGap + viewOffset.y - viewSize.h < 0;
						bool goodLeft  = transformToAlign(curPos).x + g.size.w + viewOffset.x > 0;
						bool goodRight = transformToAlign(curPos).x + viewOffset.x - viewSize.w < 0;
						
						if ((goodLeft && goodUp) && ((goodDown && goodRight) || viewSize.area() == 0))
						{
							fm::vec2  leftAlignPos = curPos + g.leftdown + fm::vec2(0,metrics.maxH);
							fm::vec2i rectCorner   = transformToAlign(leftAlignPos) + viewOffset;
							
							for (auto coef : {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(0,1),fm::vec2(1,0),fm::vec2(0,1),fm::vec2(1,1)})
							{
								fm::vec2 rectPos =  rectCorner + coef * g.size;
								
								if (viewSize.area())
								{
									rectPos.x = std::min<float>(std::max<float>(rectPos.x,0),viewSize.w);
									rectPos.y = std::min<float>(std::max<float>(rectPos.y,0),viewSize.h);
								}
								
								mesh.pts.push_back(fm::vec2i(rectPos));
								mesh.uvs.push_back(fm::vec2i(rectPos - rectCorner) + g.pos);
								mesh.clr.push_back(color);
							}							
						}
					}
					
					curPos.x += getWidthCp(0,cp,font,monospacing);
				}
			}
			begNewLine(render);
		}
		
		fg::DrawData getDrawData()
		{
			if (!font) return fg::DrawData();
			
			metrics = font.getMetrics();
			font.setCharacterSize(charSize);
			
			findWordBoundaries();
			renderCharacters();
			
			return mesh;
		}
	};
	
	void GuiText::updateDrawData()
	{
		if (getFont())
		{
			fm::vec2 viewSize = getSize();
			
			m_drawdata = DrawDataFromText(m_string,getFont(),m_charSize,m_align,m_wrapMode,viewSize,m_viewOffset,m_clr).getDrawData();
			
			if (getSize() == fm::vec2())
				GuiElement::setSize(viewSize);
			
			m_tex = &getFont().getTexture();			
		}
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(GuiContext &owner) : GuiElement(owner),
										  m_tex(nullptr),
										  m_wrapMode(TextWrapWord),
										  m_charSize(14),
										  m_align(TextAlignLeft),
										  m_font(owner.getDefaultFont())
										  
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(fm::vec2 size,GuiContext &owner) : GuiElement(size,owner),
													    m_tex(nullptr),
														m_wrapMode(TextWrapWord),
													    m_charSize(14),
													    m_align(TextAlignLeft),
													    m_clr(fm::vec4::Black),
													    m_font(owner.getDefaultFont())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(fm::String text,GuiContext &owner) : GuiElement(owner),
													      m_tex(nullptr),
														  m_wrapMode(TextWrapWord),
													      m_charSize(14),
														  m_string(text),
													      m_align(TextAlignLeft),
													      m_clr(fm::vec4::Black),
													      m_font(owner.getDefaultFont())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(fm::String text,fm::vec2 size,GuiContext &owner) : GuiElement(size,owner),
																		m_tex(nullptr),
																		m_wrapMode(TextWrapWord),
																		m_charSize(14),
																		m_string(text),
																		m_align(TextAlignLeft),
																		m_clr(fm::vec4::Black),
																		m_font(owner.getDefaultFont())
	{
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setSize(fm::vec2s size)
	{
		GuiElement::setSize(size);
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setText(const fm::String &str)
	{
		m_string = str;
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	const fm::String &GuiText::getText() const
	{
		return m_string;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::onDraw(fg::ShaderManager &shader)
	{
		if (!getFont()) return;
		
		shader.getModelStack().push().mul(fm::MATRIX::translation(getPosition()));
		shader.getTexUVStack().push().mul(getFont().getTexture().getPixToUnitMatrix());
		shader.useTexture(m_tex);
		shader.draw(m_drawdata);
		shader.getTexUVStack().pop();
		shader.getModelStack().pop();
	}
		
	/////////////////////////////////////////////////////////////
	void GuiText::setFont(GuiFont font)
	{
		m_font = font;
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font GuiText::getFont()
	{
		return m_font.getFont(getOwnerContext());
	}
	
	/////////////////////////////////////////////////////////////
	fm::String GuiText::getFontName() const
	{
		return m_font.fontName;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setCharacterSize(fm::Size charSize)
	{
		m_charSize = charSize;
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GuiText::getCharacterSize() const
	{
		return m_charSize;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setWrap(TextWrap mode)
	{
		m_wrapMode = mode;
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	TextWrap GuiText::getWrapMode() const
	{
		return m_wrapMode;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setAlign(TextAlign mode)
	{
		m_align = mode;
		updateDrawData();
	}

	/////////////////////////////////////////////////////////////
	void GuiText::setColor(fm::vec4 color)
	{
		m_clr = color;
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec4 GuiText::getColor() const
	{
		return m_clr;
	}
	
	/////////////////////////////////////////////////////////////
	TextAlign GuiText::getAlign() const
	{
		return m_align;
	}
		
	/////////////////////////////////////////////////////////////
	void GuiText::setViewOffset(fm::vec2 viewOffset)
	{
		m_viewOffset = viewOffset;
		updateDrawData();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GuiText::getViewOffset() const
	{
		return m_viewOffset;
	}
}
