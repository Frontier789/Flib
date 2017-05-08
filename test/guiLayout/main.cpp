#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "ColorPicker.hpp"

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
	
	GuiScrollBar *sb = new GuiScrollBar(win);
	sb->setPosition(vec2(160,160));
	sb->setSize(vec2(80,80));
	sb->setCallback([&](GuiScrollBar &,float a) {
		cp->raiseBrightness(pow(1.1,a));
	});
	
	
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




