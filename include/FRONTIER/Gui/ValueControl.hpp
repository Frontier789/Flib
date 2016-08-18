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
        ValueControl(const fm::String &text,
                     const fm::vec2 &size,
                     fm::Ref<const fg::Font> font = fm::nullPtr,
                     fm::Size characterSize = 12,
                     const fm::Delegate<void,ValueControl *> &callback = fm::nullPtr);

        ////////////////////////////////////////////////////////////
        ValueControl(const fm::vec2 &size,
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

