#ifndef FRONTIER_BUTTON_HPP_INCLUDED
#define FRONTIER_BUTTON_HPP_INCLUDED
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#define FRONTIER_BUTTON

namespace fgui
{
    class Button : public GuiElement
    {
    public:
        enum State {
            MouseOut,
            MouseOver,
            Pressed
        };

    protected:
        State m_state;
        fm::Delegate<void,Button*> m_callback;

        /////////////////////////////////////////////////////////////
        virtual void onHover(bool on);
        virtual void onPress(bool on);

    public:
        Button(const RelPos &pos,
               const fm::vec2 &size,
               const fm::String &id = "unnamed",
               Layout *parent = fm::nullPtr,
               const fm::Delegate<void,Button*> &callback = fm::nullPtr,
               State state = MouseOut);

        /////////////////////////////////////////////////////////////
        explicit Button(const fm::vec2 &size,
                        const fm::String &id = "unnamed",
                        Layout *parent = fm::nullPtr,
                        const fm::Delegate<void,Button*> &callback = fm::nullPtr,
                        State state = MouseOut);

        /////////////////////////////////////////////////////////////
        explicit Button(const fm::vec2 &size,
                        const fm::String &id = "unnamed",
                        const fm::Delegate<void,Button*> &callback = fm::nullPtr,
                        State state = MouseOut);

        /////////////////////////////////////////////////////////////
        explicit Button(const fm::vec2 &size,
                        const fm::Delegate<void,Button*> &callback = fm::nullPtr,
                        State state = MouseOut);

        /////////////////////////////////////////////////////////////
        explicit Button(const fm::Delegate<void,Button*> &callback,
                        State state = MouseOut);

        /////////////////////////////////////////////////////////////
        virtual bool contains(const fm::vec2 &p) const;

        /////////////////////////////////////////////////////////////
        virtual bool handleEvent(const fw::Event &ev);

        /////////////////////////////////////////////////////////////
        virtual void setCallback(const fm::Delegate<void,Button*> &callback);
        const fm::Delegate<void,Button*> &getCallback() const;
    };
}

#endif // FRONTIER_BUTTON_HPP_INCLUDED

