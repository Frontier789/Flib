#ifndef FRONTIER_EDITTEXT_HPP_INCLUDED
#define FRONTIER_EDITTEXT_HPP_INCLUDED
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/Gui/Widget.hpp>
#define FRONTIER_EDITTEXT

namespace fgui
{
    class EditText : public Widget
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
        fg::DrawData m_charsDraw;
        fg::DrawData m_selectDraw;
        fg::DrawData m_caretsDraw;
        std::deque<fm::String> m_lines;
        std::vector<Caret> m_carets;
        const fg::Texture *m_tex;
        std::deque<Undo> m_undos;
        std::deque<Undo> m_redos;
        const fg::Font *m_font;
        fg::Metrics m_metrics;
        fm::vec2 m_pixViewPos;
        fm::Size m_charSize;
        fm::vec2 m_viewPos;
        bool m_recordUndo;
        bool m_monoSpace;
        bool m_editable;
        bool m_freeView;
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
        fm::Size getCharWidth(fm::Uint32 cp,fg::Glyph *retGlyph = fm::nullPtr) const;
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

    public:
        ////////////////////////////////////////////////////////////
        EditText(const fm::vec2 &pos,
                 const fm::vec2 &size,
                 const fm::String &id = "unnamed",
                 Layout *parent = fm::nullPtr,
                 const fm::String &text = "",
                 const fg::Font *font = fm::nullPtr,
                 fm::Size characterSize = 12);

        ////////////////////////////////////////////////////////////
        EditText(const fm::String &text,
                 const fm::vec2 &size,
                 fg::Font *font = fm::nullPtr,
                 fm::Size characterSize = 12);

        ////////////////////////////////////////////////////////////
        virtual bool handleEvent(const fw::Event &ev);
        virtual bool handleKey(fw::KeyboardEvent key);

        ////////////////////////////////////////////////////////////
        virtual void handleEnter(fm::vec2 p);
        virtual void handleLeave(fm::vec2 p);
        virtual void handleHover(fm::vec2 p);
        virtual void handlePress(fm::vec2 p,fw::Mouse::Button btn);
        virtual void handleRelease(fm::vec2 p,fw::Mouse::Button btn);

        ////////////////////////////////////////////////////////////
        virtual void onDraw(fg::ShaderManager &shader) const;
        virtual void onUpdate(float dt = 0.f);

        ////////////////////////////////////////////////////////////
        virtual void setSize(const fm::vec2 &size);

        ////////////////////////////////////////////////////////////
        virtual void setText(const fm::String &text);
        fm::String getText(const fm::String &eol = "\n") const;

        ////////////////////////////////////////////////////////////
        virtual void setFont(const fg::Font *font);
        const fg::Font *getFont() const;

        ////////////////////////////////////////////////////////////
        virtual void setFreeView(bool freeView);
        bool getFreeView() const;

        ////////////////////////////////////////////////////////////
        virtual void setInsert(bool doesInsert);
        bool getInsert() const;

        ////////////////////////////////////////////////////////////
        virtual void setMonoSpace(bool isMonoSpaced);
        bool getMonoSpace() const;

        ////////////////////////////////////////////////////////////
        virtual void setEditable(bool isEditable);
        bool getEditable() const;

        ////////////////////////////////////////////////////////////
        virtual void setCharSize(fm::Size charSize);
        fm::Size getCharSize() const;

        ////////////////////////////////////////////////////////////
        fm::vec2 getViewPos() const;
        virtual void setViewPos(fm::vec2 p);
        virtual void moveViewPos(fm::vec2 delta);

        ////////////////////////////////////////////////////////////
        fm::Size getLineCount() const;
        fm::String &getLine(fm::Size index);
        const fm::String &getLine(fm::Size index) const;

        ////////////////////////////////////////////////////////////
        fm::String getLeadingWhiteSpaces(fm::Size lineIndex) const;
        virtual int getCharType(fm::Uint32 cp) const;

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
        virtual void insertText(Caret &caret,const fm::String &text);
        virtual void delText(Caret &caret,int amount);
        virtual void newLine(Caret &caret);

        /////////////////////////////////////////////////////////////
        virtual void moveCaretLine(Caret &caret,int deltaLine,bool resetSel = true,bool advanceSel = false);
        virtual void moveCaretPos( Caret &caret,int deltaPos, bool resetSel = true,bool advanceSel = false);
        virtual void addCaret(const TextPos &tp);
        virtual void setCaret(Caret &caret,const TextPos &tp,const TextPos &sel);
    };
}

#endif // FRONTIER_EDITTEXT_HPP_INCLUDED

