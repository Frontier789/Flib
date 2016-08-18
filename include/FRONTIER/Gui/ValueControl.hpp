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
#ifndef FRONTIER_VALUECONTROL_HPP_INCLUDED
#define FRONTIER_VALUECONTROL_HPP_INCLUDED
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/Gui/EditText.hpp>
#define FRONTIER_VALUECONTROL

namespace fgui
{
    class ValueControl : public EditText
    {
		fm::Delegate<void,ValueControl *> m_callback;
        fm::String m_lastValid;
		int m_lineNum;
    public:
    	
        ////////////////////////////////////////////////////////////
        ValueControl(const RelPos &pos,
                     const fm::vec2 &size,
                     const fm::String &id = "unnamed",
                     Layout *parent = fm::nullPtr,
                     const fm::String &text = "",
                     fm::Ref<const fg::Font> font = fm::nullPtr,
                     fm::Size characterSize = 12,
                     const fm::Delegate<void,ValueControl *> &callback = fm::nullPtr);

        ////////////////////////////////////////////////////////////
        ValueControl(const fm::vec2 &size,
                     const fm::String &text = "",
                     fm::Ref<const fg::Font> font = fm::nullPtr,
                     fm::Size characterSize = 12,
                     const fm::Delegate<void,ValueControl *> &callback = fm::nullPtr);

        ////////////////////////////////////////////////////////////
		ValueControl(float width,
					 int lines = 1,
                     const fm::Delegate<void,ValueControl *> &callback = fm::nullPtr,
                     fm::Ref<const fg::Font> font = fm::nullPtr,
                     fm::Size characterSize = 12);

        ////////////////////////////////////////////////////////////
		ValueControl(float width,
					 const fm::Delegate<void,ValueControl *> &callback,
                     fm::Ref<const fg::Font> font = fm::nullPtr,
                     fm::Size characterSize = 12);

        /////////////////////////////////////////////////////////////
		void setCallback(const fm::Delegate<void,ValueControl *> &callback);
		
        /////////////////////////////////////////////////////////////
		virtual void setFont(fm::Ref<const fg::Font> font);
        virtual void setSize(const fm::vec2 &size);
        
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

