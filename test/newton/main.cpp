#include <Frontier.hpp>
#include <iostream>

using namespace std;

int main()
{
	GuiWindow win(vec2(640,480),"Hexy",Window::Default &~ Window::Resize &~ Window::Maximize);
	
	Clock fpsClock;
	bool running = true;
	
	for (int loop=0;running;++loop)
	{
		Event ev;
		while (win.popEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				running = false;
			}
		}
		
		win.clear();
		
		win.swapBuffers();
		
		Sleep(1.0 / 60.0 - fpsClock.getSeconds());
		fpsClock.restart();
	}
}
