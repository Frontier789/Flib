#include <FRONTIER/Gui/ValueControl.hpp>

namespace fgui
{
    ////////////////////////////////////////////////////////////
    ValueControl::ValueControl(const RelPos &pos,
                               const fm::vec2 &size,
                               const fm::String &id,
                               Layout *parent,
                               const fm::String &text,
                               fm::Ref<const fg::Font> font,
                               fm::Size characterSize,
                               const fm::Delegate<void,ValueControl *> &callback) : EditText(pos,size,id,parent,text,font,characterSize),
																					m_callback(callback),
																					m_lineNum(0)
    {

    }

    ////////////////////////////////////////////////////////////
    ValueControl::ValueControl(const fm::String &text,
                               const fm::vec2 &size,
                               fm::Ref<const fg::Font> font,
                               fm::Size characterSize,
                               const fm::Delegate<void,ValueControl *> &callback) : EditText(text,size,font,characterSize),
																					m_callback(callback),
																					m_lineNum(0)
    {

    }

    ////////////////////////////////////////////////////////////
    ValueControl::ValueControl(const fm::vec2 &size,
                               fm::Ref<const fg::Font> font,
                               fm::Size characterSize,
                               const fm::Delegate<void,ValueControl *> &callback) : EditText(size,font,characterSize),
																					m_callback(callback),
																					m_lineNum(0)
    {

    }

	////////////////////////////////////////////////////////////
	ValueControl::ValueControl(float width,
							   int lines,
							   const fm::Delegate<void,ValueControl *> &callback,
							   fm::Ref<const fg::Font> font,
							   fm::Size characterSize) : EditText(fm::vec2(width,0),font,characterSize),
														 m_callback(callback),
														 m_lineNum(lines)
	{
		
	}

	////////////////////////////////////////////////////////////
	ValueControl::ValueControl(float width,
							   const fm::Delegate<void,ValueControl *> &callback,
							   fm::Ref<const fg::Font> font,
							   fm::Size characterSize) : EditText(fm::vec2(width,0),font,characterSize),
														 m_callback(callback),
														 m_lineNum(1)
	{
		
	}
    
    ////////////////////////////////////////////////////////////
	void ValueControl::setCallback(const fm::Delegate<void,ValueControl *> &callback)
	{
		m_callback = callback;
	}
	
    ////////////////////////////////////////////////////////////
	void ValueControl::setFont(fm::Ref<const fg::Font> font)
	{
		EditText::setFont(font);
		
		if (m_lineNum)
		{
			fg::Metrics met = font->getMetrics();
			m_realSize.h = met.lineGap * m_lineNum;
			m_userSize = m_realSize;
		}
	}
	
    ////////////////////////////////////////////////////////////
    void ValueControl::setSize(const fm::vec2 &size)
    {
		m_lineNum = 0;
		
		EditText::setSize(size);
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
                fm::String str = getDataString();
                correctText(str);
                setText(str);

                m_lastValid.swap(str);
                
                m_callback(this);

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
}
