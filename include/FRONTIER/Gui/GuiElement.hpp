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
#ifndef FRONTIER_GUIELEMENT_HPP_INCLUDED
#define FRONTIER_GUIELEMENT_HPP_INCLUDED
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/System/Time.hpp>
#include <FRONTIER/Config.hpp>
#define FRONTIER_GUIELEMENT

namespace fgui
{
    class Layout;
	class ResourceManager;
	
	class RelPos
	{
	public:
		fm::vec2 pix;
		fm::vec2 parentRel;
		fm::vec2 childRel;
		
		RelPos(fm::vec2 pix = fm::vec2(),fm::vec2 parentRel = fm::vec2(),fm::vec2 childRel = fm::vec2());
	};
	
    class GuiElement : public fg::Drawable
    {
    protected:
        ////////////////////////////////////////////////////////////
		ResourceManager *m_resMan;
        fm::vec2 m_userSize;
        fm::vec2 m_realSize;
        Layout *m_parent;
        fm::String m_id;
        RelPos m_pos;
        bool m_enabled;
        bool m_active;

    public:
        ////////////////////////////////////////////////////////////
        GuiElement();

        ////////////////////////////////////////////////////////////
        GuiElement(const RelPos &pos,
                   const fm::vec2 &size,
                   const fm::String &id = "unnamed",
                   Layout *parent = fm::nullPtr,
                   bool enabled = true);

        ////////////////////////////////////////////////////////////
        explicit GuiElement(const fm::vec2 &size,
                            const fm::String &id = "unnamed",
                            Layout *parent = fm::nullPtr,
                            bool enabled = true);

        ////////////////////////////////////////////////////////////
        explicit GuiElement(const fm::String &id,
                            Layout *parent = fm::nullPtr,
                            bool enabled = true);

        ////////////////////////////////////////////////////////////
        virtual ~GuiElement();

        ////////////////////////////////////////////////////////////
        virtual bool handleEvent(const fw::Event &ev);

        ////////////////////////////////////////////////////////////
        virtual void onDraw(fg::ShaderManager &shader);
        virtual void onUpdate(const fm::Time &dt = fm::Time::Zero);

        ////////////////////////////////////////////////////////////
        virtual void setParent(Layout *parent);
        Layout *const &getParent() const;
        Layout *&getParent();

        ////////////////////////////////////////////////////////////
        virtual void setActive(bool active = true);
        bool isActive() const;

        ////////////////////////////////////////////////////////////
        virtual void setEnabled(bool enabled = true);
        bool isEnabled() const;

        ////////////////////////////////////////////////////////////
		virtual void setResMan(ResourceManager *resMan);
        ResourceManager *getResMan() const;

        ////////////////////////////////////////////////////////////
        virtual void setPos(const RelPos &pos);
        fm::vec2 getPosInParent() const;
        fm::vec2 getPos() const;

        ////////////////////////////////////////////////////////////
        virtual void setSize(const fm::vec2 &size);
        virtual fm::vec2 getSize() const;

        ////////////////////////////////////////////////////////////
        void setId(const fm::String &id);
        const fm::String &getId() const;
    };
}

#endif // FRONTIER_GUIELEMENT_HPP_INCLUDED


