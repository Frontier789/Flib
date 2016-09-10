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
#include <FRONTIER/Gui/LinearLayout.hpp>
#include <FRONTIER/System/Swap.hpp>

namespace fgui
{
    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout() : Layout(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr),
                                   m_ori(Vertical),
                                   m_needRecalc(false),
                                   m_align(Left)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(const RelPos &pos,
                               const fm::vec2 &size,
                               Orientation ori,
                               const fm::String &id,
                               Layout *parent,
                               int align) : Layout(pos,size,id,parent),
                                            m_ori(ori),
											m_needRecalc(false),
											m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(const fm::vec2 &size,
                               Orientation ori,
                               const fm::String &id,
                               Layout *parent,
                               int align) : Layout(fm::vec2(),size,id,parent),
                                            m_ori(ori),
											m_needRecalc(false),
											m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(Orientation ori,
                               const fm::String &id,
                               Layout *parent,
                               int align) : Layout(fm::vec2(),fm::vec2(),id,parent),
                                            m_ori(ori),
											m_needRecalc(false),
											m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(const fm::String &id,
                               Layout *parent,
                               int align) : Layout(fm::vec2(),fm::vec2(),id,parent),
                                            m_ori(Vertical),
											m_needRecalc(false),
											m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::LinearLayout(Orientation ori,
                               int align) : Layout(fm::vec2(),fm::vec2(),"unnamed",fm::nullPtr),
                                            m_ori(ori),
											m_needRecalc(false),
											m_align(align)
    {

    }

    ////////////////////////////////////////////////////////////
    LinearLayout::~LinearLayout()
    {
		clear();
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
            m_elemAligns.resize(index+1,m_align);
        }
        else
        {
            m_elements.push_back(fm::nullPtr);
            m_elemAligns.push_back(m_align);
        }

        fm::Size i = m_elements.size()-1;
        while (i > index)
        {
        	fm::swap(m_elements[i-1],m_elements[i]);
        	fm::swap(m_elemAligns[i-1],m_elemAligns[i]);

            --i;
        }

        m_elements[i] = newElement;

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
				m_elements[index]   = m_elements[index+1];
				m_elemAligns[index] = m_elemAligns[index+1];
				
				++index;
            }

            m_elements.pop_back();
            m_elemAligns.pop_back();

			m_needRecalc = true;

            return ret;
        }

        return fm::nullPtr;
    }
        
	/////////////////////////////////////////////////////////////
	void LinearLayout::setAlign(int align)
	{
		m_align = align;
		C(m_elemAligns.size()) m_elemAligns[i] = align;
		
		m_needRecalc = true;
	}

	////////////////////////////////////////////////////////////
	void LinearLayout::setAlign(fm::Size id,int align)
	{
		if (m_elemAligns.size() > id)
			m_elemAligns[id] = align;
		
		m_needRecalc = true;
	}

    ////////////////////////////////////////////////////////////
	int LinearLayout::getAlign(fm::Size id) const
	{
		return m_elemAligns[id];
	}

    ////////////////////////////////////////////////////////////
	int LinearLayout::getAlign() const
	{
		return m_align;
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
        
		if (elementCount())
			mnSize[addSide] += (elementCount()-1)*3;

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
                
				s[addSide] += 3;

                float offset = (ratio-1.f)*s[addSide]/2.f;

                fm::vec2 p;
                p[addSide] = offset + used;

				if (m_elemAligns[i] == 1)
					p[maxSide] += (m_realSize[maxSide] - s[maxSide])/2;
					
				if (m_elemAligns[i] == 2)
					p[maxSide] += m_realSize[maxSide] - s[maxSide];

                e->setPos(fm::vec2((fm::vec2i)p));

                used += ratio*s[addSide];                
            }
        }
    }
}
