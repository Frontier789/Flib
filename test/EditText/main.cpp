#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>

using namespace std;

namespace fgui
{
	
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
		
		std::vector<fg::FontSprite> &sprites;
		std::vector<fm::Size> wordBegs,wordEnds;
		std::vector<float> wordPixDifBegs,wordPixDifEnds;
		std::vector<fm::Size> newLinePoses;
		std::vector<fm::Size> lineWidths;
		fm::Size maxLineWidth;
		
		SpritesFromText(std::vector<fg::FontSprite> &sprites,
						const fm::String &str,
						fg::Glyph::Style style,
						fg::Font font,
						fm::Size charSize,
						TextAlign align,
						TextWrap wrap,
						fm::vec2 &viewSize,
						fm::vec2 viewOffset,
						fm::vec4 color,
						fm::vec2 pos);
		static int categorizeCp(fm::Uint32 cp);
		static bool isWhiteSpaceCp(fm::Uint32 cp);
		float getWidthCp(fm::Uint32 prevcp,fm::Uint32 cp);
		void findWordBoundaries();
		
		float transfViewOffset;
		fm::Size lineWidthId;
		fg::Metrics metrics;
		fm::Size newLineId;
		fm::vec2 curPos;
		std::vector<fm::vec2> spritePoses;
		fm::String renderedText;
		
		void begNewLine(bool render);
		fm::vec2 transformToAlign(fm::vec2 leftp);
		void calcCorrectViewOffset();
		void renderCharacterRects(bool pass2);
		void renderCharacters();
		void updateSprites();
	};

	class FRONTIER_API GuiEditText : public GuiElement
	{
	protected:
		std::vector<fg::FontSprite> m_sprites; ///< The sprites that build up the text
		std::vector<fm::String> m_lines; ///< The string being displayed per lines 
		fg::Glyph::Style m_style; ///< The style of the text
		fm::vec2 m_viewOffset; ///< The offset of the view rectangle
		fm::vec2 m_frameSize;  ///< The size of the frame
		fm::Size m_lineCount;  ///< Number of line displayed
		fm::vec2 m_viewSize;   ///< The size of the view rectangle
		TextWrap m_wrapMode;   ///< The way the text gets wrapped
		fm::Size m_charSize;   ///< The caharacter size used when drawing
		fm::Size m_tabSize;    ///< Characters per tab
		fm::vec2 m_viewPos;    ///< The position of the view rectangle
		bool m_monospacing;    ///< Indicates whether manual monospacing is used
		TextAlign m_align;     ///< The align of the text
		fm::vec4 m_clr; ///< The color of the text
		GuiFont m_font; ///< The font used at displaying
	
	protected:
        ////////////////////////////////////////////////////////////
		void updateSprites(); ///< Internal function
		void adaptRealSize(); ///< Internal helper function
		Metrics getMetrics() const; ///< Internal helper function

        ////////////////////////////////////////////////////////////
		bool isNewLine(const fm::String &str,fm::Size pos) const; ///< Check if a string has newline at a position
		virtual int classify(fm::Uint32 character) const; ///< Get the class of a character (word boundaries are decided based on it)

        ////////////////////////////////////////////////////////////
		struct TextPos
		{
			fm::Size line; ///< The line of the textual position
			fm::Size pos;  ///< The position on the line

            TextPos(fm::Size line = 0,fm::Size pos = 0); ///< default constructor
            TextPos(const fm::vec2s &vec); ///< construct from vector

            bool operator<(const TextPos &tp) const;  ///< Comparison operator
            bool operator>(const TextPos &tp) const;  ///< Comparison operator
            bool operator<=(const TextPos &tp) const; ///< Comparison operator
            bool operator>=(const TextPos &tp) const; ///< Comparison operator
            bool operator==(const TextPos &tp) const; ///< Comparison operator
            bool operator!=(const TextPos &tp) const; ///< Comparison operator
            TextPos operator+(const TextPos &tp) const; ///< Arithmetic operator
            TextPos operator-(const TextPos &tp) const; ///< Arithmetic operator

            bool isBetween(const TextPos &p1,const TextPos &p2) const; ///< Check if textpos is between two other textposes
		};

        ////////////////////////////////////////////////////////////
		struct Caret
		{
			TextPos tp;
		};

        ////////////////////////////////////////////////////////////
		class CaretManager : public std::vector<Caret> ///< Manages data related to carets
		{
		public:
			fg::DrawData draw; ///< The draw data for the displayed crates

			CaretManager(fm::Size caretCount = 1); ///< Create the caret manager
			void onDraw(fg::ShaderManager &shader); ///< Draw the carets
		};

        ////////////////////////////////////////////////////////////
		void fixTextPos(TextPos &tp) const; ///< Place a possibly out of bound textpos inside
        fm::vec2 pixPosFromTextPos(TextPos tp) const; ///< Retrieve the corresponding pixel position of a textpos
        fm::Size getCharWidth(fm::Uint32 cp,fg::Glyph *retGlyph = nullptr) const; ///< Get the width of a character
        TextPos textPosFromPixPos(fm::vec2 pixPos) const; ///< Get textpos a pixel position cerresponds to
        fm::Size posFromPixPos(float pixPosX,fm::Size lineIndex) const; ///< Get pos a pixel position cerresponds to on a given line

	private:
		CaretManager m_carets; ///< The carets of the editor

        ////////////////////////////////////////////////////////////
		void buildCaretsDraw()
		{
			Mesh m;
			for (Caret c : m_carets)
			{
				m.pts.push_back(pixPosFromTextPos(c.tp) + getFrameSize());
				m.pts.push_back(pixPosFromTextPos(c.tp) + vec2(0,getMetrics().lineGap) + getFrameSize());
				m.clr.push_back(vec4::Black);
				m.clr.push_back(vec4::Black);
			}

			m_carets.draw = m;
			m_carets.draw.clearDraws();
			m_carets.draw.addDraw(0,m.pts.size(),fg::Lines);
		}
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param owner The owner of the guielement
		/// 
		/////////////////////////////////////////////////////////////
		GuiEditText(GuiContext &owner);

		/////////////////////////////////////////////////////////////
		/// @brief construct the text with given size and owner
		/// 
		/// @param owner The owner of the guielement
		/// @param size The size of the visible area
		/// 
		/////////////////////////////////////////////////////////////
		GuiEditText(GuiContext &owner,fm::vec2 size);

		/////////////////////////////////////////////////////////////
		/// @brief construct the text with given text and owner
		/// 
		/// @param owner The owner of the guielement
		/// @param lineCount Number of line displayed
		/// @param width The width of the visible area in pixels
		/// 
		/////////////////////////////////////////////////////////////
		GuiEditText(GuiContext &owner,fm::Size lineCount,float width);

		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the editor
		/// 
		/// @param size The new size
		/// 
		/////////////////////////////////////////////////////////////
		void setSize(fm::vec2s size) override;

		/////////////////////////////////////////////////////////////
		/// @brief Set the number of lines displayed
		/// 
		/// @param lineCount The number of lines displayed
		/// 
		/////////////////////////////////////////////////////////////
		void setLineCount(fm::Size lineCount);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the editor
		/// 
		/// @param pos The new position
		/// 
		/////////////////////////////////////////////////////////////
		void setPosition(fm::vec2i pos) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the text of the editor
		/// 
		/// @param str The new text
		/// 
		/////////////////////////////////////////////////////////////
		void setText(const fm::String &str);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the style of the editor
		/// 
		/// @param style The new style
		/// 
		/////////////////////////////////////////////////////////////
		void setStyle(const fg::Glyph::Style &style);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the style of the editor
		/// 
		/// @return The style
		/// 
		/////////////////////////////////////////////////////////////
		fg::Glyph::Style getStyle() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the usage of monospacing
		/// 
		/// @param mono True to use monospacing
		/// 
		/////////////////////////////////////////////////////////////
		void setMonospacing(bool mono);
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if monospacing is used
		/// 
		/// @return True iff monospacing is used
		/// 
		/////////////////////////////////////////////////////////////
		bool getMonospacing() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the text of the editor
		/// 
		/// @param newLine The string to use as newline indicator
		/// 
		/// @return The text
		/// 
		/////////////////////////////////////////////////////////////
		fm::String getText(fm::String newLine = "\n") const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of lines in the editor
		/// 
		/// @return The line count
		/// 
        ////////////////////////////////////////////////////////////
        fm::Size getLineCount() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a given line of the editor
		/// 
		/// @param index The index of the line
		/// 
		/// @return The line
		/// 
        ////////////////////////////////////////////////////////////
        const fm::String &getLine(fm::Size index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the lingth of a given line of the editor
		/// 
		/// @param index The index of the line
		/// 
		/// @return The line length
		/// 
        ////////////////////////////////////////////////////////////
        fm::Size getLineLength(fm::Size index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the font of the editor
		/// 
		/// @param font The new font
		/// 
		/////////////////////////////////////////////////////////////
		void setFont(GuiFont font);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the font of the editor
		/// 
		/// @return The font
		/// 
		/////////////////////////////////////////////////////////////
		fg::Font getFont() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the name of the font the GuiEditText uses
		/// 
		/// @return The name of the font
		/// 
		/////////////////////////////////////////////////////////////
		fm::String getFontName() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the character size of the editor
		/// 
		/// @param charSize The new character size
		/// 
		/////////////////////////////////////////////////////////////
		void setCharacterSize(fm::Size charSize);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the character size of the editor
		/// 
		/// @return The character size
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getCharacterSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the wrap mode of the editor
		/// 
		/// @param wrap The new wrap mode
		/// 
		/////////////////////////////////////////////////////////////
		void setWrap(TextWrap wrap);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the wrap mode of the editor
		/// 
		/// @return The wrap mode
		/// 
		/////////////////////////////////////////////////////////////
		TextWrap getWrapMode() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the align of the editor
		/// 
		/// @param mode The new align
		/// 
		/////////////////////////////////////////////////////////////
		void setAlign(TextAlign mode);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the align of the editor
		/// 
		/// @return The align
		/// 
		/////////////////////////////////////////////////////////////
		TextAlign getAlign() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the color of the editor
		/// 
		/// @param clr The new color
		/// 
		/////////////////////////////////////////////////////////////
		void setColor(fm::vec4 clr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the color of the editor
		/// 
		/// @return The color
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec4 getColor() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the view offset of the editor
		/// 
		/// @param viewOffset The new view offset
		/// 
		/////////////////////////////////////////////////////////////
		void setViewOffset(fm::vec2 viewOffset);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the view offset of the editor
		/// 
		/// @return The view offset
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getViewOffset() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Access one of the sprites of the editor
		/// 
		/// @return Reference to the sprite
		/// 
		/////////////////////////////////////////////////////////////
		fg::FontSprite &getCharSprite(fm::Size index);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of sprites composing the GuiEditText
		/// 
		/// @return The number of sprites
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getCharSpriteCount() const; 
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of characters a tab equals to
		/// 
		/// @return The tabsize
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getTabSize() const; 
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the number of characters a tab equals to
		/// 
		/// @param tabSize The new tabsize
		/// 
		/////////////////////////////////////////////////////////////
		void setTabSize(fm::Size tabSize); 

		/////////////////////////////////////////////////////////////
		/// @brief Set the frame size of the editor
		/// 
		/// @param frameSize The new size of the frame 
		/// 
		////////////////////////////////////////////////////////////
		void setFrameSize(fm::vec2 frameSize);

		/////////////////////////////////////////////////////////////
		/// @brief Get the frame size of the editor
		/// 
		/// @return The size of the frame 
		/// 
		////////////////////////////////////////////////////////////
		fm::vec2 getFrameSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the editor without frame
		/// 
		/// @return The size of the inner editing area
		/// 
		////////////////////////////////////////////////////////////
		fm::vec2 getViewSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the position of the view rectangle
		/// 
		/// @return The viewing position
		/// 
		////////////////////////////////////////////////////////////
        fm::vec2 getViewPos() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the view rectangle
		/// 
		/// @param pos The viewing position
		/// 
		////////////////////////////////////////////////////////////
        virtual void setViewPos(fm::vec2 pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Move the position of the view rectangle
		/// 
		/// @param delta The amount to move the viewing position
		/// 
		////////////////////////////////////////////////////////////
        virtual void moveViewPos(fm::vec2 delta);
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw the GuiEditText
		/// 
		/// @param shader The shader to use
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;

		/////////////////////////////////////////////////////////////
		/// @brief Internally handle an event
		/// 
		/// @param ev The event
		/// 
		/// @return True iff the event got handled
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool onEvent(fw::Event &ev) override;
	};

    /////////////////////////////////////////////////////////////
    GuiEditText::TextPos::TextPos(fm::Size line,fm::Size pos) : line(line),
                                                                pos(pos)
    {

    }

    /////////////////////////////////////////////////////////////
    GuiEditText::TextPos::TextPos(const fm::vec2s &vec) : line(vec.x),
                                                          pos(vec.y)
    {

    }

    /////////////////////////////////////////////////////////////
    bool GuiEditText::TextPos::operator<(const TextPos &tp) const
    {
        return (line < tp.line) || (line == tp.line && pos < tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool GuiEditText::TextPos::operator>(const TextPos &tp) const
    {
        return (line > tp.line) || (line == tp.line && pos > tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool GuiEditText::TextPos::operator<=(const TextPos &tp) const
    {
        return (line < tp.line) || (line == tp.line && pos <= tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool GuiEditText::TextPos::operator>=(const TextPos &tp) const
    {
        return (line > tp.line) || (line == tp.line && pos >= tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool GuiEditText::TextPos::operator==(const TextPos &tp) const
    {
        return line == tp.line && pos == tp.pos;
    }

    /////////////////////////////////////////////////////////////
    bool GuiEditText::TextPos::operator!=(const TextPos &tp) const
    {
        return line != tp.line || pos != tp.pos;
    }

    /////////////////////////////////////////////////////////////
    GuiEditText::TextPos GuiEditText::TextPos::operator+(const TextPos &tp) const
    {
        return TextPos(line+tp.line, pos+tp.pos);
    }

    /////////////////////////////////////////////////////////////
    GuiEditText::TextPos GuiEditText::TextPos::operator-(const TextPos &tp) const
    {
        return TextPos(line-tp.line, pos-tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool GuiEditText::TextPos::isBetween(const TextPos &p1,const TextPos &p2) const
    {
        if ((*this)==p1 || (*this)==p2)
            return true;

        if (p1<p2)
            return (*this)<p2 && p1<(*this);

        return (*this)<p1 && p2<(*this);
    }

	/////////////////////////////////////////////////////////////
	void GuiEditText::updateSprites()
	{
		if (getFont())
		{
			fm::vec2 viewSize = getViewSize();
			
			SpritesFromText(m_sprites,getText(),getStyle(),getFont(),getCharacterSize(),getAlign(),getWrapMode(),viewSize,m_viewOffset,m_clr,getPosition() + getFrameSize()).updateSprites();
			
			GuiElement::setSize(viewSize + getFrameSize()*2);	
		}
	}

	/////////////////////////////////////////////////////////////
	void GuiEditText::adaptRealSize()
	{
		if (m_lineCount)
		{
			GuiElement::setSize(vec2(m_viewSize.w,m_lineCount * getMetrics().lineGap));
		}
		else
		{
			GuiElement::setSize(m_viewSize);
		}

		updateSprites();
	}

	/////////////////////////////////////////////////////////////
	Metrics GuiEditText::getMetrics() const
	{
		getFont().setCharacterSize(getCharacterSize());

		return getFont().getMetrics();
	}
	
    /////////////////////////////////////////////////////////////
	void GuiEditText::fixTextPos(TextPos &tp) const
	{
		tp.line = std::min(tp.line,getLineCount());
		tp.pos  = std::min(tp.pos, getLineLength(tp.line));
	}

    /////////////////////////////////////////////////////////////
    fm::vec2 GuiEditText::pixPosFromTextPos(TextPos tp) const
    {
        fm::vec2 ret(0,tp.line * getMetrics().lineGap);

		fixTextPos(tp);

        C(tp.pos)
            ret.x += getCharWidth(m_lines[tp.line][i]);

        return ret;
    }

    /////////////////////////////////////////////////////////////
    fm::Size GuiEditText::getCharWidth(fm::Uint32 cp,fg::Glyph *retGlyph) const
    {
		fg::Font font = getFont();

		if (cp == ' ')
			return font.getGlyph('w').size.w + 1;

		if (cp == '\t')
			return (font.getGlyph('w').size.w + 1) * getTabSize();

		fg::Glyph glyph = font.getGlyph(cp);

		if (retGlyph)
			*retGlyph = glyph;

		return getMonospacing() ? font.getGlyph('w').size.w + 1 : glyph.size.w + 2;
    }

	/////////////////////////////////////////////////////////////
    GuiEditText::TextPos GuiEditText::textPosFromPixPos(fm::vec2 pixPos) const
    {
        fm::Size lineCount = getLineCount();
		Metrics  metrics   = getMetrics();

        if (pixPos.y < 0) pixPos.y = 0;

        fm::Size line = std::min<fm::Size>(pixPos.y / metrics.lineGap,lineCount - 1);
        fm::Size pos;

        if (pixPos.y >= std::max<float>((lineCount+5)*metrics.lineGap,getViewSize().h+getViewPos().y+metrics.lineGap*2))
        {
            pos = getLine(lineCount-1).size();
        }
        else if (pixPos.y >= lineCount*metrics.lineGap)
        {
            pos = posFromPixPos(pixPos.x,lineCount-1);
        }
        else
        {
            pos = posFromPixPos(pixPos.x,line);
        }

        return TextPos(line,pos);
    }

	/////////////////////////////////////////////////////////////
    fm::Size GuiEditText::posFromPixPos(float pixPosX,fm::Size lineIndex) const
    {
        if (pixPosX < 0)
            pixPosX = 0;

        if (lineIndex > getLineCount())
            return m_lines.back().size();

        const fm::String &line = getLine(lineIndex);

        float curw = 0;
        fm::Size ret = 0;

        while (ret < line.size())
        {
            float w = getCharWidth(line[ret]);

            if (curw <= pixPosX && curw+w >= pixPosX)
            {
                float inCharP = pixPosX - curw;

                if (inCharP >= w/2)
                    ret++;

                break;
            }

            curw += w;
            ret ++;
        }

        return ret;
    }

	/////////////////////////////////////////////////////////////
	GuiEditText::CaretManager::CaretManager(fm::Size caretCount) : vector(caretCount)
	{

	}
	
	/////////////////////////////////////////////////////////////
	void GuiEditText::CaretManager::onDraw(fg::ShaderManager &shader)
	{
		shader.draw(draw);
	}

	/////////////////////////////////////////////////////////////
	GuiEditText::GuiEditText(GuiContext &owner) : GuiElement(owner),
												  m_lines(1),
												  m_style(fg::Glyph::Regular),
												  m_lineCount(1),
												  m_wrapMode(fgui::TextWrapOff),
												  m_charSize(owner.getDefCharSize()),
												  m_tabSize(4),
												  m_monospacing(false),
												  m_align(fgui::TextAlignLeft),
												  m_font(owner.getDefaultFont())
	{
		buildCaretsDraw();
	}

	/////////////////////////////////////////////////////////////
	GuiEditText::GuiEditText(GuiContext &owner,fm::vec2 size) : 
												  GuiElement(owner,size),
												  m_lines(1),
												  m_style(fg::Glyph::Regular),
												  m_lineCount(0),
												  m_viewSize(size),
												  m_wrapMode(fgui::TextWrapOff),
												  m_charSize(owner.getDefCharSize()),
												  m_tabSize(4),
												  m_monospacing(false),
												  m_align(fgui::TextAlignLeft),
												  m_font(owner.getDefaultFont())
	{
		buildCaretsDraw();
	}

	/////////////////////////////////////////////////////////////
	GuiEditText::GuiEditText(GuiContext &owner,fm::Size lineCount,float width) : 
												  GuiElement(owner),
												  m_lines(1),
												  m_style(fg::Glyph::Regular),
												  m_lineCount(lineCount),
												  m_viewSize(width,0),
												  m_wrapMode(fgui::TextWrapOff),
												  m_charSize(owner.getDefCharSize()),
												  m_tabSize(4),
												  m_monospacing(false),
												  m_align(fgui::TextAlignLeft),
												  m_font(owner.getDefaultFont())
	{
		GuiElement::setSize(vec2(width,lineCount * getFont().getMetrics().lineGap));
		buildCaretsDraw();
	}

	/////////////////////////////////////////////////////////////
	void GuiEditText::setSize(fm::vec2s size)
	{
		if (m_lineCount == 0 && m_viewSize == size) return;

		m_lineCount = 0;
		m_viewSize  = size;
		adaptRealSize();
	}

	/////////////////////////////////////////////////////////////
	void GuiEditText::setLineCount(fm::Size lineCount)
	{
		if (m_lineCount == lineCount) return;

		m_lineCount = lineCount;
		adaptRealSize();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiEditText::setPosition(fm::vec2i pos)
	{
		if (pos == getPosition()) return;

		fm::vec2 diff = pos - getPosition();
		
		for (auto &sprite : m_sprites)
			sprite.setPosition(sprite.getPosition() + diff);
		
		GuiElement::setPosition(pos);
	}
	
	/////////////////////////////////////////////////////////////
	int GuiEditText::classify(fm::Uint32 character) const
	{
        if (fm::String(".\\(){}[]<>+-*=/&|@%^~\'\"?!$#:;,").find(character) != fm::String::npos)
            return 1;

        if (fm::String(" \t\r\n").find(character) != fm::String::npos)
            return 2;

        return 3;
	}

	/////////////////////////////////////////////////////////////
	bool GuiEditText::isNewLine(const fm::String &str,fm::Size pos) const
	{
		// \n \r \r\n
		return str[pos] == '\n' || (str[pos] == '\r' && (pos+1 == str.size() || str[pos+1] != '\n'));
	}

	/////////////////////////////////////////////////////////////
	void GuiEditText::setText(const fm::String &str)
	{
		m_lines.resize(1);

		fm::Size lineBeg = 0;
		for (fm::Size i=0;i<str.size();++i)
		{
			if (isNewLine(str,i))
			{
				m_lines.back() = str.substr(lineBeg,i-lineBeg);
				m_lines.push_back(fm::String());
				lineBeg = i+1;
			}
		}

		if (lineBeg != str.size())
		{
			m_lines.back() = str.substr(lineBeg);
		}

		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiEditText::setStyle(const fg::Glyph::Style &style)
	{
		if (m_style == style) return;

		m_style = style;

		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Glyph::Style GuiEditText::getStyle() const
	{
		return m_style;
	}

	/////////////////////////////////////////////////////////////
	void GuiEditText::setMonospacing(bool mono)
	{
		if (m_monospacing == mono) return;

		m_monospacing = mono;
		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	bool GuiEditText::getMonospacing() const
	{
		return m_monospacing;
	}
	
	/////////////////////////////////////////////////////////////
	fm::String GuiEditText::getText(fm::String newLine) const
	{
		String ret = m_lines[0];
	
		for (fm::Size i=1;i<m_lines.size();++i)
		{
			ret += newLine + m_lines[i];
		}

		return ret;
	}

	////////////////////////////////////////////////////////////
	fm::Size GuiEditText::getLineCount() const
	{
		return m_lines.size();
	}

	////////////////////////////////////////////////////////////
	const fm::String &GuiEditText::getLine(fm::Size index) const
	{
		return m_lines[index];
	}

	////////////////////////////////////////////////////////////
	fm::Size GuiEditText::getLineLength(fm::Size index) const
	{
		return m_lines[index].size();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiEditText::setFont(GuiFont font)
	{
		m_font = font;

		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	fg::Font GuiEditText::getFont() const
	{
		return m_font.getFont(getOwnerContext());
	}
	
	/////////////////////////////////////////////////////////////
	fm::String GuiEditText::getFontName() const
	{
		return m_font.fontName;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiEditText::setCharacterSize(fm::Size charSize)
	{
		if (m_charSize == charSize) return;

		m_charSize = charSize;

		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GuiEditText::getCharacterSize() const
	{
		return m_charSize;
	}

	/////////////////////////////////////////////////////////////
	void GuiEditText::setWrap(TextWrap mode)
	{
		if (m_wrapMode == mode) return;

		m_wrapMode = mode;
		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	TextWrap GuiEditText::getWrapMode() const
	{
		return m_wrapMode;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiEditText::setAlign(TextAlign mode)
	{
		if (m_align == mode) return;

		m_align = mode;
		updateSprites();
	}

	/////////////////////////////////////////////////////////////
	void GuiEditText::setColor(fm::vec4 color)
	{
		if (color == m_clr) return;
		
		m_clr = color;
		
		for (auto &sprite : m_sprites)
			sprite.setColor(m_clr);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec4 GuiEditText::getColor() const
	{
		return m_clr;
	}
	
	/////////////////////////////////////////////////////////////
	TextAlign GuiEditText::getAlign() const
	{
		return m_align;
	}
		
	/////////////////////////////////////////////////////////////
	void GuiEditText::setViewOffset(fm::vec2 viewOffset)
	{
		if (m_viewOffset == viewOffset) return;

		m_viewOffset = viewOffset;
		updateSprites();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GuiEditText::getViewOffset() const
	{
		return m_viewOffset;
	}
	
	/////////////////////////////////////////////////////////////
	fg::FontSprite &GuiEditText::getCharSprite(fm::Size index)
	{
		return m_sprites[index];
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GuiEditText::getCharSpriteCount() const
	{
		return m_sprites.size();
	}

    /////////////////////////////////////////////////////////////
	fm::Size GuiEditText::getTabSize() const
	{
		return m_tabSize;
	}

    /////////////////////////////////////////////////////////////
	void GuiEditText::setTabSize(fm::Size tabSize)
	{
		if (m_tabSize == tabSize) return;

		m_tabSize = tabSize;
		updateSprites();
	}
	
	////////////////////////////////////////////////////////////
	void GuiEditText::setFrameSize(fm::vec2 frameSize)
	{
		if (m_frameSize == frameSize) return;

		setSize(getViewSize() + frameSize*2);
		
		m_frameSize = frameSize;
		updateSprites();
	}
	
	////////////////////////////////////////////////////////////
	fm::vec2 GuiEditText::getFrameSize() const
	{
		return m_frameSize;
	}
	
	////////////////////////////////////////////////////////////
	fm::vec2 GuiEditText::getViewSize() const
	{
		return getSize() - getFrameSize()*2;
	}

	////////////////////////////////////////////////////////////
	fm::vec2 GuiEditText::getViewPos() const
	{
		return m_viewPos;
	}	

    ////////////////////////////////////////////////////////////
    void GuiEditText::setViewPos(fm::vec2 p)
    {
        if (p.x < 0) p.x = 0;
        if (p.y < 0) p.y = 0;

		Metrics metrics = getMetrics();

        float mx = (getLineCount() - 1) * metrics.lineGap;

        if (p.y > mx) p.y = mx;

        m_viewPos = p;

        updateSprites();
    }

    ////////////////////////////////////////////////////////////
    void GuiEditText::moveViewPos(fm::vec2 delta)
    {
        setViewPos(m_viewPos + delta);
    }
	
	/////////////////////////////////////////////////////////////
	void GuiEditText::onDraw(fg::ShaderManager &shader)
	{
		(void)shader;

		m_carets.onDraw(shader);
	}

	/////////////////////////////////////////////////////////////
	bool GuiEditText::onEvent(fw::Event &ev)
	{
		if (ev.type == Event::TextEntered)
		{
			setText(getText() + ev.text.utf8character);
		}

		return GuiElement::onEvent(ev);
	}
}

int main()
{
	GuiWindow win(vec2(640,480),"GuiEditText");

	auto editor = new GuiEditText(win,win.getSize() - vec2(20));
	editor->setText("lol\nk");
	editor->setFrameSize(vec2(10,10));

	win.getMainLayout().addChildElement(editor);

	win.runGuiLoop();
}
