#include <FRONTIER/Gui/ValueControl.hpp>

////////////////////////////////////////////////////////////
ValueControl::ValueControl(const fm::vec2 &pos,
                           const fm::vec2 &size,
                           const fm::String &id,
                           Layout *parent,
                           const fm::String &text,
                           const fg::Font *font,
                           fm::Size characterSize) : EditText(pos,size,id,parent,text,font,characterSize),
                                                     m_lastValid("")
{

}

////////////////////////////////////////////////////////////
ValueControl::ValueControl(const fm::String &text,
                           const fm::vec2 &size,
                           fg::Font *font,
                           fm::Size characterSize) : EditText(text,size,font,characterSize),
                                                     m_lastValid("")
{

}

////////////////////////////////////////////////////////////
bool ValueControl::handleEvent(const fw::Event &ev)
{
    if (ev.type == fw::Event::TextEntered)
    {
        fm::Uint32 c = ev.text.utf8character;
        if (!acceptChar(c))
            return false;
    }

    bool handled = EditText::handleEvent(ev);

    if (ev.type == fw::Event::TextEntered)
    {
        onStringChange();
    }

    return handled;
}

////////////////////////////////////////////////////////////
bool ValueControl::isTextValid(const fm::String &text) const
{
    (void)text;

    return true;
}

////////////////////////////////////////////////////////////
void ValueControl::correctText(fm::String &text)
{
    (void)text;
}

////////////////////////////////////////////////////////////
bool ValueControl::acceptChar(fm::Uint32 character) const
{
    (void)character;

    return true;
}

////////////////////////////////////////////////////////////
bool ValueControl::handleKey(fw::KeyboardEvent key)
{
    if (key.code == fw::Keyboard::Enter)
    {
        if (isTextValid(getDataString()))
        {
            fm::String line = getLine(0);
            correctText(line);
            setText(line);

            m_lastValid.swap(line);

            onDataChange();

            setActive(false);
        }
        else
        {
            setText(m_lastValid);
        }

        return true;
    }

    return EditText::handleKey(key);
}

////////////////////////////////////////////////////////////
void ValueControl::onStringChange()
{

}

////////////////////////////////////////////////////////////
void ValueControl::onDataChange()
{

}

////////////////////////////////////////////////////////////
const fm::String &ValueControl::getDataString() const
{
    return getLine(0);
}

