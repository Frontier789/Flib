#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

namespace fgui
{
	class EditText : public GuiElement
	{
		struct LineData {
			std::vector<fm::Uint32> str;
		};
		struct TextPos {
			fm::Size linep;
			fm::Size charp;
		};
		struct TextInt {
			TextPos beg;
			TextPos end;
		};
		std::deque<LineData> m_lines;
		std::vector<fg::FontSprite> m_sprites;
		fg::Font m_font;
	public:
		EditText(GuiContext &cont,vec2i size);
		
		void setText(const fm::String &str);
		
		static std::deque<LineData> strToData(const fm::String &str);
		void createSpritesFromText();
		void createSprites(const std::vector<TextInt> &view);
		fm::Size getLineCount() const;
		fm::Size getCharsInLine(fm::Size line) const;
		fm::Size getCharsInInterval(const TextInt &interval) const;
		fm::Size getCharsInView(const std::vector<TextInt> &view) const;
		fm::String getViewString(const std::vector<TextInt> &view) const;
		int getCharWidth(fm::Uint32 c) const;
		static bool isNewLine(const String &str,fm::Size p);
		
		
		Time t;
		void onUpdate(const fm::Time &dt) override
		{
			t += dt;
			for (auto &spr : m_sprites) {
				float dy = sin(t.s());
				spr.setPosition(spr.getPosition() + vec3(0,dy,0));
			}
		}
	};
	
	EditText::EditText(GuiContext &cont,vec2i size) : 
		GuiElement(cont,size),
		m_font(cont.getDefaultFont()) 
	{}
		
	void EditText::setText(const fm::String &str)
	{
		m_lines = strToData(str);
		
		createSpritesFromText();
	}
	
	bool EditText::isNewLine(const String &str,fm::Size p)
	{
		return (str[p] == '\n' && (!p || str[p-1] != '\r')) || 
			   (str[p] == '\r' && p+1<str.size() && str[p-1] == '\n');
	}
	
	std::deque<EditText::LineData> EditText::strToData(const fm::String &str)
	{
		std::deque<LineData> ret;
		ret.push_back(LineData());
		
		// \n \r\n
		for (fm::Size i=0;i<str.size();++i) {
			if (isNewLine(str,i))
				ret.push_back(LineData());
			else if (str[i] != '\r')
				ret.back().str.push_back(str[i]);
		}
		
		return ret;
	}
	
	fm::Size EditText::getLineCount() const
	{
		return m_lines.size();
	}
	
	fm::Size EditText::getCharsInLine(fm::Size line) const
	{
		if (line >= getLineCount()) return 0;
		
		return m_lines[line].str.size();
	}
	
	void EditText::createSpritesFromText()
	{
		std::vector<TextInt> view;
		view.resize(getLineCount());
		C(view.size())
			view[i] = TextInt{{i,0},{i,getCharsInLine(i)}};
		
		createSprites(view);
	}
	
	fm::Size EditText::getCharsInInterval(const TextInt &interval) const
	{
		const auto &beg = interval.beg;
		const auto &end = interval.end;
		
		if (beg.linep == end.linep)
			return end.charp - beg.charp;
		
		fm::Size chars = 0;
		chars += end.charp; // last line
		chars += getCharsInLine(beg.linep) - beg.charp; // first line
		chars += end.linep - beg.linep; // newline chars
		
		for (fm::Size i=beg.linep+1;i<end.linep;++i)
			chars += getCharsInLine(i);
		
		return chars;
	}
	
	fm::Size EditText::getCharsInView(const std::vector<TextInt> &view) const
	{
		fm::Size s = 0;
		for (auto &interval : view) 
			s += getCharsInInterval(interval);
		
		return s;
	}
	
	fm::String EditText::getViewString(const std::vector<TextInt> &view) const
	{
		fm::String str;
		for (auto &interval : view) {
			auto &line_str = m_lines[interval.beg.linep].str;
			str += fm::String(line_str.begin() + interval.beg.charp,line_str.begin() + interval.end.charp);
		}
		return str;
	}
	
	int EditText::getCharWidth(fm::Uint32 c) const
	{
		if (c == ' ') c = 'a';
		
		return m_font.getGlyphRect(c).size.w;
	}
	
	void EditText::createSprites(const std::vector<TextInt> &view)
	{
		m_sprites = m_font.getSprites(getViewString(view),fg::Glyph::Regular);
		
		if (view.empty()) return;
		
		Metrics m = m_font.getMetrics();
		
		Size vline = 0;
		Size vchar = view[0].beg.charp;
		vec2i head = vec2i(0,m.maxH);
		for (auto &spr : m_sprites) {
			
			while (vline < view.size() && vchar == view[vline].end.charp) {
				++vline;
				vchar = view[vline].beg.charp;
				head.x = 0;
				head.y += m.lineGap;
			}
			
			auto imgId = spr.getImageID();
			auto grect = m_font.getGlyphRect(imgId.codePoint,imgId.style);
			
			vec2i p = head + grect.pos;
			
			head.x += getCharWidth(imgId.codePoint);
			
			spr.setPosition(p);
			spr.setColor(vec4::Black);
			++vchar;
		}
	}
}

int main()
{
	GuiWindow win(vec2(640,480),"GuiEditText");

	auto editor = new EditText(win,win.getSize() - vec2(20));

	win.getMainLayout().addChildElement(editor);

	String str = "To be, or \nnot to be, that\n is the question here.";
	Size n=0;
	
	Clock cc;
	Clock clk;
	win.runGuiLoop([&](){
		if (n < str.size() && clk.s() > (.1 + .4 * (n && str[n-1] == ','))) clk.restart(), editor->setText(str.substr(0,++n));
	});
}
