#include <FRONTIER/Graphics/IndexArrayHolder.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Util/Clipboard.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/Gui/EditText.hpp>
#include <algorithm>

#include "TextToDraw.hpp"

namespace fgui
{
    /////////////////////////////////////////////////////////////
    EditText::TextPos::TextPos(fm::Size line,fm::Size pos) : line(line),
                                                             pos(pos)
    {

    }

    /////////////////////////////////////////////////////////////
    EditText::TextPos::TextPos(const fm::vec2s &vec) : line(vec.x),
                                                       pos(vec.y)
    {

    }

    /////////////////////////////////////////////////////////////
    bool EditText::TextPos::operator<(const TextPos &tp) const
    {
        return (line < tp.line) || (line == tp.line && pos < tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool EditText::TextPos::operator>(const TextPos &tp) const
    {
        return (line > tp.line) || (line == tp.line && pos > tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool EditText::TextPos::operator<=(const TextPos &tp) const
    {
        return (line < tp.line) || (line == tp.line && pos <= tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool EditText::TextPos::operator>=(const TextPos &tp) const
    {
        return (line > tp.line) || (line == tp.line && pos >= tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool EditText::TextPos::operator==(const TextPos &tp) const
    {
        return line == tp.line && pos == tp.pos;
    }

    /////////////////////////////////////////////////////////////
    bool EditText::TextPos::operator!=(const TextPos &tp) const
    {
        return line != tp.line || pos != tp.pos;
    }

    /////////////////////////////////////////////////////////////
    EditText::TextPos EditText::TextPos::operator+(const TextPos &tp) const
    {
        return TextPos(line+tp.line, pos+tp.pos);
    }

    /////////////////////////////////////////////////////////////
    EditText::TextPos EditText::TextPos::operator-(const TextPos &tp) const
    {
        return TextPos(line-tp.line, pos-tp.pos);
    }

    /////////////////////////////////////////////////////////////
    bool EditText::TextPos::isBetween(const TextPos &p1,const TextPos &p2) const
    {
        if ((*this)==p1 || (*this)==p2)
            return true;

        if (p1<p2)
            return (*this)<p2 && p1<(*this);

        return (*this)<p1 && p2<(*this);
    }


    ////////////////////////////////////////////////////////////
    EditText::Caret::Caret(const TextPos &tp,float savedWidth,const TextPos &sel) : tp(tp),
                                                                                    savedWidth(savedWidth),
                                                                                    sel(sel)
    {

    }

    ////////////////////////////////////////////////////////////
    EditText::UndoStep::UndoStep(Action action,fm::String text,TextPos tp) : action(action),
                                                                             tp(tp)
    {
        text.swap(this->text);
    }

    ////////////////////////////////////////////////////////////
    void EditText::recalcChars()
    {
        m_charsDraw.reset();

        if (!m_font) return;

       std::vector<fm::String const *> ptr(m_lines.size());

        C(m_lines.size())
            ptr[i] = &m_lines[i];

        priv::getDrawFromText(m_charsDraw,&ptr[0],m_lines.size(),
                              m_font,m_charSize,&m_tex,
                              fm::nullPtr,fm::rect2i(getViewPos(),getSize()),
                              fm::vec4::Black,m_monoSpace ? 1 : 0);
    }

    ////////////////////////////////////////////////////////////
    void EditText::recalcSelect()
    {
        m_selectDraw.reset();

        if (!m_font) return;

        std::vector<fm::vec2> pts;
        std::vector<fm::vec4> clr;
        std::vector<fm::Uint32> inds;
        fm::Size quadID = 0;

        fm::vec2 offset = -getViewPos();

        fm::vec2 size = getSize();

        fm::Size mnLine = fm::max(-offset.y/m_metrics.lineGap,0.f);
        fm::Size mxLine = fm::max((-offset.y+size.h)/m_metrics.lineGap,0.f);

        C(m_carets.size())
        {
            Caret &caret = m_carets[i];

            TextPos from  = fm::min(caret.sel,caret.tp);
            TextPos until = fm::max(caret.sel,caret.tp);

            fm::Size untilLine = until.line;

            if (from == until) continue;

            if (from.line < mnLine)
            {
                from.line = mnLine,
                from.pos  = 0;
            }

            if (until.line > mxLine)
            {
                until.line = mxLine,
                until.pos  = getLine(mxLine).size();
            }

            for (fm::Size line = from.line;line <= until.line;++line)
            {
                fm::Size posBeg = (line == from.line ? from.pos : 0);
                fm::Size posEnd = (line == until.line ? until.pos : getLine(line).size());

                fm::vec2 p0 = pixPosFromTextPos(TextPos(line,posBeg)) + offset;
                fm::vec2 p1 = pixPosFromTextPos(TextPos(line,posEnd)) + offset + fm::vec2(0,m_metrics.lineGap);

                if (line < untilLine) p1.x += getCharWidth(' ');

                if (p0.x > size.w || p1.x < 0) continue;

                Cxy(2,2)
                {
                    fm::vec2 p = fm::vec2(x ? p1.x : p0.x, y ? p1.y : p0.y);

                    p.x = fm::min(fm::max(p.x,0.f),size.w);
                    p.y = fm::min(fm::max(p.y,0.f),size.h);

                    pts.push_back(p);
                    clr.push_back(fm::vec4(.75,.75,.75));
                }

                C(3) inds.push_back(quadID*4 + i);
                C(3) inds.push_back(quadID*4 + i + 1);

                quadID++;
            }
        }

        if (pts.size())
        {
            m_selectDraw[fg::Assoc::Position].set(&pts[0],pts.size());
            m_selectDraw[fg::Assoc::Color].set(&clr[0],clr.size());
            m_selectDraw.addDraw(fg::IndexArrayHolder(&inds[0],inds.size()),fg::Triangles);
        }
    }

    ////////////////////////////////////////////////////////////
    void EditText::recalcCarets()
    {
        m_caretsDraw.reset();

        std::vector<fm::vec2> pts;
        std::vector<fm::vec4> clr;

        fm::vec2 offset = -getViewPos();
        fm::vec2 size = getSize();

        fm::Size mnLine = fm::max(-offset.y/m_metrics.lineGap,0.f);
        fm::Size mxLine = fm::max((-offset.y+size.h)/m_metrics.lineGap,0.f);

        C(m_carets.size())
        {
            if (m_carets[i].tp.line > mxLine || m_carets[i].tp.line < mnLine) continue;

            fm::vec2 p0 = pixPosFromTextPos(m_carets[i].tp) + offset;
            fm::vec2 p1 = p0 + fm::vec2(0,m_metrics.lineGap);

            if (!m_insert)
            {
                const fm::String &line = getLine(m_carets[i].tp.line);
                float w = (m_carets[i].tp.pos == line.size() ? getCharWidth(' ') : getCharWidth(line[m_carets[i].tp.pos]));

                p0.y += m_metrics.lineGap - 2;
                p1.x += w;
            }

            if (p0.x > size.w || p0.x < 0 ||
                p1.x > size.w || p1.x < 0) continue;

            if (!m_insert)
            {
                fm::vec2 delta[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),
                                    fm::vec2(1,1),fm::vec2(0,1),fm::vec2(0,0)};
                Cx(6)
                    pts.push_back(delta[x]*p0 + (fm::vec2(1,1)-delta[x])*p1);

                clr.resize(clr.size()+6,fm::vec4::Black);
            }
            else
            {
                pts.push_back(p0);
                pts.push_back(p1);

                clr.resize(clr.size()+2,fm::vec4::Black);
            }

        }

        if (!pts.size())
            return;

        C(pts.size())
        {
            pts[i].x = fm::min(fm::max(pts[i].x,0.f),size.w);
            pts[i].y = fm::min(fm::max(pts[i].y,0.f),size.h);
        }

        m_caretsDraw[fg::Assoc::Position].set(&pts[0],pts.size());
        m_caretsDraw[fg::Assoc::Color].set(&clr[0],clr.size());
        m_caretsDraw.addDraw(0,pts.size(),m_insert ? fg::Lines : fg::Triangles);
    }

    /////////////////////////////////////////////////////////////
    fm::vec2 EditText::pixPosFromTextPos(const TextPos &tp) const
    {
        if (m_font)
            m_font->setCharacterSize(m_charSize);

        fm::vec2 ret;

        ret.y = tp.line * m_metrics.lineGap;

        C(fm::min(tp.pos,m_lines[tp.line].size()))
            ret.x += getCharWidth(m_lines[tp.line][i]);

        return ret;
    }

    /////////////////////////////////////////////////////////////
    fm::Size EditText::getCharWidth(fm::Uint32 cp,fg::Glyph *retGlyph) const
    {
        return priv::getCharWidth(m_font,cp,retGlyph,m_monoSpace);
    }

    EditText::TextPos EditText::textPosFromPixPos(fm::vec2 pixPos) const
    {
        fm::Size lineCount = getLineCount();
        fm::Size line = lineFromPixPos(pixPos);
        fm::Size pos;

        if (pixPos.y >= fm::max<float>((lineCount+5)*m_metrics.lineGap,getSize().h+getViewPos().y+m_metrics.lineGap*2))
        {
            pos = getLine(lineCount-1).size();
        }
        else if (pixPos.y >= lineCount*m_metrics.lineGap)
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
    fm::Size EditText::lineFromPixPos(fm::vec2 pixPos) const
    {
        if (pixPos.y < 0)
            pixPos.y = 0;

        fm::Size lineCount = getLineCount();

        if (pixPos.y >= lineCount*m_metrics.lineGap)
            pixPos.y  = lineCount*m_metrics.lineGap-1;

        return pixPos.y / m_metrics.lineGap;
    }

    /////////////////////////////////////////////////////////////
    fm::Size EditText::posFromPixPos(fm::vec2 pixPos) const
    {
        if (pixPos.y >= getLineCount()*m_metrics.lineGap)
        {
            return getLine(lineFromPixPos(pixPos)).size();
        }

        return posFromPixPos(pixPos.x,lineFromPixPos(pixPos));
    }

    fm::Size EditText::posFromPixPos(float pixPosX,fm::Size lineIndex) const
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

    ////////////////////////////////////////////////////////////
    void EditText::remDoubleCarets()
    {
        for (fm::Size x = 0;x < m_carets.size(); ++x)
            for (fm::Size y = x + 1;y < m_carets.size(); ++y)
            {
                Caret &a = m_carets[x];
                Caret &b = m_carets[y];

                if (a.tp.isBetween(b.tp,b.sel) ||
                    b.tp.isBetween(a.tp,a.sel))
                {
                    m_recalcCarets = true;
                    m_recalcSelect = true;

                    TextPos mn = fm::min(fm::min(a.tp,a.sel),fm::min(b.tp,b.sel));
                    TextPos mx = fm::max(fm::max(a.tp,a.sel),fm::max(b.tp,b.sel));

                    m_carets.erase(m_carets.begin()+y);

                    if (a.tp < a.sel)
                        a.tp  = mn,
                        a.sel = mx;
                    else
                        a.tp  = mx,
                        a.sel = mn;

                    y = x; // increases to x+1
                }
            }
    }

    ////////////////////////////////////////////////////////////
    void EditText::fixCaret(Caret &caret)
    {
        fm::Size oline = fm::min(getLineCount()-1,caret.tp.line);
        fm::Size opos  = fm::min(m_lines[caret.tp.line].size(),caret.tp.pos);

        fm::Size sline = fm::min(getLineCount()-1,caret.sel.line);
        fm::Size spos  = fm::min(m_lines[caret.sel.line].size(),caret.sel.pos);

        if (caret.tp != TextPos(oline,opos))
            setCaret(caret,TextPos(oline,opos),TextPos(sline,spos));
    }

    namespace priv
    {
        class CaretSorter
        {
        public:
            bool ascending;
            CaretSorter(bool a = true) : ascending(a) {}

            bool operator()(const EditText::Caret &a,const EditText::Caret &b) const
            {
                if (a.tp  != b.tp)  return (ascending ? a.tp < b.tp : a.tp > b.tp);
                if (a.sel != b.sel) return a.sel < b.sel;

                return a.savedWidth < b.savedWidth;
            }
        };
    }


    ////////////////////////////////////////////////////////////
    void EditText::sortCarets(bool ascending)
    {
        std::sort(m_carets.begin(),m_carets.end(),priv::CaretSorter(ascending));
    }

    ////////////////////////////////////////////////////////////
    EditText::EditText(const fm::vec2 &pos,
                       const fm::vec2 &size,
                       const fm::String &id,
                       Layout *parent,
                       const fm::String &text,
                       const fg::Font *font,
                       fm::Size characterSize) : Widget(pos,size,id,parent),
                                                 m_tex(fm::nullPtr),
                                                 m_font(fm::nullPtr),
                                                 m_charSize(characterSize),
                                                 m_recordUndo(true),
                                                 m_monoSpace(true),
                                                 m_editable(true),
                                                 m_freeView(false),
                                                 m_insert(true),
                                                 m_rightDrag(false),
                                                 m_dragMode(NoDrag),
                                                 m_lastClick(fm::String::npos,fm::String::npos),
                                                 m_recalcChars(true),
                                                 m_recalcCarets(true),
                                                 m_recalcSelect(true)
    {
        setFont(font);
        setText(text);
    }

    ////////////////////////////////////////////////////////////
    EditText::EditText(const fm::String &text,
                       const fm::vec2 &size,
                       fg::Font *font,
                       fm::Size characterSize) : Widget(fm::vec2(),size,"unnamed",fm::nullPtr),
                                                 m_tex(fm::nullPtr),
                                                 m_font(fm::nullPtr),
                                                 m_charSize(characterSize),
                                                 m_monoSpace(true),
                                                 m_editable(true),
                                                 m_freeView(false),
                                                 m_insert(true),
                                                 m_rightDrag(false),
                                                 m_dragMode(NoDrag),
                                                 m_lastClick(fm::String::npos,fm::String::npos),
                                                 m_recalcChars(true),
                                                 m_recalcCarets(true),
                                                 m_recalcSelect(true)

    {
        setFont(font);
        setText(text);
    }

    ////////////////////////////////////////////////////////////
    bool EditText::handleEvent(const fw::Event &ev)
    {
        if (isActive())
        {
            if (ev.type == fw::Event::TextEntered)
            {
                if (!getEditable()) return false;

                fm::Uint32 notPrinted[] = {'\b','\n','\r',127};

                bool printed = (ev.text.utf8character > 27);

                C(sizeof(notPrinted)/sizeof(*notPrinted))
                {
                    if (ev.text.utf8character == notPrinted[i])
                        printed = false;
                }

                if (printed)
                {
                    addUndo(UndoStep::InsertText);

                    C(m_carets.size())
                        insertText(m_carets[i],ev.text.utf8character);
                }
            }

            if (ev.type == fw::Event::MouseWheelMoved)
            {
                if (!ev.wheel.ctrl)
                {
                    int am = 3;

                    if (ev.wheel.shift)
                        am = getSize().h / m_metrics.lineGap;

                    if (ev.wheel.alt)
                        am = 1;

                    moveViewPos(fm::vec2(0,m_metrics.lineGap * -ev.wheel.delta * am));
                }
                else
                {
                    if (ev.wheel.delta < 0 && getCharSize() > 8) setCharSize(getCharSize()-1);

                    if (ev.wheel.delta > 0) setCharSize(getCharSize()+1);
                }
            }

            if (ev.type == fw::Event::KeyPressed)
            {
                handleKey(ev.key);
            }
        }

        return Widget::handleEvent(ev);
    }

    ////////////////////////////////////////////////////////////
    void EditText::handleEnter(fm::vec2 p)
    {
        fw::Mouse::setCursor(fw::Mouse::IBeam);

        Widget::handleEnter(p);
    }

    ////////////////////////////////////////////////////////////
    void EditText::handleLeave(fm::vec2 p)
    {
        fw::Mouse::setCursor(fw::Mouse::Arrow);

        Widget::handleEnter(p);
    }

    ////////////////////////////////////////////////////////////
    void EditText::handleHover(fm::vec2 p)
    {
        fm::vec2 inp = p-getPos()+getViewPos();
        TextPos tp = textPosFromPixPos(inp);

        if (m_rightDrag)
        {
            moveViewPos(-(p-m_lastRight));
            m_lastRight = p;
        }

        if (m_dragMode == KeepSelDrag)
        {
            setCaret(m_carets.back(),tp,m_carets.back().sel);

            remDoubleCarets();

            bringCaretsToView();
        }

        if (m_dragMode == KeepWordDrag)
        {
            Caret &caret = m_carets.back();

            TextPos sel;

            if (tp < m_dragTP0)
            {
                sel = m_dragTP1;
            }
            else
            {
                if (tp > m_dragTP1)
                {
                    sel = m_dragTP0;
                }
                else
                {
                    float w0 = pixPosFromTextPos(m_dragTP0).x;
                    float w1 = inp.x;
                    float w2 = pixPosFromTextPos(m_dragTP1).x;

                    if (w1-w0 < w2-w1)
                    {
                        tp  = m_dragTP0;
                        sel = m_dragTP1;
                    }
                    else
                    {
                        tp  = m_dragTP1;
                        sel = m_dragTP0;
                    }
                }
            }

            setCaret(caret,tp,sel);

            remDoubleCarets();

            bringCaretsToView();
        }

        Widget::handleHover(p);
    }

    ////////////////////////////////////////////////////////////
    void EditText::handlePress(fm::vec2 p,fw::Mouse::Button btn)
    {
        if (btn == fw::Mouse::Left)
        {
            fm::vec2 inp = p-getPos()+getViewPos();
            TextPos tp = textPosFromPixPos(inp);

            if (fw::Keyboard::isKeyHeld(fw::Keyboard::LShift) ||
                fw::Keyboard::isKeyHeld(fw::Keyboard::RShift))
            {
                if (m_dragMode == NoDrag)
                {
                    setCaret(m_carets.back(),tp,m_carets.back().sel);

                    remDoubleCarets();
                }
            }
            else
            {
                if (fw::Keyboard::isKeyHeld(fw::Keyboard::LCtrl) ||
                    fw::Keyboard::isKeyHeld(fw::Keyboard::RCtrl))
                {
                    addCaret(tp);

                    remDoubleCarets();
                }
                else
                {
                    m_carets.resize(1);
                    setCaret(m_carets[0],tp,tp);
                }
            }

            addUndo();

            // double click
            if (m_clickClk.getTime() < fm::milliseconds(400) && m_lastClick == tp)
            {
                bool left = (inp.x <= pixPosFromTextPos(tp).x);

                Caret cpy;
                setCaret(cpy,tp,tp);

                int l = getWordLength(cpy.tp,left,false);
                moveCaretPos(cpy,l,true,false);

                int r = getWordLength(cpy.tp,!left,false);
                moveCaretPos(cpy,r,false,false);

                if (!left)
                    setCaret(m_carets.back(),cpy.sel,cpy.tp);
                else
                    setCaret(m_carets.back(),cpy.tp,cpy.sel);

                remDoubleCarets();

                m_dragTP0 = cpy.sel;
                m_dragTP1 = cpy.tp;

                m_dragMode = KeepWordDrag;
            }
            else
            {
                m_dragMode = KeepSelDrag;

                m_clickClk.restart();
            }

            m_lastClick = tp;
        }

        if (btn == fw::Mouse::Right)
        {
            m_rightDrag = true;

            m_lastRight = p;
        }
    }

    ////////////////////////////////////////////////////////////
    void EditText::handleRelease(fm::vec2 p,fw::Mouse::Button btn)
    {
        if (btn == fw::Mouse::Left)
        {
            m_dragMode = NoDrag;
        }

        if (btn == fw::Mouse::Right)
        {
            m_rightDrag = false;
        }

        Widget::handleRelease(p,btn);
    }

    ////////////////////////////////////////////////////////////
    bool EditText::handleKey(fw::KeyboardEvent key)
    {
        int movPos  = (key.code == fw::Keyboard::Left)*-1 + (key.code == fw::Keyboard::Right)*1;
        int movLine = (key.code == fw::Keyboard::Up  )*-1 + (key.code == fw::Keyboard::Down )*1;

        if (movLine)
        {
            if (!getEditable())
            {
                if (key.shift || key.alt)
                    return false;
            }

            sortCarets(movLine < 0);

            if (!key.ctrl && key.shift && key.alt)
            {
                C(m_carets.size())
                {
                    if ((movLine < 0 && m_carets[i].tp.line) ||
                        (movLine > 0 && m_carets[i].tp.line+1 < getLineCount()))
                        addCaret(m_carets[i].tp + TextPos(movLine,0));
                }

                remDoubleCarets();

                finishUndo();
            }

            if (!key.ctrl && !key.alt)
            {
                C(m_carets.size())
                    moveCaretLine(m_carets[i],movLine,!key.shift);

                remDoubleCarets();

                finishUndo();
            }

            if (key.ctrl && !key.shift && !key.alt)
            {
                moveViewPos(fm::vec2(0,m_metrics.lineGap*movLine));
            }

            if (key.ctrl && key.shift && !key.alt)
            {
                C(m_carets.size())
                {
                    bool r = (m_carets[i].sel <= m_carets[i].tp);

                    TextPos from  = fm::min(m_carets[i].sel,m_carets[i].tp);
                    TextPos until = fm::max(m_carets[i].sel,m_carets[i].tp);

                    from.pos  = 0;
                    until.pos = getLine(until.line).size();

                    setCaret(m_carets[i],(r ? until : from),(r ? from : until));
                }

                remDoubleCarets();

                bool up = (movLine < 0);
                fm::Size lineCount = getLineCount();

                C(m_carets.size())
                {
                    TextPos from  = fm::min(m_carets[i].sel,m_carets[i].tp);
                    TextPos until = fm::max(m_carets[i].sel,m_carets[i].tp);

                    if (up && i)
                    {
                        TextPos prevMx = fm::max(m_carets[i-1].sel,m_carets[i-1].tp);

                        if (prevMx.line+1 == from.line) continue;
                    }

                    if (!up && i)
                    {
                        TextPos prevMn = fm::min(m_carets[i-1].sel,m_carets[i-1].tp);

                        if (prevMn.line == until.line+1) continue;
                    }

                    if ((up && from.line > 0) || (!up && until.line+1 < lineCount))
                    {
                        fm::Size next = up ? (from.line-1) : (until.line+1);
                        fm::Size prev = up ? (until.line+1) : (from.line);

                        addUndoStep(UndoStep(UndoStep::MoveLine,"",TextPos(next,prev)));

                        moveLine(next,prev);

                        setCaret(m_carets[i],(up ? m_carets[i].tp  - TextPos(1,0) : m_carets[i].tp  + TextPos(1,0)),
                                             (up ? m_carets[i].sel - TextPos(1,0) : m_carets[i].sel + TextPos(1,0)));
                    }
                }

                addUndo();
            }
        }

        if (movPos)
        {
            bool left = movPos < 0;
            sortCarets(left);

            C(m_carets.size())
            {
                if (key.ctrl) movPos = getWordLength(m_carets[i].tp,left,true);

                moveCaretPos(m_carets[i],movPos,!key.shift);
            }

            finishUndo();
        }

        int del = (key.code == fw::Keyboard::Backspace ? -1 : 0) + (key.code == fw::Keyboard::Delete ? 1 : 0);

        if (del)
        {
            if (!getEditable()) return false;

            sortCarets(del < 0);

            addUndo(UndoStep::DeleteText);

            bool left = (del < 0);

            C(m_carets.size())
            {
                if (key.ctrl)
                    del = getWordLength(m_carets[i].tp,left);

                delText(m_carets[i],del);
            }
        }

        if (key.code == fw::Keyboard::Enter)
        {
            if (!getEditable()) return false;

            if (key.shift)
            {
                C(m_carets.size())
                {
                    TextPos tp(m_carets[i].tp.line,getLine(m_carets[i].tp.line).size());
                    setCaret(m_carets[i],tp,tp);
                }

                remDoubleCarets();
            }

            C(m_carets.size())
                newLine(m_carets[i]);

            addUndo();
        }

        if (key.code == fw::Keyboard::Escape)
        {
            TextPos minTP = m_carets[0].tp;
            C(m_carets.size())
            {
                if (m_carets[i].tp < minTP)
                    minTP = m_carets[i].tp;
            }

            m_carets.resize(1);

            setCaret(m_carets[0],minTP,minTP);
        }

        if (key.code == fw::Keyboard::Tab)
        {
            if (!getEditable()) return false;

            addUndo(UndoStep::InsertText);

            C(m_carets.size())
                insertText(m_carets[i],'\t');
        }

        if (key.code == fw::Keyboard::End)
        {
            if (key.ctrl)
            {
                fm::Size lineCount = getLineCount();
                TextPos lastTP(lineCount-1,getLine(lineCount-1).size());
                TextPos minSel = lastTP;

                if (key.shift)
                {
                    C(m_carets.size())
                    {
                        if (minSel > m_carets[i].sel)
                            minSel = m_carets[i].sel;
                    }
                }

                m_carets.resize(1);

                setCaret(m_carets[0],lastTP,key.shift ? minSel : lastTP);
            }
            else
            {
                C(m_carets.size())
                {
                    TextPos tp(m_carets[i].tp.line,getLine(m_carets[i].tp.line).size());
                    TextPos sel = key.shift ? m_carets[i].sel : tp;

                    setCaret(m_carets[i],tp,sel);
                }

                remDoubleCarets();
            }

            bringCaretsToView();

            finishUndo();
        }

        if (key.code == fw::Keyboard::Insert)
        {
            m_insert = !m_insert;

            m_recalcCarets = true;
        }

        if (key.code == fw::Keyboard::Home)
        {
            if (key.ctrl)
            {
                TextPos maxSel(0,0);

                if (key.shift)
                {
                    C(m_carets.size())
                    {
                        if (maxSel < m_carets[i].sel)
                            maxSel = m_carets[i].sel;
                    }
                }

                m_carets.resize(1);

                setCaret(m_carets[0],TextPos(0,0),key.shift ? maxSel : TextPos(0,0));
            }
            else
            {
                C(m_carets.size())
                {
                    fm::Size ws = getLeadingWhiteSpaces(m_carets[i].tp.line).size();

                    TextPos tp(m_carets[i].tp.line,m_carets[i].tp.pos == ws ? 0 : ws);
                    TextPos sel = key.shift ? m_carets[i].sel : tp;

                    setCaret(m_carets[i],tp,sel);
                }

                remDoubleCarets();
            }

            bringCaretsToView();

            finishUndo();
        }

        if (key.ctrl)
        {
            if (key.code == fw::Keyboard::C ||
                key.code == fw::Keyboard::X)
            {
                if (fm::util::clipboard::openClipboard())
                {
                    fm::String clipData;

                    fm::String eol = "\n";

                    #ifdef FRONTIER_OS_WINDOWS
                    eol = "\r\n";
                    #endif // FRONTIER_OS_WINDOWS

                    C(m_carets.size())
                    {
                        Caret &caret = m_carets[i];

                        TextPos tpMin = fm::min(caret.tp,caret.sel);
                        TextPos tpMax = fm::max(caret.tp,caret.sel);

                        for (fm::Size line = tpMin.line;line <= tpMax.line;++line)
                        {
                            fm::Size from  = (line == tpMin.line ? tpMin.pos : 0);
                            fm::Size until = (line == tpMax.line ? tpMax.pos : getLine(line).size());

                            clipData += getLine(line).substr(from,until-from);

                            if (line != tpMax.line)
                                clipData += eol;
                        }

                        if (i+1 != m_carets.size())
                            clipData += eol;
                    }

                    fm::util::clipboard::emptyClipboard();
                    fm::util::clipboard::setData(clipData);

                    fm::util::clipboard::closeClipboard();

                    if (key.code == fw::Keyboard::X && getEditable())
                    {
                        C(m_carets.size())
                            delText(m_carets[i],0);
                    }
                }
            }
            if (key.code == fw::Keyboard::V)
            {
                if (!getEditable()) return false;

                if (fm::util::clipboard::openClipboard())
                {
                    if (fm::util::clipboard::hasTextData())
                    {
                        finishUndo();

                        fm::String clipData = fm::util::clipboard::getData();
                        fm::util::clipboard::closeClipboard();

                        fm::Size beg = 0;

                        C(clipData.size())
                        {
                            if (clipData[i] == '\n' || clipData[i] == '\r')
                            {
                                Cx(m_carets.size())
                                {
                                    insertText(m_carets[x],clipData.substr(beg,i-beg));
                                    newLine(m_carets[x]);
                                }

                                if (clipData[i] == '\r' && i+1 < clipData.size() && clipData[i+1] == '\n')
                                    ++i;

                                beg = i+1;
                            }
                        }

                        C(m_carets.size())
                            insertText(m_carets[i],clipData.substr(beg,clipData.size()-beg));

                    }
                    else
                    {
                       fm::util::clipboard::closeClipboard();
                    }
                }
            }

            if (key.code == fw::Keyboard::Z)
            {
                if (!getEditable()) return false;

                if (key.shift)
                    redo();
                else
                    undo();
            }

            if (key.code == fw::Keyboard::Y)
            {
                if (!getEditable()) return false;

                redo();
            }

            if (key.code == fw::Keyboard::A)
            {
                m_carets.resize(1);
                fm::Size lineCount = getLineCount();
                TextPos lastTP(lineCount-1,getLine(lineCount-1).size());

                setCaret(m_carets[0],TextPos(0,0),lastTP);
            }

            if (key.code == fw::Keyboard::T)
            {
                if (!getEditable()) return false;

                sortCarets(true);

                C(m_carets.size())
                {
                    setCaret(m_carets[i],m_carets[i].tp,m_carets[i].tp);

                    fm::Size line = m_carets[i].tp.line;

                    if (line > 0)
                    {
                        addUndoStep(UndoStep(UndoStep::MoveLine,"",TextPos(line,line-1)));

                        moveLine(line,line-1);
                    }
                }

                addUndo();
            }

            if (key.code == fw::Keyboard::D)
            {
                if (!getEditable()) return false;

                finishUndo();

                sortCarets(false);

                C(m_carets.size())
                {
                    Caret &caret = m_carets[i];

                    if (caret.tp == caret.sel)
                    {
                        Caret cp = caret;
                        TextPos lend = TextPos(cp.tp.line,getLine(cp.tp.line).size());
                        setCaret(cp,lend,lend);
                        newLine(cp);
                        insertText(cp,getLine(caret.tp.line));
                    }
                    else
                    {
                        TextPos tpMin = fm::min(caret.tp,caret.sel);
                        TextPos tpMax = fm::max(caret.tp,caret.sel);

                        std::deque<fm::String> toAdd;

                        while (tpMin.line < tpMax.line)
                        {
                            toAdd.push_back(m_lines[tpMin.line].substr(tpMin.pos));

                            tpMin.pos = 0;
                            tpMin.line ++;
                        }

                        toAdd.push_back(m_lines[tpMin.line].substr(tpMin.pos,tpMax.pos-tpMin.pos));

                        Caret cp;
                        setCaret(cp,tpMax,tpMax);

                        C(toAdd.size())
                        {
                            insertText(cp,toAdd[i]);

                            if (i+1 < toAdd.size())
                                newLine(cp);
                        }
                    }
                }

                addUndo();
            }
        }

        return false;
    }

    ////////////////////////////////////////////////////////////
    void EditText::onDraw(fg::ShaderManager &shader) const
    {
        if (!m_tex)
            return;

        GuiElement::onDraw(shader);

        fw::Window::setBlend(fw::Alpha);

        shader.getModelStack().push().mul(fm::MATRIX::translation(fm::vec2i(getPos())));
        shader.getTexUVStack().push(m_tex->getPixToUnitMatrix());

        shader.useTexture(fm::nullPtr);
        shader.draw(m_selectDraw);

        shader.getColorStack().push().mul(fm::MATRIX::scaling(0,0,0));
        shader.useTexture(m_tex);
        shader.draw(m_charsDraw);

        shader.useTexture(fm::nullPtr);
        if (isActive())
            shader.draw(m_caretsDraw);

        shader.getColorStack().pop();
        shader.getTexUVStack().pop();
        shader.getModelStack().pop();
    }

    ////////////////////////////////////////////////////////////
    void EditText::onUpdate(float dt)
    {
        (void)dt;

        if (m_recalcChars)
        {
            onTextChanged();
            m_recalcChars = false;
            recalcChars();
        }

        if (m_recalcCarets)
        {
            m_recalcCarets = false;
            recalcCarets();
        }

        if (m_recalcSelect)
        {
            m_recalcSelect = false;
            recalcSelect();
        }
    }

    ////////////////////////////////////////////////////////////
    void EditText::setSize(const fm::vec2 &size)
    {
        GuiElement::setSize(size);

        m_recalcChars  = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setText(const fm::String &text)
    {
        m_lines = std::deque<fm::String>(1);

        fm::Size beg = 0;

        C(text.size())
        {
            if (text[i] == '\n' || text[i] == '\r')
            {
                text.substr(beg,i-beg).swap(m_lines.back());
                m_lines.push_back(fm::String());

                if (text[i] == '\r' && i+1<text.size() && text[i+1] == '\n')
                    ++i;

                beg = i+1;
            }
        }

        text.substr(beg,text.size()-beg).swap(m_lines.back());

        m_carets.resize(1);
        m_carets[0] = Caret(TextPos(0,0));

        setViewPos(fm::vec2());

        m_recalcChars  = true;
        m_recalcCarets = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    fm::String EditText::getText(const fm::String &eol) const
    {
        fm::String ret;

        C(m_lines.size())
        {
            ret += m_lines[i];

            if (i+1 != m_lines.size())
                ret += eol;
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setFont(const fg::Font *font)
    {
        if (m_font == font) return;

        m_font = font;

        if (m_font)
            m_font->setCharacterSize(m_charSize),
            m_metrics = m_font->getMetrics();
        else
            m_metrics = fg::Metrics();

        m_recalcChars  = true;
        m_recalcCarets = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    const fg::Font *EditText::getFont() const
    {
        return m_font;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setFreeView(bool freeView)
    {
        if (m_freeView != freeView)
        {
            m_freeView = freeView;
            setViewPos(getViewPos());
        }
    }

    ////////////////////////////////////////////////////////////
    bool EditText::getFreeView() const
    {
        return m_freeView;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setInsert(bool doesInsert)
    {
        if (m_insert != doesInsert)
        {
            m_insert = doesInsert;
            m_recalcCarets = true;
        }
    }

    ////////////////////////////////////////////////////////////
    bool EditText::getInsert() const
    {
        return m_insert;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setMonoSpace(bool isMonoSpaced)
    {
        if (m_monoSpace != isMonoSpaced)
        {
            m_monoSpace    = isMonoSpaced;
            m_recalcChars  = true;
            m_recalcCarets = true;
            m_recalcSelect = true;
        }
    }

    ////////////////////////////////////////////////////////////
    bool EditText::getMonoSpace() const
    {
        return m_monoSpace;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setEditable(bool isEditable)
    {
        m_editable = isEditable;
    }

    ////////////////////////////////////////////////////////////
    bool EditText::getEditable() const
    {
        return m_editable;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setCharSize(fm::Size charSize)
    {
        if (m_charSize == charSize) return;

        if (m_font)
        {
            m_font->setCharacterSize(charSize);
            m_metrics = m_font->getMetrics();
        }

        m_charSize     = charSize;
        m_recalcChars  = true;
        m_recalcCarets = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    fm::Size EditText::getCharSize() const
    {
        return m_charSize;
    }

    ////////////////////////////////////////////////////////////
    fm::vec2 EditText::getViewPos() const
    {
        return m_viewPos;
    }

    ////////////////////////////////////////////////////////////
    void EditText::setViewPos(fm::vec2 p)
    {
        if (!m_font) return;

        if (p.x < 0) p.x = 0;
        if (p.y < 0) p.y = 0;

        float mx = (m_lines.size()-1)*m_metrics.lineGap;

        if (p.y > mx) p.y = mx;

        m_pixViewPos = p;
        m_viewPos = p;

        if (!m_freeView)
        {
            m_viewPos.y = (int(m_viewPos.y/m_metrics.lineGap))*m_metrics.lineGap;

            if (m_monoSpace)
                m_viewPos.x = (int(m_viewPos.x/getCharWidth(' ')))*getCharWidth(' ');
        }

        m_recalcChars  = true;
        m_recalcCarets = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    void EditText::moveViewPos(fm::vec2 delta)
    {
        setViewPos(m_pixViewPos + delta);
    }

    ////////////////////////////////////////////////////////////
    fm::Size EditText::getLineCount() const
    {
        return m_lines.size();
    }

    ////////////////////////////////////////////////////////////
    fm::String &EditText::getLine(fm::Size index)
    {
        return m_lines[index];
    }

    ////////////////////////////////////////////////////////////
    fm::String EditText::getLeadingWhiteSpaces(fm::Size lineIndex) const
    {
        const fm::String &line = getLine(lineIndex);

        int spaceType = getCharType(' ');

        C(line.size())
        {
            if (getCharType(line[i]) != spaceType)
                return line.substr(0,i);
        }

        return line.size();
    }

    ////////////////////////////////////////////////////////////
    int EditText::getCharType(fm::Uint32 cp) const
    {
        if (fm::String(".\\(){}[]<>+-*=/&|@%^~\'\"?!$#:;,").find(cp) != fm::String::npos)
            return 1;

        if (fm::String(" \t\r\n").find(cp) != fm::String::npos)
            return 2;

        return 3;
    }

    ////////////////////////////////////////////////////////////
    void EditText::bringCaretsToView()
    {
        TextPos tp = m_carets[0].tp;

        C(m_carets.size())
            tp = fm::max(tp,m_carets[i].tp);

        fm::vec2 p = pixPosFromTextPos(tp);
        fm::vec2 v = getViewPos();
        fm::vec2 size = getSize();
        float spaceW  = getCharWidth(' ');

        float dUp   = m_metrics.lineGap;
        float dDown = m_metrics.lineGap*2;

        if (v.h <= 2*m_metrics.lineGap)
            dUp   = 0,
            dDown = m_metrics.lineGap;

        if (p.y - dUp   < v.y)          v.y = p.y - dUp;
        if (p.y + dDown > v.y + size.h) v.y = p.y + dDown - size.h;

        if (p.x - spaceW     < v.x)          v.x = p.x - spaceW;
        if (p.x + spaceW*1.5 > v.x + size.w) v.x = p.x + spaceW*1.5 - size.w;

        setViewPos(v);
    }

    ////////////////////////////////////////////////////////////
    int EditText::getWordLength(const TextPos &tp,bool left,bool enableSpaceJump) const
    {
        const fm::String *lineStr = &getLine(tp.line);

        int type;
        int spaceType = getCharType(' ');
        int prevType;
        fm::Size ret  = 0;
        fm::Size pos  = tp.pos;
        fm::Size line = tp.line;

        if (!(tp.pos == 0 && left) && !(tp.pos == lineStr->size() && !left))
        {
            type = getCharType(lineStr->at(pos + (left ? -1 : 0)));
        }
        else
            type = -1;

        prevType = type;

        while (true)
        {
            if (left)
            {
                if (pos == 0)
                {
                    if (line == 0) break;
                    else
                    {
                        if (type == -1 || type == spaceType)
                        {
                            if (!enableSpaceJump && type == spaceType) break;

                            if (prevType != spaceType && prevType != -1) break;

                            type = -1;
                            line--;
                            lineStr = &getLine(line);
                            pos = lineStr->size();
                            ret--;

                            if (!enableSpaceJump) break;
                        }
                        else break;
                    }
                }
                else
                {
                    int curType = getCharType(lineStr->at(pos-1));

                    if (type == -1) break;

                    if ((type == spaceType && curType != prevType && prevType != spaceType) ||
                        (type != spaceType && curType == spaceType) ||
                        (type != spaceType && curType != spaceType && curType != prevType) ||
                        (prevType != curType && !enableSpaceJump))
                    {
                        break;
                    }
                    else
                    {
                        pos--;
                        ret--;

                        prevType = curType;
                    }
                }
            }
            else
            {
                if (pos == lineStr->size())
                {
                    if (line+1 == getLineCount()) break;
                    else
                    {
                        if (type == -1)
                        {
                            line++;
                            lineStr = &getLine(line);
                            pos = 0;
                            ret++;

                            if (!enableSpaceJump) break;
                        }
                        else break;
                    }
                }
                else
                {
                    int curType = getCharType(lineStr->at(pos));

                    if (type == -1)
                        type = spaceType,
                        prevType = spaceType;


                    if ((type == spaceType && curType != prevType && prevType == spaceType) ||
                        (type != spaceType && curType != spaceType && curType != prevType) ||
                        (prevType != curType && !enableSpaceJump))
                    {
                        break;
                    }
                    else
                    {
                        pos++;
                        ret++;

                        prevType = curType;
                    }
                }
            }
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////
    void EditText::applyUndo(const Undo &undo,bool redo)
    {
        C(undo.steps.size())
        {
            const UndoStep &step = undo.steps[redo ? i : undo.steps.size()-1-i];

            UndoStep::Action action = step.action;

            if (redo)
                action = (EditText::UndoStep::Action)(UndoStep::ActionInvert - action);

            if (action == UndoStep::InsertLine)
            {
                m_lines[step.tp.line].push_back(m_lines[step.tp.line+1]);
                m_lines.erase(m_lines.begin() + step.tp.line + 1);
            }

            if (action == UndoStep::InsertText)
            {
                m_lines[step.tp.line].erase(step.tp.pos,step.text.size());
            }

            if (action == UndoStep::DeleteLine)
            {
                m_lines.insert(m_lines.begin() + step.tp.line + 1,m_lines[step.tp.line].substr(step.tp.pos,fm::String::npos));
                m_lines[step.tp.line].erase(step.tp.pos,fm::String::npos);
            }

            if (action == UndoStep::DeleteText)
            {
                m_lines[step.tp.line].insert(step.tp.pos,step.text);
            }

            if (action == UndoStep::MoveLine)
            {
                fm::Size next = step.tp.line;
                fm::Size prev = step.tp.pos;

                if (redo)
                    moveLine(next,prev);
                else
                {
                    if (next > prev)
                        moveLine(prev,next+1);
                    else
                        moveLine(prev-1,next);
                }
            }
        }

        if (redo)
            m_carets = undo.caretsAfter;
        else
            m_carets = undo.caretsBefore;

        bringCaretsToView();

        m_recalcCarets = true;
        m_recalcChars  = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    void EditText::moveLine(fm::Size line,fm::Size newIndex)
    {
        m_lines.insert(m_lines.begin()+newIndex,fm::String());

        if (newIndex <= line) line++;

        getLine(line).swap(getLine(newIndex));

        m_lines.erase(m_lines.begin()+line);

        m_recalcChars = true;
    }

    ////////////////////////////////////////////////////////////
    void EditText::onTextChanged()
    {

    }

    ////////////////////////////////////////////////////////////
    void EditText::undo()
    {
        while (m_undos.size() && m_undos.back().steps.size() == 0)
            m_undos.pop_back();

        if (m_undos.size())
        {
            applyUndo(m_undos.back());

            m_redos.push_back(m_undos.back());

            m_undos.pop_back();
        }
    }

    ////////////////////////////////////////////////////////////
    void EditText::redo()
    {
        while (m_redos.size() && m_redos.back().steps.size() == 0)
            m_redos.pop_back();

        if (m_redos.size())
        {
            applyUndo(m_redos.back(),true);

            m_undos.push_back(m_redos.back());

            m_redos.pop_back();
        }
    }

    ////////////////////////////////////////////////////////////
    void EditText::addUndoStep(UndoStep step)
    {
        if (!m_recordUndo) return;

        if (m_undos.size() == 0)
            addUndo();

        bool added = false;

        if (m_undos.back().steps.size())
        {
            UndoStep &prev = m_undos.back().steps.back();

            if (prev.action == UndoStep::InsertText && step.action == prev.action)
            {
                if (prev.tp.line == step.tp.line &&
                    prev.tp.pos  <= step.tp.pos &&
                    prev.tp.pos + prev.text.size() >= step.tp.pos)
                {
                    prev.text.insert(step.tp.pos - prev.tp.pos,step.text);

                    added = true;
                }
            }

            if (prev.action == UndoStep::DeleteText && step.action == prev.action)
            {
                if (prev.tp.line == step.tp.line)
                {
                    if (prev.tp.pos == step.tp.pos)
                    {
                        prev.text.push_back(step.text);
                        added = true;
                    }
                    if (prev.tp.pos == step.tp.pos + step.text.size())
                    {
                        step.text.push_back(prev.text);
                        step.text.swap(prev.text);
                        prev.tp = step.tp;

                        added = true;
                    }
                }
            }
        }

        if (!added)
        {
            m_undos.back().steps.push_back(step);
        }

        m_undos.back().caretsAfter = m_carets;
    }

    ////////////////////////////////////////////////////////////
    void EditText::addUndo(UndoStep::Action action)
    {
        if (!m_recordUndo) return;

        if (m_undos.size() && m_undos.back().steps.size())
        {
            m_undos.back().caretsAfter = m_carets;
        }

        if (!m_undos.size() || (m_undos.back().steps.size() && m_undos.back().steps.back().action != action))
        {
            m_undos.push_back(Undo());
            m_undos.back().caretsBefore = m_carets;

            m_redos.resize(0);
        }
        else if (m_undos.size() && !m_undos.back().steps.size())
        {
            m_undos.back().caretsBefore = m_carets;
        }
    }

    ////////////////////////////////////////////////////////////
    void EditText::finishUndo()
    {
        if (!m_recordUndo) return;

        m_undos.push_back(Undo());
        m_undos.back().caretsBefore = m_carets;
    }

    ////////////////////////////////////////////////////////////
    void EditText::addUndo()
    {
        if (!m_recordUndo) return;

        if (m_undos.size() && m_undos.back().steps.size())
        {
            m_undos.back().caretsAfter = m_carets;
        }

        if (!m_undos.size() || m_undos.back().steps.size())
        {
            m_undos.push_back(Undo());
            m_undos.back().caretsBefore = m_carets;

            m_redos.resize(0);
        }
        else if (m_undos.size() && !m_undos.back().steps.size())
        {
            m_undos.back().caretsBefore = m_carets;
        }
    }

    ////////////////////////////////////////////////////////////
    const fm::String &EditText::getLine(fm::Size index) const
    {
        return m_lines[index];
    }

    ////////////////////////////////////////////////////////////
    void EditText::insertText(Caret &caret,const fm::String &text)
    {
        // fix caret
        fixCaret(caret);

        // delete selected text
        delText(caret,0);

        if (!m_insert)
        {
            const fm::String &line = getLine(caret.tp.line);
            fm::Size toDel = fm::min(line.size()-caret.tp.pos,text.size());

            delText(caret,toDel);
        }

        TextPos tp = caret.tp;

        // do action
        getLine(caret.tp.line).insert(caret.tp.pos,text);

        // advance carets
        C(m_carets.size())
        {
            if (m_carets[i].tp.line == caret.tp.line &&
                m_carets[i].tp.pos > caret.tp.pos)
            {
                moveCaretPos(m_carets[i],text.size(),false,true);
            }
        }

        setCaret(caret,caret.tp + TextPos(0,text.size()),caret.tp + TextPos(0,text.size()));

        // add undo option
        addUndoStep(UndoStep(UndoStep::InsertText,text,tp));

        // notify drawing
        m_recalcChars  = true;
    }

    /////////////////////////////////////////////////////////////
    void EditText::delText(Caret &caret,int amount)
    {
        // fix caret
        fixCaret(caret);

        if (caret.tp != caret.sel)
        {
            amount = 0;

            TextPos from  = fm::min(caret.sel,caret.tp);
            TextPos until = fm::max(caret.sel,caret.tp);

            for (fm::Size line = from.line;line <= until.line;++line)
            {
                fm::Size posBeg = (line == from.line ? from.pos : 0);
                fm::Size posEnd = (line == until.line ? until.pos : getLine(line).size()+1);

                amount += posEnd - posBeg;
            }

            if (caret.tp > caret.sel)
            {
                amount *= -1;
            }
        }

        while (amount < 0)
        {
            if (caret.tp.pos == 0)
            {
                if (caret.tp.line == 0)
                {
                    amount = 0;
                }
                else
                {
                    amount++;

                    fm::Size nlen = m_lines[caret.tp.line-1].size();

                    // add undo option
                    addUndoStep(UndoStep(UndoStep::DeleteLine,"",TextPos(caret.tp.line-1,nlen)));

                    // do action
                    m_lines[caret.tp.line-1].push_back(m_lines[caret.tp.line]);
                    m_lines.erase(m_lines.begin()+caret.tp.line);

                    // advance carets
                    C(m_carets.size())
                    {
                        if (m_carets[i].tp.line == caret.tp.line &&
                            m_carets[i].tp.pos > caret.tp.pos)
                        {
                            setCaret(m_carets[i],m_carets[i].tp - TextPos(1,0) + TextPos(nlen),m_carets[i].sel - TextPos(1,0) + TextPos(nlen));
                        }
                        else if (m_carets[i].tp.line > caret.tp.line)
                        {
                            setCaret(m_carets[i],m_carets[i].tp - TextPos(1,0),m_carets[i].sel - TextPos(1,0));
                        }
                    }

                    setCaret(caret,TextPos(caret.tp.line-1,nlen),TextPos(caret.tp.line-1,nlen));
                }
            }
            else
            {
                fm::Size begp = 0;
                if (fm::Size(-1*amount) < caret.tp.pos)
                {
                    begp = caret.tp.pos - (-1*amount);
                    amount = 0;
                }
                else
                    amount += caret.tp.pos;

                fm::Size charsRem = caret.tp.pos - begp;

                // add undo option
                addUndoStep(UndoStep(UndoStep::DeleteText,m_lines[caret.tp.line].substr(begp,charsRem),TextPos(caret.tp.line,begp)));

                // do action
                m_lines[caret.tp.line].erase(begp,charsRem);

                // advance carets
                C(m_carets.size())
                {
                    if (m_carets[i].tp.line == caret.tp.line &&
                        m_carets[i].tp.pos > caret.tp.pos)
                    {
                        moveCaretPos(m_carets[i],-int(charsRem),false,true);
                    }
                }

                setCaret(caret,TextPos(caret.tp.line,begp),TextPos(caret.tp.line,begp));

                remDoubleCarets();
            }
        }

        while (amount > 0)
        {
            if (caret.tp.pos == m_lines[caret.tp.line].size())
            {
                if (caret.tp.line+1 == m_lines.size())
                {
                    amount = 0;
                }
                else
                {
                    amount--;

                    fm::Size oldLen = m_lines[caret.tp.line].size();

                    // add undo option
                    addUndoStep(UndoStep(UndoStep::DeleteLine,"",TextPos(caret.tp.line,oldLen)));

                    // do action
                    m_lines[caret.tp.line].push_back(m_lines[caret.tp.line+1]);
                    m_lines.erase(m_lines.begin()+caret.tp.line+1);

                    // advance carets
                    C(m_carets.size())
                    {
                        if (m_carets[i].tp.line == caret.tp.line+1)
                        {
                            setCaret(m_carets[i],m_carets[i].tp - TextPos(1,0) + TextPos(0,oldLen),m_carets[i].sel - TextPos(1,0) + TextPos(0,oldLen));
                        }
                        else if (m_carets[i].tp.line > caret.tp.line+1)
                        {
                            setCaret(m_carets[i],m_carets[i].tp - TextPos(1,0),m_carets[i].sel - TextPos(1,0));
                        }
                    }

                    remDoubleCarets();
                }
            }
            else
            {
                fm::Size llen  = m_lines[caret.tp.line].size();
                fm::Size until = fm::String::npos;
                fm::Size deleted = 0;

                if (fm::Size(amount) < llen-caret.tp.pos)
                {
                    deleted = amount;
                    until   = caret.tp.pos + amount;
                    amount  = 0;
                }
                else
                {
                    deleted = llen-caret.tp.pos;
                    amount -= llen-caret.tp.pos;
                }

                // add undo option
                addUndoStep(UndoStep(UndoStep::DeleteText,m_lines[caret.tp.line].substr(caret.tp.pos,until-caret.tp.pos),caret.tp));

                // do action
                m_lines[caret.tp.line].erase(caret.tp.pos,until-caret.tp.pos);

                // advance carets
                C(m_carets.size())
                {
                    if (m_carets[i].tp.line == caret.tp.line &&
                        m_carets[i].tp.pos   > caret.tp.pos)
                    {
                        moveCaretPos(m_carets[i],-int(deleted),false,true);
                    }
                }

                remDoubleCarets();
            }
        }

        caret.sel = caret.tp;

        bringCaretsToView();

        // notify drawing
        m_recalcCarets = true;
        m_recalcChars  = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    void EditText::newLine(Caret &caret)
    {
        // fix caret
        fixCaret(caret);

        // delete selected text
        delText(caret,0);

        // add undo option
        addUndoStep(UndoStep(UndoStep::InsertLine,"",TextPos(caret.tp)));

        // do action
        m_lines.insert(m_lines.begin()+caret.tp.line+1,fm::String());
        getLine(caret.tp.line).substr(caret.tp.pos,fm::String::npos).swap(getLine(caret.tp.line+1));

        getLine(caret.tp.line).erase(caret.tp.pos,fm::String::npos);

        // advance carets
        C(m_carets.size())
        {
            if (m_carets[i].tp.line == caret.tp.line &&
                m_carets[i].tp.pos  >  caret.tp.pos)
            {
                setCaret(m_carets[i],m_carets[i].tp + TextPos(1,0) - TextPos(0,m_lines[caret.tp.line].size()),m_carets[i].sel + TextPos(1,0) - TextPos(0,m_lines[caret.tp.line].size()));
            }
            else if (m_carets[i].tp.line > caret.tp.line)
            {
                setCaret(m_carets[i],m_carets[i].tp + TextPos(1,0),m_carets[i].sel + TextPos(1,0));
            }
        }

        setCaret(caret,TextPos(caret.tp.line+1,0),TextPos(caret.tp.line+1,0));

        bringCaretsToView();

        // notify drawing
        m_recalcChars  = true;
        m_recalcCarets = true;
    }

    namespace priv
    {
        void advanceLine(EditText::TextPos &tp,int deltaLine,fm::Size lineCount,fm::Size lastLineSize,bool &isEnd)
        {
            if (deltaLine < 0 && tp.line < fm::Size(deltaLine*-1))
            {
                isEnd = true;
                tp = EditText::TextPos(0,0);
            }
            else if (deltaLine > 0 && lineCount - tp.line <= fm::Size(deltaLine))
            {
                isEnd = true;
                tp = EditText::TextPos(lineCount - 1,lastLineSize);
            }
            else
            {
                isEnd = false;
                tp.line += deltaLine;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void EditText::moveCaretLine(Caret &caret,int deltaLine,bool resetSel,bool advanceSel)
    {
        // fix caret
        fixCaret(caret);

        if (!deltaLine) return;

        fm::Size lineCount = getLineCount();

        bool isEnd = false;

        // advance caret
        priv::advanceLine(caret.tp,deltaLine,lineCount,getLine(lineCount-1).size(),isEnd);

        // reset pos to saved one
        if (!isEnd)
            caret.tp.pos = posFromPixPos(caret.savedWidth,caret.tp.line);
        else
            caret.savedWidth = pixPosFromTextPos(caret.tp).x;

        // advance/reset sel if requested
        if (advanceSel)
            priv::advanceLine(caret.tp,deltaLine,lineCount,getLine(lineCount-1).size(),isEnd);

        if (resetSel)
            caret.sel = caret.tp;

        bringCaretsToView();

        // notify drawing
        m_recalcCarets = true;
        m_recalcSelect = true;
    }

    namespace priv
    {
        void advancePos(EditText &et,EditText::TextPos &tp,int deltaPos)
        {
            fm::Size lineSize  = et.getLine(tp.line).size();
            fm::Size lineCount = et.getLineCount();

            while (deltaPos)
            {
                if (deltaPos < 0)
                {
                    if (tp.pos == 0)
                    {
                        if (tp.line > 0)
                        {
                            tp.line--;
                            tp.pos = et.getLine(tp.line).size();

                            deltaPos++;
                        }
                        else break;
                    }
                    else
                    {
                        fm::Size am = fm::min(tp.pos,fm::Size(-deltaPos));
                        deltaPos += am;
                        tp.pos -= am;
                    }
                }
                if (deltaPos > 0)
                {
                    if (tp.pos == lineSize)
                    {
                        if (tp.line+1 < lineCount)
                        {
                            tp.line++;
                            lineSize = et.getLine(tp.line).size();
                            tp.pos = 0;

                            deltaPos--;
                        }
                        else break;
                    }
                    else
                    {
                        fm::Size am = fm::min(lineSize-tp.pos,fm::Size(deltaPos));
                        deltaPos -= am;
                        tp.pos += am;
                    }
                }
            }
        }
    }


    ////////////////////////////////////////////////////////////
    void EditText::moveCaretPos(Caret &caret,int deltaPos,bool resetSel,bool advanceSel)
    {
        if (!deltaPos) return;

        if (caret.sel != caret.tp && resetSel)
        {
            TextPos tp;
            if (deltaPos < 0) tp = fm::min(caret.sel,caret.tp);
            if (deltaPos > 0) tp = fm::max(caret.sel,caret.tp);

            if (deltaPos)
                caret.sel = tp,
                caret.tp  = tp;
        }
        else
        {
            priv::advancePos(*this,caret.tp,deltaPos);

            caret.savedWidth = pixPosFromTextPos(caret.tp).x;

            if (advanceSel)
                priv::advancePos(*this,caret.sel,deltaPos);

            if (resetSel)
                caret.sel = caret.tp;

            remDoubleCarets();
        }

        bringCaretsToView();

        m_recalcCarets = true;
        m_recalcSelect = true;
    }

    ////////////////////////////////////////////////////////////
    void EditText::addCaret(const TextPos &tp)
    {
        m_carets.push_back(Caret());
        setCaret(m_carets.back(),tp,tp);

        bringCaretsToView();

        m_recalcCarets = true;
        m_recalcSelect = true;
    }
    ////////////////////////////////////////////////////////////
    void EditText::setCaret(Caret &caret,const TextPos &tp,const TextPos &sel)
    {
        caret.tp  = tp;
        caret.sel = sel;
        caret.savedWidth = pixPosFromTextPos(caret.tp).x;

        m_recalcCarets = true;
        m_recalcSelect = true;
    }
}
