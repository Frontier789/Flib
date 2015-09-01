////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_BASICCONTROL_HPP_INCLUDED
#define FRONTIER_BASICCONTROL_HPP_INCLUDED

#include <FRONTIER/Graphics/Metrics.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/Gui/Widget.hpp>

#define FRONTIER_BASICCONTROL

namespace fg
{
	class Texture;
	class Glyph;
	class Font;
}

namespace Fgui
{
	class BasicControl : public Widget
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "BasicControl"

	protected:
		class DragMode
		{
		public:
			enum {
				NoDrag   = 0,
				FixedSel = 1,
				ResetSel = 2,
				KeepWord = 3
			};
		};


	public:
		class TextPos
		{
		public:
			fm::Size line;
			fm::Size pos;

			TextPos(fm::Size line = 0,fm::Size pos = 0);

			bool operator<(const TextPos &tp) const;
			bool operator>(const TextPos &tp) const;
			bool operator<=(const TextPos &tp) const;
			bool operator>=(const TextPos &tp) const;
			bool operator==(const TextPos &tp) const;
			bool operator!=(const TextPos &tp) const;

			bool isBetween(const TextPos &p1,const TextPos &p2) const;
		};

		class Caret
		{
		public:
			TextPos tp;
			float savedWidth;
			TextPos sel;

			Caret(const TextPos &tp = TextPos(),float savedWidth = 0,const TextPos &sel = TextPos());
		};

		class CharColor
		{
		public:
			fm::vec4 clr;
			fm::vec4 bkclr;

			CharColor(fm::vec4 clr = fm::vec4::Black,fm::vec4 bkclr = fm::vec4(0,0,0,0));
		};

	protected:
		std::deque<std::deque<CharColor> > m_charColors;
		std::deque<fm::String> m_lines;
		std::deque<Caret> m_carets;

		unsigned int m_charSize;
		const fg::Texture *m_tex;
		fm::vec2 m_pixViewPos;
		fm::vec2 m_viewPos;

		fg::Buffer m_vbo;
		fg::Buffer m_ibo;
		fg::Buffer m_vboBkg;
		fg::Buffer m_iboBkg;
		fg::Buffer m_caretVbo;
		fg::Buffer m_caretIbo;

		fm::Size m_vertexCount;
		fm::Size m_vertexCountBkg;
		fm::Size m_vertexCountCaret;
		fm::Size m_vertexCountCaretBkg;

		fg::Metrics m_metrics;
		fm::mat4 m_transform;
		fg::Font *m_font;

		bool m_needRecalcCarets;
		bool m_needBuildVertices;
		bool m_needSetupPosition;
		bool m_needSetViewCaret;
		bool m_insertMode;
		bool m_editable;

		fm::Size m_dragCaret;
		TextPos m_wordBeg;
		TextPos m_wordEnd;
		int m_dragMode;

		fm::Clock m_clickClk;
		TextPos m_lastClick;

		fm::vec2 m_lastRightClick;
		bool m_rightDown;

		/////////////////////////////////////////////////////////////
		void setupMemberData();

		/////////////////////////////////////////////////////////////
		void uniquifyCarets();



		/////////////////////////////////////////////////////////////
		virtual void buildVertices();

		/////////////////////////////////////////////////////////////
		virtual void setupPosition();

		/////////////////////////////////////////////////////////////
		virtual void recalcCarets();

		/////////////////////////////////////////////////////////////
		virtual int getCharType(fm::Uint32 cp);

	public:

		/////////////////////////////////////////////////////////////
		virtual float getCharWidth(fm::Uint32 cp,fg::Glyph *retGlyph = fm::nullPtr);

		/////////////////////////////////////////////////////////////
		virtual void addText(Caret &caret,const fm::String &str);

		/////////////////////////////////////////////////////////////
		virtual void newLine(Caret &caret);

		/////////////////////////////////////////////////////////////
		virtual void delText(Caret &caret,int amount);

		/////////////////////////////////////////////////////////////
		virtual fm::Size getWordLength(const TextPos &tp,bool left,bool includeNewLine = true);

		/////////////////////////////////////////////////////////////
		virtual void setCaret(Caret &caret,const TextPos &tp,bool resetSel = true);

		/////////////////////////////////////////////////////////////
		virtual void setCaret(Caret &caret,fm::Size line,fm::Size pos,bool resetSel = true);

		/////////////////////////////////////////////////////////////
		virtual void addCaret(TextPos tp);

		/////////////////////////////////////////////////////////////
		virtual void addCaret(fm::Size line,fm::Size pos);

		/////////////////////////////////////////////////////////////
		virtual void delCaret(fm::Size caretIndex);

		/////////////////////////////////////////////////////////////
		virtual void moveCaretPos(Caret &caret,int dpos,bool resetSel = true);

		/////////////////////////////////////////////////////////////
		virtual void moveCaretLine(Caret &caret,int dline,bool resetSel = true);

		/////////////////////////////////////////////////////////////
		virtual fm::String getLeadingWhiteSpaces(fm::Size lineIndex);

		/////////////////////////////////////////////////////////////
		virtual fm::Size lineFromPixPos(fm::vec2 pixPos);

		/////////////////////////////////////////////////////////////
		virtual fm::Size posFromPixPos(fm::vec2 pixPos);

		/////////////////////////////////////////////////////////////
		virtual TextPos textPosFromPixPos(fm::vec2 pixPos);

		/////////////////////////////////////////////////////////////
		virtual fm::vec2 pixPosFromTextPos(const TextPos &tp);

		/////////////////////////////////////////////////////////////
		virtual int charBetween(const TextPos &tp1,const TextPos &tp2);

		/////////////////////////////////////////////////////////////
		virtual void setViewCaret();

		/////////////////////////////////////////////////////////////
		virtual void setInsertMode(bool insertMode = true);

		/////////////////////////////////////////////////////////////
		virtual bool getInsertMode();

		/////////////////////////////////////////////////////////////
		virtual bool handleShortcut(fw::KeyboardEvent key);

		/////////////////////////////////////////////////////////////
		virtual void duplicateLine(fm::Size line);

		/////////////////////////////////////////////////////////////
		virtual void duplicateSelected(Caret &caret);

		/////////////////////////////////////////////////////////////
		virtual void setViewPos(const fm::vec2 &viewPos);

		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getViewPos();

		/////////////////////////////////////////////////////////////
		virtual void onDrawChars();

		/////////////////////////////////////////////////////////////
		virtual void onDrawCharsBkg();

		/////////////////////////////////////////////////////////////
		virtual void onDrawCarets();

		/////////////////////////////////////////////////////////////
		virtual void onDrawCaretsBkg();

		/////////////////////////////////////////////////////////////
		virtual void extendSelsToLine();

		/////////////////////////////////////////////////////////////
		virtual void moveLines(int dir);

	public:

		/////////////////////////////////////////////////////////////
		BasicControl(const std::string &name = "NoName",
					 const Anchor &anchor = Anchor(),
					 const fm::vec2 &size = fm::vec2(),
					 Widget *parent = fm::nullPtr,
					 const fm::String &defText = "0",
					 fg::Font *font = fm::nullPtr,
					 fm::Size characterSize = 20,
					 bool editable = true);

		/////////////////////////////////////////////////////////////
		virtual void setAnchor(const Anchor &anchor);

		/////////////////////////////////////////////////////////////
		virtual void setParent(Widget *parent);

		/////////////////////////////////////////////////////////////
		virtual void setSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		virtual void onDraw();

		/////////////////////////////////////////////////////////////
		virtual void onUpdate();

		/////////////////////////////////////////////////////////////
		virtual void onParentChange();

		/////////////////////////////////////////////////////////////
		virtual void setContent(const fm::String &str);

		/////////////////////////////////////////////////////////////
		virtual void setEditable(bool enable = true);

		/////////////////////////////////////////////////////////////
		virtual bool getEditable() const;

		/////////////////////////////////////////////////////////////
		virtual bool handleEvent(const fw::Event &ev);

		/////////////////////////////////////////////////////////////
		virtual const std::string &getRealType() const;
	};
}

#endif // FRONTIER_BASICCONTROL_HPP_INCLUDED


