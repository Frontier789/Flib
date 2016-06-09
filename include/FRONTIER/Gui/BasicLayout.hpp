#ifndef FRONTIER_BASICLAYOUT_HPP_INCLUDED
#define FRONTIER_BASICLAYOUT_HPP_INCLUDED
#include <FRONTIER/Gui/Layout.hpp>
#define FRONTIER_BASICLAYOUT
#include <vector>

namespace fgui
{
    class BasicLayout : public Layout
    {
    protected:
        std::vector<GuiElement*> m_elements;

        /////////////////////////////////////////////////////////////
        virtual void onContentChanged();

    public:
        BasicLayout();

        /////////////////////////////////////////////////////////////
        explicit BasicLayout(const fm::vec2 &pos,
                             const fm::String &id = "unnamed",
                             Layout *parent = fm::nullPtr);

        /////////////////////////////////////////////////////////////
        explicit BasicLayout(const fm::String &id,
                              Layout *parent = fm::nullPtr);

        /////////////////////////////////////////////////////////////
        virtual ~BasicLayout();

        /////////////////////////////////////////////////////////////
        virtual fm::Size    elementCount() const;
        virtual GuiElement *elementAt(fm::Size index);
        virtual void        insertElement(fm::Size index,GuiElement *newElement);
        virtual GuiElement *remElement(fm::Size index);
    };
}

#endif // FRONTIER_BASICLAYOUT_HPP_INCLUDED
