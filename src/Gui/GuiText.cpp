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
	
	class SpritesFromText
	{
	public:
		const fm::String &str;
		fg::Glyph::Style style;
		fg::Font font;
		fm::Size charSize;
		TextAlign align;
		TextWrap wrap;
		fm::vec2 &viewSize;
		fm::vec2 viewOffset;
		bool monospacing;
		fm::vec4 color;
		fm::vec2 maxView;
		fm::vec2 pos;
		int tabwidth;
		
		std::vector<fm::Size> wordBegs,wordEnds;
		std::vector<float> wordPixDifBegs,wordPixDifEnds;
		std::vector<fm::Size> newLinePoses;
		std::vector<fm::Size> lineWidths;
		fm::Size maxLineWidth;
		
		SpritesFromText(const fm::String &str,
						fg::Glyph::Style style,
						fg::Font font,
						fm::Size charSize,
						TextAlign align,
						TextWrap wrap,
						fm::vec2 &viewSize,
						fm::vec2 viewOffset,
						fm::vec4 color,
						fm::vec2 pos) : 
						 str(str),
						 style(style),
						 font(font), 
						 charSize(charSize), 
						 align(align), 
						 wrap(wrap), 
						 viewSize(viewSize), 
						 viewOffset(viewOffset), 
						 color(color),
						 pos(pos),
						 tabwidth(4)
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
		
		float getWidthCp(fm::Uint32 prevcp,fm::Uint32 cp)
		{
			if (cp == ' ' ) return getWidthCp(0,'m');
			if (cp == '\t') return getWidthCp(0,'m')*tabwidth;
			
			fm::rect2i grect = font.getGlyphRect(monospacing ? 'm' : cp,style);
			float width = grect.size.w + grect.pos.x;
			
			if (prevcp)
				width += font.getKerning(prevcp,cp) * 0;
			
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
				
				x += getWidthCp((!i ? 0 : str[i-1]),str[i]);
				
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
		std::vector<fm::vec2> spritePoses;
		std::vector<fg::FontSprite> sprites;
		fm::String renderedText;
		
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
		
		void renderCharacterRects(bool pass2 = true)
		{
			if (pass2 && !lineWidths.size())
				renderCharacterRects(false);
			
			spritePoses.clear();
			renderedText = "";
			
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
						begNewLine(pass2);
						whiteSpacesDiscarded = true;
						
						newLineId++;
					}
				}
				
				if (!whiteSpacesDiscarded && wrap == TextWrapWord && viewSize.w != 0)
				{
					if (curPos.x + transfViewOffset + wordPixDifBegs[i] + wordPixDifEnds[i] >= viewSize.w)
					{
						begNewLine(pass2);
						whiteSpacesDiscarded = true;
					}
				}
				
				float woff = whiteSpacesDiscarded ? 0 : wordPixDifBegs[i];
				
				curPos.x += woff;
				
				for (fm::Size l = wordBegs[i];l != wordEnds[i];++l)
				{
					fm::Uint32 cp = str[l];
					fm::rect2i grect = font.getGlyphRect(cp,style);
					
					if (viewSize.w != 0 && curPos.x + transfViewOffset + grect.size.w >= viewSize.w && wrap != TextWrapOff)
					{
						if (l == wordBegs[i])
						{
							curPos.x -= woff;
						}
						
						begNewLine(pass2);
					}
					
					if (pass2)
					{
						bool goodUp    = (lineWidthId+1) * metrics.lineGap + viewOffset.y > 0;
						bool goodDown  = lineWidthId * metrics.lineGap + viewOffset.y - viewSize.h < 0;
						bool goodLeft  = transformToAlign(curPos).x + grect.size.w + viewOffset.x > 0;
						bool goodRight = transformToAlign(curPos).x + viewOffset.x - viewSize.w < 0;
						
						if ((goodLeft && goodUp) && ((goodDown && goodRight) || viewSize.area() == 0))
						{
							fm::vec2  leftAlignPos = curPos + grect.pos + fm::vec2(0,metrics.maxH);
							fm::vec2i rectCorner   = transformToAlign(leftAlignPos) + viewOffset;
							
							if (viewSize.area())
							{
								rectCorner.x = std::min<float>(std::max<float>(rectCorner.x,0),viewSize.w);
								rectCorner.y = std::min<float>(std::max<float>(rectCorner.y,0),viewSize.h);
							}
							
							maxView.x = std::max<int>(rectCorner.x + grect.size.w,maxView.x);
							maxView.y = std::max<int>(rectCorner.y + grect.size.h,maxView.y);
							
							spritePoses.push_back(rectCorner);
							renderedText+=cp;			
						}
					}
					
					curPos.x += getWidthCp(0,cp);
				}
			}
			begNewLine(pass2);
						
			if (!viewSize.area())
				viewSize = maxView;
		}
		
		void renderCharacters()
		{
			renderCharacterRects();
			
			sprites = std::move(font.getSprites(renderedText,style));
			
			for (fm::Size i=0;i<sprites.size();++i)
			{
				fg::FontSprite &sprite = sprites[i];
				sprite.setPosition(spritePoses[i] + pos);
				sprite.setColor(color);	
			}
		}
		
		std::vector<fg::FontSprite> getSprites()
		{
			if (!font) return std::vector<fg::FontSprite>();
			
			font.setCharacterSize(charSize);
			metrics = font.getMetrics();
			
			findWordBoundaries();
			renderCharacters();
			
			return std::move(sprites);
		}
	};
	
	void GuiText::updateSprites()
	{
		if (getFont())
		{
			fm::vec2 viewSize = m_viewSize;
			
			m_sprites = std::move(SpritesFromText(m_string,m_style,getFont(),m_charSize,m_align,m_wrapMode,viewSize,m_viewOffset,m_clr,getPosition()).getSprites());
			
			GuiElement::setSize(viewSize);		
		}
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(GuiContext &owner) : GuiElement(owner),
										  m_style(fg::Glyph::Regular),
										  m_wrapMode(TextWrapWord),
										  m_charSize(14),
										  m_align(TextAlignLeft),
										  m_font(owner.getDefaultFont())
										  
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(GuiContext &owner,fm::vec2 size) : GuiElement(owner,size),
													    m_style(fg::Glyph::Regular),
													    m_viewSize(size),
														m_wrapMode(TextWrapWord),
													    m_charSize(14),
													    m_align(TextAlignLeft),
													    m_clr(fm::vec4::Black),
													    m_font(owner.getDefaultFont())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(GuiContext &owner,fm::String text) : GuiElement(owner),
														  m_style(fg::Glyph::Regular),
														  m_wrapMode(TextWrapWord),
													      m_charSize(14),
														  m_string(text),
													      m_align(TextAlignLeft),
													      m_clr(fm::vec4::Black),
													      m_font(owner.getDefaultFont())
	{
		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	GuiText::GuiText(GuiContext &owner,fm::vec2 size,fm::String text) : GuiElement(owner,size),
																		m_style(fg::Glyph::Regular),
																		m_wrapMode(TextWrapWord),
																		m_charSize(14),
																		m_string(text),
																		m_align(TextAlignLeft),
																		m_clr(fm::vec4::Black),
																		m_font(owner.getDefaultFont())
	{
		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setSize(fm::vec2s size)
	{
		m_viewSize = size;
		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setText(const fm::String &str)
	{
		if (m_string != str)
		{
			m_string = str;
			updateSprites();
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setStyle(const fg::Glyph::Style &style)
	{
		if (m_style != style)
		{
			m_style = style;
			updateSprites();
		}
	}
	
	/////////////////////////////////////////////////////////////
	fg::Glyph::Style GuiText::getStyle() const
	{
		return m_style;
	}
	
	/////////////////////////////////////////////////////////////
	const fm::String &GuiText::getText() const
	{
		return m_string;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::onDraw(fg::ShaderManager &shader)
	{
		(void)shader;
	}
		
	/////////////////////////////////////////////////////////////
	void GuiText::setFont(GuiFont font)
	{
		m_font = font;
		updateSprites();
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
		updateSprites();
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
		updateSprites();
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
		updateSprites();
	}

	/////////////////////////////////////////////////////////////
	void GuiText::setColor(fm::vec4 color)
	{
		m_clr = color;
		
		for (auto &sprite : m_sprites)
			sprite.setColor(m_clr);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiText::setPosition(fm::vec2i pos)
	{
		fm::vec2i diff = pos - getPosition();
		
		for (auto &sprite : m_sprites)
			sprite.setPosition(sprite.getPosition() + diff);
		
		GuiElement::setPosition(pos);
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
		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GuiText::getViewOffset() const
	{
		return m_viewOffset;
	}
	
	/////////////////////////////////////////////////////////////
	fg::FontSprite &GuiText::getCharSprite(fm::Size index)
	{
		return m_sprites[index];
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GuiText::getCharSpriteCount() const
	{
		return m_sprites.size();
	}
}
