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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/Gui/ScrollBar.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	void ScrollBar::setupRail()
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	bool ScrollBar::inDragArea(const fm::vec2 &p) const
	{
		fm::vec2 s  = getSize();
		fm::vec2 pt  = getPosition();
		fm::vec2 sprS = s * getScrollSize();
		
		if (getScrollDirection() == ScrollHorizontal)
		{
			return fm::rect2f(pt + fm::vec2((s.w-sprS.w)*getScrollState(),0),fm::vec2(sprS.w,s.h)).contains(p);
		}
		else
		{
			return fm::rect2f(pt + fm::vec2(0,(s.h-sprS.h)*(1 - getScrollState())),fm::vec2(s.w,sprS.h)).contains(p);
		}
	}

	/////////////////////////////////////////////////////////////
	ScrollBar::ScrollBar(GuiContext &cont) : GuiScrollBar(cont),
											 m_direction(ScrollDefault),
											 m_grabbed(false)
	{
		m_handleAnim.setSprite("norm" ,cont.getSprite("Button_Bckg_Norm" ));
		m_handleAnim.setSprite("press",cont.getSprite("Button_Bckg_Press"));
		m_handleAnim.setSprite("hover",cont.getSprite("Button_Bckg_Hover"));
		
		fg::Mesh rectangle = fg::Mesh::getRectangle();
		
		rectangle.clr.resize(rectangle.pts.size());
		C(rectangle.clr.size())
			rectangle.clr[i] = fm::vec4(.5,.5,.5,.5);
		
		m_railDraw = rectangle;
	}

	/////////////////////////////////////////////////////////////
	ScrollBar::ScrollBar(GuiContext &cont,fm::vec2s size,fm::Delegate<void,GuiScrollBar &> callback) : ScrollBar(cont)
	{
		setSize(size);
		setCallback(callback);
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::setSize(fm::vec2s size)
	{
		GuiScrollBar::setSize(size);
		
		setupRail();
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
		}
		else
		{
			m_handleAnim.setState(inDragArea(p) ? "hover" : "norm");
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
			}
				
			m_grabState = getScrollState();
			m_grabbed = true;
			m_grabp = p;
			
			m_handleAnim.setState("press");		
		}
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onRelease(fw::Mouse::Button button,fm::vec2 /* p */ )
	{
		if (button == fw::Mouse::Left)
		{
			m_grabbed = false;
			
			m_handleAnim.setState("norm");
		}
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onDraw(fg::ShaderManager &shader)
	{
		fm::vec2 p = getPosition();
		fm::vec2 s = getSize();
		fm::vec2 sprS = s * getScrollSize();
		
		fm::mat4 m1;
		
		if (getScrollDirection() == ScrollHorizontal)
		{
			m1 = fm::MATRIX::translation(p + s*fm::vec2(0,.5) - fm::vec2(0,2) + fm::vec2(sprS.w/2,0)) * fm::MATRIX::scaling(fm::vec2(s.w - sprS.w,4));
			m_handleAnim.setPosition(p + fm::vec2((s.w-sprS.w)*getScrollState(),0));
			m_handleAnim.setSize(fm::vec2(sprS.w,s.h));
		}
		else
		{
			m1 = fm::MATRIX::translation(p + s*fm::vec2(.5,0) - fm::vec2(2,0) + fm::vec2(0,sprS.h/2)) * fm::MATRIX::scaling(fm::vec2(4,s.h - sprS.h));
			m_handleAnim.setPosition(p + fm::vec2(0,(s.h-sprS.h)*(1 - getScrollState())));
			m_handleAnim.setSize(fm::vec2(s.w,sprS.h));
		}
		
		shader.getModelStack().push().mul(m1);
		shader.useTexture(nullptr);
		shader.draw(m_railDraw);
		shader.getModelStack().pop();
		
		m_handleAnim.onDraw(shader);
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::setScrollDirection(ScrollDirection direction)
	{
		m_direction = direction;
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
