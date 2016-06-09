#ifndef FRONTIER_WIDGET_HPP_INCLUDED
#define FRONTIER_WIDGET_HPP_INCLUDED
#include <FRONTIER/System/Delegate.hpp>#include <FRONTIER/Gui/GuiElement.hpp>
#define FRONTIER_WIDGET

class Widget : public GuiElement
{
    bool m_mouseIn;
    int  m_pressed;
public:
    ////////////////////////////////////////////////////////////
    fm::Delegate<void,Widget &,fm::vec2> onEnter;
    fm::Delegate<void,Widget &,fm::vec2> onHover;
    fm::Delegate<void,Widget &,fm::vec2> onLeave;

    ////////////////////////////////////////////////////////////
    fm::Delegate<void,Widget &,fm::vec2,fw::Mouse::Button> onPress;
    fm::Delegate<void,Widget &,fm::vec2,fw::Mouse::Button> onRelease;

    ////////////////////////////////////////////////////////////
    Widget(const fm::vec2 &pos,
           const fm::vec2 &size,
           const fm::String &id = "unnamed",
           Layout *parent = fm::nullPtr,
           bool enabled = true);

    ////////////////////////////////////////////////////////////
    explicit Widget(const fm::vec2 &size,
                    const fm::String &id = "unnamed",
                    Layout *parent = fm::nullPtr,
                    bool enabled = true);

    ////////////////////////////////////////////////////////////
    explicit Widget(const fm::String &id,
                    Layout *parent = fm::nullPtr,
                    bool enabled = true);

    ////////////////////////////////////////////////////////////
    virtual ~Widget();

    ////////////////////////////////////////////////////////////
    virtual bool handleEvent(const fw::Event &ev);

    ////////////////////////////////////////////////////////////
    virtual void handleEnter(fm::vec2 p);
    virtual void handleLeave(fm::vec2 p);
    virtual void handleHover(fm::vec2 p);
    virtual void handlePress(fm::vec2 p,fw::Mouse::Button btn);
    virtual void handleRelease(fm::vec2 p,fw::Mouse::Button btn);

    ////////////////////////////////////////////////////////////
    virtual bool contains(fm::vec2 p) const;
};

#endif // FRONTIER_GUIELEMENT_HPP_INCLUDED



