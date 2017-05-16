#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "ColorPicker.hpp"

/////////////////////////////////////////////////////////////
/// @brief Skinned scrollbar gui element
///
/////////////////////////////////////////////////////////////
class ScrollBar : public GuiScrollBar, public ClickListener
{
	SpriteAnimation m_handleAnim; ///< Sprite anim for handle
	DrawData m_railDraw; ///< DrawData for the rail
	float m_grabState;   ///< The state the scrollbar was in when it was grabbed
	fm::vec2 m_grabp;    ///< The point where the scroller was grabbed
	bool m_grabbed;      ///< True iff the scrolles is grabbed
	
	/////////////////////////////////////////////////////////////
	/// @brief Test if a point is inside the drag area
	/// 
	/// @param p The point
	/// 
	/// @return True iff p is in the drag area
	/// 
	/////////////////////////////////////////////////////////////
	virtual bool inDragArea(const fm::vec2 &p) const;
	
public:
	/////////////////////////////////////////////////////////////
	/// @brief Default constructor
	/// 
	/// @param cont The owning context
	/// 
	/////////////////////////////////////////////////////////////
	ScrollBar(GuiContext &cont);
	/*
	/////////////////////////////////////////////////////////////
	/// @brief Set the size of the gui element
	/// 
	/// @param size The size
	/// 
	/////////////////////////////////////////////////////////////
	virtual void setSize(fm::vec2s size) override;
		*/
	/////////////////////////////////////////////////////////////
	/// @brief Called when the mouse moves inside the gui element
	/// 
	/// @param p The position of the mouse after moving
	/// @param prevP The position of the mouse before moving
	/// 
	/////////////////////////////////////////////////////////////
	virtual void onMouseMoved(fm::vec2 p,fm::vec2 prevP) override;
	
	/////////////////////////////////////////////////////////////
	/// @brief Called when a mouse button is pressed while inside the gui element
	/// 
	/// @param button The button that was pressed
	/// @param p The position of the mouse
	/// 
	/////////////////////////////////////////////////////////////
	virtual void onPress(fw::Mouse::Button button,fm::vec2 p) override;
	
	/////////////////////////////////////////////////////////////
	/// @brief Called when a mouse button is released while inside the gui element
	/// 
	/// @param button The button that was released
	/// @param p The position of the mouse
	/// 
	/////////////////////////////////////////////////////////////
	virtual void onRelease(fw::Mouse::Button button,fm::vec2 p) override;
		
	/////////////////////////////////////////////////////////////
	/// @brief draw the gui element
	///
	/// @param shader The shader to use
	///
	/////////////////////////////////////////////////////////////
	virtual void onDraw(fg::ShaderManager &shader) override;
};
	/////////////////////////////////////////////////////////////
	bool ScrollBar::inDragArea(const fm::vec2 &p) const
	{
		vec2 s = getSize();
		vec2 pt = getPosition();
		
		float sprH = s.h * getScrollSize();
		
		return fm::rect2f(pt + vec2(0,(s.h-sprH)*getState()),vec2(s.w,sprH)).contains(p);
	}

	/////////////////////////////////////////////////////////////
	ScrollBar::ScrollBar(GuiContext &cont) : GuiScrollBar(cont),
											 m_grabbed(false)
	{
		m_handleAnim.setSprite("norm" ,cont.getSprite("Button_Bckg_Norm" ));
		m_handleAnim.setSprite("press",cont.getSprite("Button_Bckg_Press"));
		m_handleAnim.setSprite("hover",cont.getSprite("Button_Bckg_Hover"));
		m_handleAnim.setState("norm");
		
		fg::Mesh rectangle = fg::Mesh::getRectangle();
		
		rectangle.clr.resize(rectangle.pts.size());
		C(rectangle.clr.size())
			rectangle.clr[i] = vec4(.5,.5,.5,.5);
		
		m_railDraw = rectangle;
	}
	/*
	/////////////////////////////////////////////////////////////
	void ScrollBar::setSize(fm::vec2s size)
	{
		
	}
	*/
	/////////////////////////////////////////////////////////////
	void ScrollBar::onMouseMoved(fm::vec2 p,fm::vec2 prevP)
	{
		if (m_grabbed)
		{
			vec2 s = getSize();
			
			float state = m_grabState + (p.y - m_grabp.y) / (s.h - s.h * getScrollSize());
			
			setState(state);
		}
		else
		{
			if (inDragArea(p))
			{
				m_handleAnim.setState("hover");
			}
			else
			{
				m_handleAnim.setState("norm");
			}
		}
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onPress(fw::Mouse::Button button,fm::vec2 p)
	{
		if (button == fw::Mouse::Left)
		{
			if (!inDragArea(p))
			{
				vec2 s = getSize();
				
				float sprH = s.h * getScrollSize();
				
				float inp = p.y - getPosition().y - sprH/2;
				
				setState(inp / (s.h - sprH));
			}
				
			m_grabState = getState();
			m_grabbed = true;
			m_grabp = p;
			
			m_handleAnim.setState("press");		
		}
	}
	
	/////////////////////////////////////////////////////////////
	void ScrollBar::onRelease(fw::Mouse::Button button,fm::vec2 p)
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
		vec2 p = getPosition();
		vec2 s = getSize();
		
		float sprH = s.h * getScrollSize();
		
		mat4 m1 = MATRIX::translation(p + s*vec2(.5,0) - vec2(2,0) + vec2(0,sprH/2)) * MATRIX::scaling(vec2(4,s.h - sprH));
		
		shader.getModelStack().push().mul(m1);
		shader.useTexture(nullptr);
		shader.draw(m_railDraw);
		shader.getModelStack().pop();
		
		m_handleAnim.setPosition(p + vec2(0,(s.h-sprH)*getState()));
		m_handleAnim.setSize(vec2(s.w,sprH));
		m_handleAnim.onDraw(shader);
	}


int main()
{
	GuiWindow win(vec2(640,480),"gui");
	
	int push_count = 0;
	
	C(10)
	{
		PushButton *pb = new PushButton(win,"push me");	
		pb->setPosition(vec2(0,i * pb->getSize().h + 5*i));
		
		pb->setCallback([&](GuiButton &b) {
			b.setText("yayy pushed " + fm::toString(++push_count) + " times");
			b.setSize(b.getGuiText().getSize() + vec2(16,10));
			win.setClearColor(vec4::White);
		});
		
		win.getMainLayout().addChildElement(pb);
	}
	
	ColorPicker *cp = new ColorPicker(win);
	cp->setPosition(vec2(150,150));
	cp->setCallback([&](ColorPicker &cp,Color c) {
		win.setClearColor(c.rgba()/255.0);
	});
	
	win.getMainLayout().addChildElement(cp);
	
	ScrollBar *ssb = new ScrollBar(win);
	ssb->setSize(vec2(25,300));
	ssb->setPosition(vec2(640 - 40 - ssb->getSize().w,0));
	ssb->setCallback([&](GuiScrollBar &,float s){
		cp->raiseBrightness(s);
	});
	
	ssb->setScrollSize(0.05);
	
	win.getMainLayout().addChildElement(ssb);
	
	bool running = true;
	while (running)
	{
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
		}
		
		win.clear();
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}




