#include <Frontier.hpp>
#include <iostream>

using namespace std;

int main()
{
	Window win(vec2(640,480),"The falling sand");
	
	bool running = true;
	Clock fpsClock;
	
	while (running)
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
		
		Sleep(1.0 / 60.0 - fpsClock.restart());
	}
}
