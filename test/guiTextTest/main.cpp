#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "Sizer.hpp"

int main()
{
	cout << "instructions: use Left,Up,Right and W,C,N also you can size the red rectangle with the mouse" << endl;
	GuiWindow win(vec2(640,480),"gui"); 
	
	GuiText t(win,u8"ár\t     víztűrő tükör\nfúrógép");
	t.setSize(vec2(340,140));
	t.setPosition(win.getSize()/2 - t.getSize()/2);
	t.setWrap(fgui::TextWrapOff);
	t.setAlign(fgui::TextAlignCenter);

	Sizer sizer(t);
	
	bool rdown = false;
	vec2 lastr;
	
	bool running = true;
	while (running)
	{
		Event ev;
		if (win.waitEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::Plus)   t.setCharacterSize(t.getCharacterSize()+1);
				if (ev.key.code == Keyboard::Minus)  t.setCharacterSize(t.getCharacterSize()-1);
				if (ev.key.code == Keyboard::Left)   t.setAlign(TextAlignLeft);
				if (ev.key.code == Keyboard::Right)  t.setAlign(TextAlignRight);
				if (ev.key.code == Keyboard::Up) t.setAlign(TextAlignCenter);
				if (ev.key.code == Keyboard::W)  t.setWrap(TextWrapWord);
				if (ev.key.code == Keyboard::N)  t.setWrap(TextWrapOff);
				if (ev.key.code == Keyboard::C)  t.setWrap(TextWrapChar);
			}
			
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Right)
				{
					rdown = true;
					lastr = ev.mouse;
				}
			}
			
			if (ev.type == Event::MouseMoved)
			{
				if (rdown)
				{
					vec2 p = ev.motion;
					
					t.setViewOffset(t.getViewOffset() + p - lastr);
					
					lastr = p;
				}
				
				float dist = vec2i(ev.motion).length();
				float sn1 = sin(deg(dist))*0.5+0.5;
				float sn2 = sin(deg(dist*2+50))*0.5+0.5;
				float sn3 = sin(deg(dist*3+80))*0.5+0.5;
				t.setColor(vec4(vec3(sn1,sn2,sn3)*.2,1));
			}
			
			if (ev.type == Event::ButtonReleased)
			{
				if (ev.mouse.button == Mouse::Right)
				{
					rdown = false;
				}
			}
			
			sizer.handleEvent(ev);
		}
		
		win.clear();
		win.draw(sizer);
		win.swapBuffers();
	}
}




