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
																					m_lineNum(size.area() ? 0 : 1)
    {

    }

    ////////////////////////////////////////////////////////////
    ValueControl::ValueControl(const fm::vec2 &size,
                               const fm::String &text,
                               fm::Ref<const fg::Font> font,
                               fm::Size characterSize,
                               const fm::Delegate<void,ValueControl *> &callback) : EditText(size,text,font,characterSize),
																					m_callback(callback),
																					m_lineNum(size.area() ? 0 : 1)
    {

    }

	////////////////////////////////////////////////////////////
	ValueControl::ValueControl(float width,
							   int lines,
							   const fm::Delegate<void,ValueControl *> &callback,
							   fm::Ref<const fg::Font> font,
							   fm::Size characterSize) : EditText(fm::vec2(width,0),"",font,characterSize),
														 m_callback(callback),
														 m_lineNum(lines)
	{
		
	}

	////////////////////////////////////////////////////////////
	ValueControl::ValueControl(float width,
							   const fm::Delegate<void,ValueControl *> &callback,
							   fm::Ref<const fg::Font> font,
							   fm::Size characterSize) : EditText(fm::vec2(width,0),"",font,characterSize),
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
		
		if (m_lineNum && font)
		{
			fg::Metrics met = font->getMetrics();
			m_realSize.h = met.lineGap * m_lineNum + getFrameSize().h*2;
			m_userSize = m_realSize;
		}
	}
	
    ////////////////////////////////////////////////////////////
    void ValueControl::setSize(const fm::vec2 &size)
    {
		float h = size.h;
		
		if (m_lineNum && getFont())
		{
			float lineH = getFont()->getMetrics().lineGap * m_lineNum + getFrameSize().h*2;
			
			if (lineH > h) h = lineH;
		}
    	
		EditText::setSize(fm::vec2(size.w,h));
    }
    
    ////////////////////////////////////////////////////////////
    void ValueControl::setCharSize(fm::Size charSize)
    {
    	EditText::setCharSize(charSize);
    	
		if (m_lineNum && getFont())
		{
			fg::Metrics met = getFont()->getMetrics();
			m_realSize.h = met.lineGap * m_lineNum + getFrameSize().h*2;
			m_userSize = m_realSize;
		}
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
		if (key.code == fw::Keyboard::Enter && !key.ctrl)
        {
            fm::String str = getDataString();
            
            if (isTextValid(str))
            {
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
    fm::String ValueControl::getDataString() const
    {
		fm::String ret;
		
		C(getLineCount())
		{
			if (i) ret += '\n';
			
			ret += getLine(i);
		}
		
        return ret;
    }
}
