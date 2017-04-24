#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "defButtonImg.hpp"

namespace fgui
{
	class PushButton : public GuiElement, public ClickListener
	{
		fm::Delegate<void,PushButton &> m_callback;
		FramedSprite m_sprite;
		GuiText m_text;
		
	public:
		/////////////////////////////////////////////////////////////
		PushButton(GuiContext &cont);
		
		/////////////////////////////////////////////////////////////
		PushButton(GuiContext &cont,const fm::String &text,const FramedSprite &spr);
		
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
		
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size) override;
		
		/////////////////////////////////////////////////////////////
		virtual void setPosition(fm::vec2i pos) override;
		
		/////////////////////////////////////////////////////////////
		GuiText &getGuiText();
		
		/////////////////////////////////////////////////////////////
		const GuiText &getGuiText() const;
		
		/////////////////////////////////////////////////////////////
		void setCallback(fm::Delegate<void,PushButton &> callback);
		
		/////////////////////////////////////////////////////////////
		virtual void onPress(fw::Mouse::Button button,fm::vec2 p) override;
	};
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont) : GuiElement(cont),
											   m_text(cont)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont,const fm::String &text,const FramedSprite &spr) : GuiElement(cont),
																							  m_sprite(spr),
																							  m_text(cont,text)
	{
		setSize(m_text.getSize());
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::onDraw(fg::ShaderManager &shader)
	{
		m_sprite.onDraw(shader);
		
		m_text.onDraw(shader);
	}
		
	/////////////////////////////////////////////////////////////
	void PushButton::setSize(fm::vec2s size)
	{ 
		m_sprite.setSize(size + m_sprite.getFrameSize()*2);
		
		m_text.setPosition(getPosition() + m_sprite.getSize() / 2 - m_text.getSize() / 2);
		
		GuiElement::setSize(m_sprite.getSize());
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setPosition(fm::vec2i pos)
	{
		m_sprite.setPos(pos);
		
		m_text.setPosition(pos + getSize() / 2 - m_text.getSize() / 2);
		
		GuiElement::setPosition(pos);
	}
	
	/////////////////////////////////////////////////////////////
	GuiText &PushButton::getGuiText()
	{
		return m_text;
	}
	
	/////////////////////////////////////////////////////////////
	const GuiText &PushButton::getGuiText() const
	{
		return m_text;
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setCallback(fm::Delegate<void,PushButton &> callback)
	{
		m_callback = callback;
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::onPress(fw::Mouse::Button button,fm::vec2)
	{
		if (button == fw::Mouse::Left)
		{
			m_callback(*this);
		}
	}
}

int main()
{
	GuiWindow win(vec2(640,480),"gui");
	
	Texture tex = getDefButtonImg();
	tex.setSmooth(true);
	
	FramedSprite spr;
	spr.setFrameSize(vec2(4,4));
	spr.setTexture(tex);
	
	fgui::PushButton *pb = new fgui::PushButton(win,"push me",spr);
	pb->setPosition(vec2(100,100));
	
	pb->setCallback([&](){cout << "pushed" << endl;});
	
	win.getMainLayout().addChildElement(pb);
	
	spr.setPos(vec2(50,50));
	
	bool running = true;
	while (running)
	{
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					spr.setSize(vec2i(vec2(ev.mouse) - vec2(50,50))/2);
				}
			}
		}
		
		win.clear();
		win.draw(spr);
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}




