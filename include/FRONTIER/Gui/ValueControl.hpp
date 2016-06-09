#ifndef FRONTIER_VALUECONTROL_HPP_INCLUDED
#define FRONTIER_VALUECONTROL_HPP_INCLUDED
#include <FRONTIER/Gui/EditText.hpp>
#define FRONTIER_VALUECONTROL

namespace fgui
{
    class ValueControl : public EditText
    {
        fm::String m_lastValid;
    public:
        ////////////////////////////////////////////////////////////
        ValueControl(const fm::vec2 &pos,
                     const fm::vec2 &size,
                     const fm::String &id = "unnamed",
                     Layout *parent = fm::nullPtr,
                     const fm::String &text = "",
                     const fg::Font *font = fm::nullPtr,
                     fm::Size characterSize = 12);

        ////////////////////////////////////////////////////////////
        ValueControl(const fm::String &text,
                     const fm::vec2 &size,
                     fg::Font *font = fm::nullPtr,
                     fm::Size characterSize = 12);

        /////////////////////////////////////////////////////////////
        virtual bool handleEvent(const fw::Event &ev);
        virtual bool handleKey(fw::KeyboardEvent key);

        /////////////////////////////////////////////////////////////
        virtual bool isTextValid(const fm::String &text) const;
        virtual void correctText(fm::String &text);
        virtual bool acceptChar(fm::Uint32 character) const;

        /////////////////////////////////////////////////////////////
        virtual void onStringChange();
        virtual void onDataChange();

        /////////////////////////////////////////////////////////////
        const fm::String &getDataString() const;
    };
}

#endif // FRONTIER_VALUECONTROL_HPP_INCLUDED

