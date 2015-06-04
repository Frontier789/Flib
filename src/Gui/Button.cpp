#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/Gui/Button.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string Button::m_className = "Button";
	
	/////////////////////////////////////////////////////////////
	void Button::onHover(bool on)
	{
		(void)on;
	}
	
	/////////////////////////////////////////////////////////////
	void Button::onClick(bool on)
	{
		(void)on;
	}
	
	/////////////////////////////////////////////////////////////
	bool Button::contains(const fm::vec2 &p)
	{
		return fm::rect2f(getPosInRoot(),getSize()).contains(p);
	}
	
	/////////////////////////////////////////////////////////////
	Button::Button(const std::string &name,
				   const Anchor &anchor,
				   const fm::vec2 &size,
				   Widget *parent,
				   void (*callback)(Button *,fm::IntPtr),
				   const fm::IntPtr &userData) : Widget(name,anchor,size,parent),
												 m_callback(callback),
												 m_userData(userData),
												 m_cursorInside(false),
												 m_grabbedMouse(false)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Button::~Button()
	{
		
	}

	/////////////////////////////////////////////////////////////
	bool Button::handleEvent(const fw::Event &ev)
	{
		if (!getEnabled())
			return false;
		
		if (ev.type == fw::Event::MouseMoved || 
			ev.type == fw::Event::MouseLeft ||
			ev.type == fw::Event::ButtonPressed ||
			ev.type == fw::Event::ButtonReleased)
		{
			bool in = (ev.type != fw::Event::MouseLeft && contains(fm::vec2::loadxy(ev.motion)));
			
			if (in)
			{
				/// handle the cursor moving onto the button
				if (ev.type == fw::Event::MouseMoved)
				{
					if (!m_cursorInside && !m_grabbedMouse)
					{
						m_cursorInside = true;
						onHover(true);
						
						return true;
					}
				}
				
				/// handle the mouse pressing the button
				if (ev.type == fw::Event::ButtonPressed)
				{
					if (m_cursorInside && !m_grabbedMouse)
					{
						m_grabbedMouse = true;
						onClick(true);
						
						return true;
					}
				}
				
				/// handle the mouse releasing the button inside
				if (ev.type == fw::Event::ButtonReleased)
				{
					if (m_cursorInside && m_grabbedMouse)
					{
						m_grabbedMouse = false;
						onClick(false);
						
						// call user defined callback
						if (m_callback)
							m_callback(this,m_userData);
						
						return true;
					}
				}
			}
			else
			{
				/// handle the cursor moving away from the button
				if (ev.type == fw::Event::MouseMoved ||
					ev.type == fw::Event::MouseLeft)
				{
					if (m_cursorInside && !m_grabbedMouse)
					{
						m_cursorInside = false;
						onHover(false);
						
						return true;
					}
				}
				
				/// handle the mouse releasing the button outside it
				if (ev.type == fw::Event::ButtonReleased)
				{
					if (m_cursorInside && m_grabbedMouse)
					{
						m_grabbedMouse = false;
						onClick(false);
						
						m_cursorInside = false;
						onHover(false);
						
						return true;
					}
				}
			}
		}
		
		return false;
	}

	/////////////////////////////////////////////////////////////
	void Button::setCallback(void (*callback)(Button *,fm::IntPtr))
	{
		m_callback = callback;
	}

	/////////////////////////////////////////////////////////////
	void (*Button::getCallback())(Button *,fm::IntPtr)
	{
		return m_callback;
	}

	/////////////////////////////////////////////////////////////
	void Button::setUserData(fm::IntPtr userData)
	{
		m_userData = userData;
	}

	/////////////////////////////////////////////////////////////
	fm::IntPtr Button::getUserData()
	{
		return m_userData;
	}
	
	/////////////////////////////////////////////////////////////
	const std::string &Button::getRealType() const
	{
		return m_className;
	}
}