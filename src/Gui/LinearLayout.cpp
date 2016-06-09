#include <FRONTIER/Gui/LinearLayout.hpp>

namespace fgui
{
    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout() : Layout(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr),
                                   m_ori(Vertical)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(const fm::vec2 &pos,
                               const fm::vec2 &size,
                               Orientation ori,
                               const fm::String &id,
                               Layout *parent) : Layout(pos,size,id,parent),
                                                     m_ori(ori)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(const fm::vec2 &size,
                               Orientation ori,
                               const fm::String &id,
                               Layout *parent) : Layout(fm::vec2(),size,id,parent),
                                                     m_ori(ori)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(Orientation ori,
                               const fm::String &id,
                               Layout *parent) : Layout(fm::vec2(),fm::vec2(),id,parent),
                                                     m_ori(ori)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(const fm::String &id,
                               Layout *parent) : Layout(fm::vec2(),fm::vec2(),id,parent),
                                                     m_ori(Vertical)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::~LinearLayout()
    {

    }

    ////////////////////////////////////////////////////////////
    fm::Size LinearLayout::elementCount() const
    {
        return m_elements.size();
    }

    ////////////////////////////////////////////////////////////
    GuiElement *LinearLayout::elementAt(fm::Size index)
    {
        if (index >= elementCount()) return fm::nullPtr;

        return m_elements[index];
    }

    ////////////////////////////////////////////////////////////
    void LinearLayout::insertElement(fm::Size index,GuiElement *newElement)
    {
        if (m_elements.size() <= index)
        {
            m_elements.resize(index+1,fm::nullPtr);
        }
        else
        {
            m_elements.push_back(fm::nullPtr);
        }

        fm::Size i = m_elements.size()-1;
        while (i > index)
        {
            GuiElement *tmp = m_elements[i];
            m_elements[i]   = m_elements[i-1];
            m_elements[i-1] = tmp;

            --i;
        }

        m_elements[i] = newElement;
        newElement->setParent(this);

        if (newElement && newElement->getParent() != this)
            newElement->setParent(this);

        m_needRecalc = true;
    }

    ////////////////////////////////////////////////////////////
    GuiElement *LinearLayout::remElement(fm::Size index)
    {
        if (index < m_elements.size())
        {
            GuiElement *ret = m_elements[index];

            while (index+1 < m_elements.size())
            {
                m_elements[index] = m_elements[index+1];
            }

            m_elements.pop_back();

            return ret;
        }

        m_needRecalc = true;

        return fm::nullPtr;
    }


    ////////////////////////////////////////////////////////////
    void LinearLayout::setSize(const fm::vec2 &size)
    {
        m_userSize = size;
        m_needRecalc = true;

        if (m_parent) m_parent->childChanged(this);
    }

    ////////////////////////////////////////////////////////////
    fm::vec2 LinearLayout::getSize() const
    {
        if (m_needRecalc)
        {
            ((LinearLayout*)this)->m_needRecalc = false;
            ((LinearLayout*)this)->recalc();
        }

        return Layout::getSize();
    }

    ////////////////////////////////////////////////////////////
    void LinearLayout::onUpdate(const fm::Time &dt)
    {
        Layout::onUpdate(dt);

        if (m_needRecalc)
        {
            m_needRecalc = false;
            recalc();
        }
    }

    ////////////////////////////////////////////////////////////
    void LinearLayout::childChanged(GuiElement *child)
    {
        (void)child;
        m_needRecalc = true;
    }

    ////////////////////////////////////////////////////////////
    void LinearLayout::setOrientation(Orientation ori)
    {
        m_ori = ori;
    }

    ////////////////////////////////////////////////////////////
    LinearLayout::Orientation LinearLayout::getOrientation() const
    {
        return m_ori;
    }

    ////////////////////////////////////////////////////////////
    void LinearLayout::recalc()
    {
        int addSide = (m_ori==Vertical ? 1 : 0);
        int maxSide = 1-addSide;

        fm::vec2 mnSize;
        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
            {
                fm::vec2 s = e->getSize();

                mnSize[addSide] += s[addSide];
                mnSize[maxSide]  = fm::max(mnSize[maxSide],s[maxSide]);
            }
        }

        m_realSize.w = fm::max(mnSize.w,m_userSize.w);
        m_realSize.h = fm::max(mnSize.h,m_userSize.h);

        float used  = 0;
        float ratio = m_realSize[addSide]/mnSize[addSide];

        C(elementCount())
        {
            GuiElement *e = elementAt(i);
            if (e)
            {
                fm::vec2 s = e->getSize();

                float offset = (ratio-1.f)*s[addSide]/2.f;

                fm::vec2 p;
                p[addSide] = offset + used;

                e->setPos(p);

                used += ratio*s[addSide];
            }
        }
    }
}
