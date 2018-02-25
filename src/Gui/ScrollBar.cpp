////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/AttributeRef.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/Gui/ScrollBar.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	void ScrollBar::setupDraws()
	{
		fm::vec2 p = getPosition();
		fm::vec2 s = getSize();
		float scrollSize = getScrollSize();
		fm::vec2 sprS = s * scrollSize;
		float scrollState = getScrollState();
		
		fm::vec2 pts[12];
		fm::vec2 tpt[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,0),fm::vec2(1,1),fm::vec2(0,1)};
		
		auto mixF = [&](float a,float b,float s,bool fst) -> float {
			
			if (a > b)
				return fst ? a : b;
			
			return a * (1-s) + b * s;
		};
		
		if (getScrollDirection() == ScrollHorizontal)
		{
			m_handle.setPosition(p + fm::vec2i((s.w-sprS.w)*scrollState,0));
			m_handle.setSize(fm::vec2i(sprS.w,s.h));
			
			C(6)
				pts[i+0] = fm::vec2(mixF(p.x + sprS.w/2,m_handle.getPosition().x,tpt[i].x,false),
									p.y + s.h/2 - 2 + 4 * tpt[i].y);
			
			C(6)
				pts[i+6] = fm::vec2(mixF(m_handle.getPosition().x + m_handle.getSize().w, p.x + s.w - sprS.w/2, tpt[i].x,true),
									p.y + s.h/2 - 2 + 4 * tpt[i].y);
		}
		else
		{
			m_handle.setPosition(p + fm::vec2i(0,(s.h-sprS.h)*(1 - scrollState)));
			m_handle.setSize(fm::vec2(s.w,sprS.h));
			
			C(6)
				pts[i+0] = fm::vec2(p.x + s.w/2 - 2 + 4 * tpt[i].x,
									mixF(p.y + sprS.h/2,m_handle.getPosition().y,tpt[i].y,false));
			
			C(6)
				pts[i+6] = fm::vec2(p.x + s.w/2 - 2 + 4 * tpt[i].x,
									mixF(m_handle.getPosition().y + m_handle.getSize().h, p.y + s.h - sprS.h/2, tpt[i].y,true));
		}
		
		m_railDraw.positions = pts;
	}
	
	/////////////////////////////////////////////////////////////
	bool ScrollBar::inDragArea(const fm::vec2 &p) const
	{
		return m_handle.contains(p);
	}

	/////////////////////////////////////////////////////////////
	ScrollBar::ScrollBar(GuiContext &cont) : GuiScrollBar(cont),
											 m_direction(ScrollDefault),
											 m_handle(cont),
											 m_grabbed(false)
	{
		fm::vec4 clr[12];
		C(12)
			clr[i] = fm::vec4(.5,.5,.5,.5);
		
		m_railDraw.colors = clr;
		m_railDraw.addDraw(0,12,fg::Triangles);
		
		setupDraws();
	}

	/////////////////////////////////////////////////////////////
	ScrollBar::ScrollBar(GuiContext &cont,fm::vec2s size,fm::Delegate<void,GuiScrollBar &> callback) : ScrollBar(cont)
	{
		setSize(size);
		setCallback(callback);
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::setScrollSize(float scrollSize)
	{
		GuiScrollBar::setScrollSize(scrollSize);
		
		setupDraws();
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::setScrollState(float state)
	{
		GuiScrollBar::setScrollState(state);
		
		setupDraws();
	}
	
	/////////////////////////////////////////////////////////////
	bool ScrollBar::onEvent(fw::Event &ev)
	{
		bool res = GuiScrollBar::onEvent(ev);
		
		m_handle.handleEvent(ev);
		
		return res;
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onScroll(float amount)
	{
		GuiScrollBar::onScroll(amount);
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::setSize(fm::vec2s size)
	{
		GuiScrollBar::setSize(size);
		
		setupDraws();
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::setPosition(fm::vec2i pos)
	{
		GuiScrollBar::setPosition(pos);
		
		setupDraws();
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onMouseMoved(fm::vec2 p,fm::vec2 /* prevP */ )
	{
		if (m_grabbed)
		{
			fm::vec2 s = getSize();
			
			float state = 0;
			
			if (getScrollDirection() == ScrollHorizontal)
			{
				state = m_grabState - (m_grabp.x - p.x) / (s.w - s.w * getScrollSize());
			}
			else
			{
				state = m_grabState - (p.y - m_grabp.y) / (s.h - s.h * getScrollSize());
			}
			
			setScrollState(state);
			setupDraws();
		}
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onPress(fw::Mouse::Button button,fm::vec2 p)
	{
		if (button == fw::Mouse::Left)
		{
			if (!inDragArea(p))
			{
				fm::vec2 s = getSize();
				fm::vec2 sprS = s * getScrollSize();
				
				if (getScrollDirection() == ScrollHorizontal)
				{
					float inp = p.x - getPosition().x - sprS.w/2;
					
					setScrollState(inp / (s.w - sprS.w));
				}
				else
				{
					float inp = p.y - getPosition().y - sprS.h/2;
					
					setScrollState(1 - inp / (s.h - sprS.h));
				}
				
				setupDraws();
			}
				
			m_grabState = getScrollState();
			m_grabbed = true;
			m_grabp = p;
		}
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onRelease(fw::Mouse::Button button,fm::vec2 /* p */ )
	{
		if (button == fw::Mouse::Left)
		{
			m_grabbed = false;
		}
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onDraw(fg::ShaderManager &shader)
	{
		shader.useTexture(nullptr);
		shader.draw(m_railDraw);
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::setScrollDirection(ScrollDirection direction)
	{
		if (m_direction != direction)
		{
			if (getScrollDirection() != direction)
				setupDraws();
			
			m_direction = direction;
		}
	}
	
	/////////////////////////////////////////////////////////////
	ScrollDirection ScrollBar::getScrollDirection() const
	{
		if (m_direction == ScrollDefault)
		{
			fm::vec2 s = getSize();
			
			return s.w < s.h ? ScrollVertical : ScrollHorizontal;
		}
		
		return m_direction;
	}
}
