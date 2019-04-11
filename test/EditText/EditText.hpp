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
#ifndef FRONTIER_EDITTEXT_HPP_INCLUDED
#define FRONTIER_EDITTEXT_HPP_INCLUDED
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/System/Ref.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/GuiText.hpp>
#define FRONTIER_EDITTEXT

#include <deque>
#include <vector>

namespace fgui
{
    class EditText : public GuiElement, public ClickListener
    {
    public:
        ////////////////////////////////////////////////////////////
        class TextPos
        {
        public:
            fm::Size line;
            fm::Size pos;

            TextPos(fm::Size line = 0,fm::Size pos = 0);
            TextPos(const fm::vec2s &vec);

            bool operator<(const TextPos &tp) const;
            bool operator>(const TextPos &tp) const;
            bool operator<=(const TextPos &tp) const;
            bool operator>=(const TextPos &tp) const;
            bool operator==(const TextPos &tp) const;
            bool operator!=(const TextPos &tp) const;
            TextPos operator+(const TextPos &tp) const;
            TextPos operator-(const TextPos &tp) const;

            bool isBetween(const TextPos &p1,const TextPos &p2) const;
        };

        ////////////////////////////////////////////////////////////
        class Caret
        {
        public:
            TextPos tp;
            float savedWidth;
            TextPos sel;

            Caret(const TextPos &tp = TextPos(),float savedWidth = 0,const TextPos &sel = TextPos());
        };

        ////////////////////////////////////////////////////////////
        class UndoStep
        {
        public:
            enum Action {
                InsertText = 1,
                DeleteText = 5,
                InsertLine = 2,
                DeleteLine = 4,
                MoveLine   = 3,
                ActionInvert = 6
            };

            UndoStep(Action action = InsertText,fm::String text = "",TextPos tp = TextPos());

            Action action;
            fm::String text;
            TextPos tp;
        };

        ////////////////////////////////////////////////////////////
        class Undo
        {
        public:
            std::deque<UndoStep> steps;
            std::vector<Caret> caretsBefore;
            std::vector<Caret> caretsAfter;
        };

        ////////////////////////////////////////////////////////////
        enum DragMode {
            NoDrag       = 0,
            KeepSelDrag  = 1,
            KeepWordDrag = 2
        };

    protected:
        ////////////////////////////////////////////////////////////
		fm::Delegate<fm::vec4,fm::Time> m_blinkCallback;
        std::vector<fg::FontSprite> m_sprites;
        std::deque<fm::String> m_lines;
        std::vector<Caret> m_carets;
        fg::DrawData m_selectDraw;
        fg::DrawData m_caretsDraw;
        std::deque<Undo> m_undos;
        std::deque<Undo> m_redos;
		fm::vec4 m_cursorColor;
        fg::Metrics m_metrics;
        fm::vec2 m_pixViewPos;
		fm::vec4 m_caretColor;
		fm::vec2 m_frameSize;
		fm::Clock m_blinkClk;
		fm::vec4 m_highlight;
        fm::Size m_charSize;
        fm::vec2 m_viewPos;
        bool m_recordUndo;
		fg::Sprite m_bckg;
		fm::vec4 m_color;
        bool m_monoSpace;
        bool m_editable;
        bool m_freeView;
        GuiFont m_font;
        bool m_insert;

        ////////////////////////////////////////////////////////////
        bool      m_rightDrag;
        fm::vec2  m_lastRight;
        DragMode  m_dragMode;
        TextPos   m_dragTP0,m_dragTP1;
        TextPos   m_lastClick;
        fm::Clock m_clickClk;

        ////////////////////////////////////////////////////////////
        bool m_recalcChars;
        bool m_recalcCarets;
        bool m_recalcSelect;

        ////////////////////////////////////////////////////////////
        virtual void recalcChars();
        virtual void recalcCarets();
        virtual void recalcSelect();

        ////////////////////////////////////////////////////////////
        fm::vec2 pixPosFromTextPos(const TextPos &tp) const;
        fm::Size getCharWidth(fm::Uint32 cp,fg::Glyph *retGlyph = nullptr) const;
        TextPos textPosFromPixPos(fm::vec2 pixPos) const;
        fm::Size lineFromPixPos(fm::vec2 pixPos) const;
        fm::Size posFromPixPos(fm::vec2 pixPos) const;
        fm::Size posFromPixPos(float pixPosX,fm::Size lineIndex) const;

        ////////////////////////////////////////////////////////////
        void remDoubleCarets();
        void fixCaret(Caret &caret);
        void sortCarets(bool ascending);

        ////////////////////////////////////////////////////////////
        void applyUndo(const Undo &undo,bool redo = false);
        void moveLine(fm::Size line,fm::Size newIndex);

        virtual void onTextChanged();
		virtual fm::vec4 defBlinkFunc(fm::Time t) const;
        void updateCaretColors();
        
        ////////////////////////////////////////////////////////////
        void updateMetrics();

    public:
        ////////////////////////////////////////////////////////////
        EditText(GuiContext &cont,
                 fm::vec2 size,
                 fm::String text = "");

        ////////////////////////////////////////////////////////////
        bool onEvent(fw::Event &ev) override;
        bool onKey(fw::KeyboardEvent key);

        ////////////////////////////////////////////////////////////
        void onMouseEnter(fm::vec2 p) override;
        void onMouseLeave(fm::vec2 p) override;
        void onMouseMove(fm::vec2 p,fm::vec2 prevP) override;
        void onPress(fw::Mouse::Button button,fm::vec2 p) override;
        void onRelease(fw::Mouse::Button button,fm::vec2 p) override;

        ////////////////////////////////////////////////////////////
        void onDraw(fg::ShaderManager &shader) override;
		void onUpdate(const fm::Time &dt = fm::Time::Zero) override;

        ////////////////////////////////////////////////////////////
        void setSize(fm::vec2s size) override;
		void setPosition(fm::vec2i pos) override;

        ////////////////////////////////////////////////////////////
		void setBackground(fg::Sprite bckg,bool setFrame = true);
		void setBackground(std::string imgId,bool setFrame = true);
		void setFrameSize(fm::vec2 frameSize);
		fm::vec2 getFrameSize() const;
		fm::vec2 getInnerSize() const;

        ////////////////////////////////////////////////////////////
        void setText(const fm::String &text);
        fm::String getText(fm::String eol = "\n") const;

        ////////////////////////////////////////////////////////////
		void setFont(GuiFont font);
        fg::Font getFont() const;

        ////////////////////////////////////////////////////////////
        void setFreeView(bool freeView);
        bool getFreeView() const;

        ////////////////////////////////////////////////////////////
        void setInsert(bool doesInsert);
        bool getInsert() const;

        ////////////////////////////////////////////////////////////
        void setMonoSpace(bool isMonoSpaced);
        bool getMonoSpace() const;

        ////////////////////////////////////////////////////////////
        void setEditable(bool isEditable);
        bool getEditable() const;

        ////////////////////////////////////////////////////////////
        void setCharSize(fm::Size charSize);
        fm::Size getCharSize() const;

        ////////////////////////////////////////////////////////////
        void setColor(fm::vec4 clr);
        fm::vec4 getColor() const;

        ////////////////////////////////////////////////////////////
        void setHighlight(fm::vec4 clr);
        fm::vec4 getHighlight() const;
        
        ////////////////////////////////////////////////////////////
        void setCaretColor(fm::vec4 clr);
        fm::vec4 getCaretColor() const;
        
        ////////////////////////////////////////////////////////////
		void setBlinkCallback(fm::Delegate<fm::vec4,fm::Time> callback,fm::vec4 caretColor = fm::vec4::White);

        ////////////////////////////////////////////////////////////
        fm::vec2 getViewPos() const;
        void setViewPos(fm::vec2 p);
        void moveViewPos(fm::vec2 delta);

        ////////////////////////////////////////////////////////////
        fm::Size getLineCount() const;
        fm::String &getLine(fm::Size index);
        const fm::String &getLine(fm::Size index) const;

        ////////////////////////////////////////////////////////////
        fm::String getLeadingWhiteSpaces(fm::Size lineIndex) const;
        int getCharType(fm::Uint32 cp) const;

        ////////////////////////////////////////////////////////////
        void bringCaretsToView();

        ////////////////////////////////////////////////////////////
        int getWordLength(const TextPos &tp,bool left,bool enableSpaceJump = true) const;

        ////////////////////////////////////////////////////////////
        void undo();
        void redo();
        void addUndoStep(UndoStep step);
        void addUndo(UndoStep::Action action);
        void finishUndo();
        void addUndo();

        /////////////////////////////////////////////////////////////
        void insertText(Caret &caret,const fm::String &text);
        void delText(Caret &caret,int amount);
        void newLine(Caret &caret);

        /////////////////////////////////////////////////////////////
        void moveCaretLine(Caret &caret,int deltaLine,bool resetSel = true,bool advanceSel = false);
        void moveCaretPos( Caret &caret,int deltaPos, bool resetSel = true,bool advanceSel = false);
        void addCaret(const TextPos &tp);
        void setCaret(Caret &caret,const TextPos &tp,const TextPos &sel);
    };
}

#endif // FRONTIER_EDITTEXT_HPP_INCLUDED

