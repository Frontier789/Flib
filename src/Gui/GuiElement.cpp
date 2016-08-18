#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/Layout.hpp>

namespace fgui
{
	////////////////////////////////////////////////////////////
	RelPos::RelPos(fm::vec2 pix,fm::vec2 parentRel,fm::vec2 childRel) : pix(pix), parentRel(parentRel), childRel(childRel)
	{
		
	}
    
    ////////////////////////////////////////////////////////////
    GuiElement::GuiElement() : m_resMan(fm::nullPtr),
                               m_userSize(fm::vec2()),
                               m_realSize(fm::vec2()),
                               m_parent(fm::nullPtr),
                               m_id("unnamed"),
                               m_pos(fm::vec2()),
                               m_enabled(true),
                               m_active(false)
    {

    }

    ////////////////////////////////////////////////////////////
    GuiElement::GuiElement(const RelPos &pos,
                           const fm::vec2 &size,
                           const fm::String &id,
                           Layout *parent,
                           bool enabled) : m_resMan(fm::nullPtr),
                                           m_userSize(size),
                                           m_realSize(size),
                                           m_parent(parent),
                                           m_id(id),
                                           m_pos(pos),
                                           m_enabled(enabled),
                                           m_active(false)
    {

    }

    ////////////////////////////////////////////////////////////
    GuiElement::GuiElement(const fm::vec2 &size,
                           const fm::String &id,
                           Layout *parent,
                           bool enabled) : m_resMan(fm::nullPtr),
                                           m_userSize(size),
                                           m_realSize(size),
                                           m_parent(parent),
                                           m_id(id),
                                           m_pos(fm::vec2()),
                                           m_enabled(enabled),
                                           m_active(false)
    {

    }

    ////////////////////////////////////////////////////////////
    GuiElement::GuiElement(const fm::String &id,
                           Layout *parent,
                           bool enabled) : m_resMan(fm::nullPtr),
                                           m_userSize(fm::vec2()),
                                           m_realSize(fm::vec2()),
                                           m_parent(parent),
                                           m_id(id),
                                           m_pos(fm::vec2()),
                                           m_enabled(enabled),
                                           m_active(false)
    {

    }

    ////////////////////////////////////////////////////////////
    GuiElement::~GuiElement()
    {

    }

    ////////////////////////////////////////////////////////////
    bool GuiElement::handleEvent(const fw::Event &ev)
    {
        (void)ev;

        return false;
    }

    ////////////////////////////////////////////////////////////
    void GuiElement::onDraw(fg::ShaderManager &shader)
    {
        (void)shader;
    }

    ////////////////////////////////////////////////////////////
    void GuiElement::onUpdate(const fm::Time &dt)
    {
        (void)dt;
    }

    ////////////////////////////////////////////////////////////
    void GuiElement::setParent(Layout *parent)
    {
        m_parent = parent;
    }

    ////////////////////////////////////////////////////////////
    Layout *&GuiElement::getParent()
    {
        return m_parent;
    }

    ////////////////////////////////////////////////////////////
    Layout *const &GuiElement::getParent() const
    {
        return m_parent;
    }

    ////////////////////////////////////////////////////////////
    void GuiElement::setActive(bool active)
    {
        m_active = active;

        if (m_parent)
        {
            if (active)
                m_parent->setActive(this);
            else
                if (m_parent->getActive() == this)
                    m_parent->setActive(fm::nullPtr);
        }
    }

    ////////////////////////////////////////////////////////////
    bool GuiElement::isActive() const
    {
        return (m_parent ? m_parent->getActive()==this : m_active);
    }

    ////////////////////////////////////////////////////////////
    void GuiElement::setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    ////////////////////////////////////////////////////////////
    bool GuiElement::isEnabled() const
    {
        return m_enabled;
    }

	////////////////////////////////////////////////////////////
	void GuiElement::setResMan(ResourceManager *resMan)
	{
		m_resMan = resMan;
	}
	
	////////////////////////////////////////////////////////////
	ResourceManager *GuiElement::getResMan() const
	{
		return m_resMan;
	}

    ////////////////////////////////////////////////////////////
    void GuiElement::setPos(const RelPos &pos)
    {
        m_pos = pos;
    }

    ////////////////////////////////////////////////////////////
    fm::vec2 GuiElement::getPos() const
    {
        if (m_parent)
			return m_parent->getPos() + m_pos.pix + m_parent->getSize() * m_pos.parentRel - getSize() * m_pos.childRel;

		return m_pos.pix + getSize() * m_pos.childRel;
    }

    ////////////////////////////////////////////////////////////
    fm::vec2 GuiElement::getPosInParent() const
    {
        if (m_parent)
			return m_pos.pix + m_parent->getSize() * m_pos.parentRel - getSize() * m_pos.childRel;

		return m_pos.pix + getSize() * m_pos.childRel;
    }

    ////////////////////////////////////////////////////////////
    void GuiElement::setSize(const fm::vec2 &size)
    {
        m_userSize = size;
        m_realSize = size;

        if (m_parent) m_parent->childChanged(this);
    }

    ////////////////////////////////////////////////////////////
    fm::vec2 GuiElement::getSize() const
    {
        return m_realSize;
    }

    ////////////////////////////////////////////////////////////
    void GuiElement::setId(const fm::String &id)
    {
        m_id = id;
    }

    ////////////////////////////////////////////////////////////
    const fm::String &GuiElement::getId() const
    {
        return m_id;
    }
}
