#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "ColorPicker.hpp"

/////////////////////////////////////////////////////////////
/// @brief Listener base class for scrolling
///
/////////////////////////////////////////////////////////////
class ScrollListener : public MouseMoveListener
{
public:
	/////////////////////////////////////////////////////////////
	/// @brief Default constructor
	/// 
	/////////////////////////////////////////////////////////////
	ScrollListener();
	
	/////////////////////////////////////////////////////////////
	/// @brief Called when the element is scrolled
	/// 
	/// @param amount The amount the element is scrolled
	/// 
	/////////////////////////////////////////////////////////////
	virtual void onScroll(float amount);
};

/////////////////////////////////////////////////////////////
ScrollListener::ScrollListener()
{
	addEventHandler([&](fw::Event &ev) -> bool {
		
		if (ev.type == fw::Event::MouseWheelMoved)
		{
			if (mouseInside())
			{
				onScroll(ev.wheel.delta);
				
				return true;
			}
		}
		
		return false;
	});
}

/////////////////////////////////////////////////////////////
void ScrollListener::onScroll(float)
{
	
}

/////////////////////////////////////////////////////////////
/// @brief Base class for gui classes that can be scrolled
///
/////////////////////////////////////////////////////////////
class GuiScrollBar : public GuiElement, public ScrollListener, public CallbackUser<GuiScrollBar,float>
{
	fm::Size m_stateCount; ///< The number of states the scrller can be in (0 for inf)
	float m_state; ///< The current state in range [0,1]
	
public:
	/////////////////////////////////////////////////////////////
	/// @brief Default constructor
	/// 
	/// @param cont The owner context
	/// 
	/////////////////////////////////////////////////////////////
	GuiScrollBar(GuiContext &cont);
	
	/////////////////////////////////////////////////////////////
	/// @brief Called when the element is scrolled
	/// 
	/// @param amount The amount the element is scrolled
	/// 
	/////////////////////////////////////////////////////////////
	virtual void onScroll(float amount);
	
	/////////////////////////////////////////////////////////////
	/// @brief Set the number of states the scroller can be in
	/// 
	/// @param stateCount The number of states (0 for inf)
	/// 
	/////////////////////////////////////////////////////////////
	void setStateCount(fm::Size stateCount);
	
	/////////////////////////////////////////////////////////////
	/// @brief Set the current state
	/// 
	/// @param state The new current state (will be clamped to [0,1])
	/// 
	/////////////////////////////////////////////////////////////
	void setState(float state);
	
	/////////////////////////////////////////////////////////////
	/// @brief Get the number of states the scroller can be in
	/// 
	/// @return The number of states (0 for inf)
	/// 
	/////////////////////////////////////////////////////////////
	fm::Size getStateCount();
	
	/////////////////////////////////////////////////////////////
	/// @brief Get the current state
	/// 
	/// @return The current current state (will be clamped to [0,1])
	/// 
	/////////////////////////////////////////////////////////////
	float getState();
};

/////////////////////////////////////////////////////////////
GuiScrollBar::GuiScrollBar(GuiContext &cont) : GuiElement(cont),
											   m_stateCount(0),
											   m_state(0)
{
	
}

/////////////////////////////////////////////////////////////
void GuiScrollBar::onScroll(float amount)
{
	m_state = m_state + (m_stateCount ? amount / m_stateCount : amount);
	callCallback(amount);
}

/////////////////////////////////////////////////////////////
void GuiScrollBar::setStateCount(fm::Size stateCount)
{
	m_stateCount = stateCount;
}

/////////////////////////////////////////////////////////////
void GuiScrollBar::setState(float state)
{
	m_state = state;
}

/////////////////////////////////////////////////////////////
fm::Size GuiScrollBar::getStateCount()
{
	return m_stateCount;
}

/////////////////////////////////////////////////////////////
float GuiScrollBar::getState()
{
	return m_state;
}

int main()
{
	GuiWindow win(vec2(640,480),"gui");
	
	PushButton *pb = new PushButton(win,"push me");
	pb->setPosition(vec2(100,100));
	
	int push_count = 0;
	
	pb->setCallback([&](GuiButton &b) {
		b.setText("yayy pushed " + fm::toString(++push_count) + " times");
		b.setSize(b.getGuiText().getSize() + vec2(10,10));
		win.setClearColor(vec4::White);
	});
	
	pb->setSize(vec2(100,50));
	
	
	ColorPicker *cp = new ColorPicker(win);
	cp->setPosition(vec2(150,150));
	cp->setCallback([&](ColorPicker &cp,Color c) {
		win.setClearColor(c.rgba()/255.0);
	});
	
	GuiScrollBar *sb = new GuiScrollBar(win);
	sb->setPosition(vec2(160,160));
	sb->setSize(vec2(80,80));
	sb->setCallback([&](GuiScrollBar &,float a) {
		cp->raiseBrightness(pow(1.1,a));
	});
	
	
	win.getMainLayout().addChildElement(pb);
	win.getMainLayout().addChildElement(cp);
	win.getMainLayout().addChildElement(sb);
	
	
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




