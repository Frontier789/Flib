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

class Layout;

class GuiElement : public fg::Drawable
{
protected:
    ////////////////////////////////////////////////////////////
    fm::vec2 m_pos;
    fm::vec2 m_userSize;
    fm::vec2 m_realSize;
    Layout *m_parent;
    fm::String m_id;
    bool m_enabled;
    bool m_active;

public:
    ////////////////////////////////////////////////////////////
    GuiElement();

    ////////////////////////////////////////////////////////////
    GuiElement(const fm::vec2 &pos,
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
    virtual void onDraw(fg::ShaderManager &shader) const;
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
    virtual void setPos(const fm::vec2 &pos);
    fm::vec2 getPosInParent() const;
    fm::vec2 getPos() const;

    ////////////////////////////////////////////////////////////
    virtual void setSize(const fm::vec2 &size);
    virtual fm::vec2 getSize() const;

    ////////////////////////////////////////////////////////////
    void setId(const fm::String &id);
    const fm::String &getId() const;
};

#endif // FRONTIER_GUIELEMENT_HPP_INCLUDED


