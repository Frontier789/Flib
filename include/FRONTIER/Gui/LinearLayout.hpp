#ifndef FRONTIER_LINEARLAYOUT_HPP_INCLUDED
#define FRONTIER_LINEARLAYOUT_HPP_INCLUDED
#include <FRONTIER/Gui/Layout.hpp>
#define FRONTIER_LINEARLAYOUT
#include <vector>

namespace fgui
{
    class LinearLayout : public Layout
    {
    public:
        enum Orientation {
            Vertical,
            Horizontal
        };

    protected:
        Orientation m_ori;
        std::vector<GuiElement*> m_elements;
        bool m_needRecalc;

        /////////////////////////////////////////////////////////////
        void recalc();

    public:
        LinearLayout();

        /////////////////////////////////////////////////////////////
        LinearLayout(const fm::vec2 &pos,
                     const fm::vec2 &size,
                     Orientation ori = Vertical,
                     const fm::String &id = "unnamed",
                     Layout *parent = fm::nullPtr);

        /////////////////////////////////////////////////////////////
        explicit LinearLayout(const fm::vec2 &size,
                              Orientation ori = Vertical,
                              const fm::String &id = "unnamed",
                              Layout *parent = fm::nullPtr);

        /////////////////////////////////////////////////////////////
        explicit LinearLayout(Orientation ori,
                              const fm::String &id = "unnamed",
                              Layout *parent = fm::nullPtr);

        /////////////////////////////////////////////////////////////
        explicit LinearLayout(const fm::String &id,
                              Layout *parent = fm::nullPtr);

        /////////////////////////////////////////////////////////////
        virtual ~LinearLayout();

        /////////////////////////////////////////////////////////////
        virtual fm::Size    elementCount() const;
        virtual GuiElement *elementAt(fm::Size index);
        virtual void        insertElement(fm::Size index,GuiElement *newElement);
        virtual GuiElement *remElement(fm::Size index);

        /////////////////////////////////////////////////////////////
        virtual void setSize(const fm::vec2 &size);
        virtual fm::vec2 getSize() const;

        /////////////////////////////////////////////////////////////
        virtual void onUpdate(const fm::Time &dt = fm::Time::Zero);

        /////////////////////////////////////////////////////////////
        virtual void childChanged(GuiElement *child);

        /////////////////////////////////////////////////////////////
        virtual void setOrientation(Orientation ori);
        Orientation getOrientation() const;
    };
}

#endif // FRONTIER_LINEARLAYOUT_HPP_INCLUDED

