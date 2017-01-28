#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "HexArray.hpp"
#include "HexPlotter.hpp"
#include "HexTerrain.hpp"

int main()
{
	Window win(vec2(640,480),"Hexy",Window::Default /*&~ Window::Resize &~ Window::Maximize*/);
		
	srand(time(0));
	
	bool running = true;
	Clock fpsClock;
	
	FixedShaderManager shader;
	Camera cam;
	shader.setCamera(cam);
	cam.getProjStack().push(MATRIX::ortho(win.getSize().w*-0.5,win.getSize().h*-0.5,win.getSize().w*0.5,win.getSize().h*0.5));
	
	HexTerrain hex(1);
	hex.gen();
	
	vec2 viewPos;
	float viewZoom = 50;
	bool leftDown = false;
	vec2 lastMouse;
	
	for (;running;)
	{
		Event ev;
		while (win.popEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				running = false;
			}
			
			if (ev.type == Event::Resized)
			{
				cam.getProjStack().push(MATRIX::ortho(win.getSize().w*-0.5,win.getSize().h*-0.5,win.getSize().w*0.5,win.getSize().h*0.5));
				FrameBuffer::setViewport(rect2s(vec2(),win.getSize()));
			}
			
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::R)
				{
					hex.gen();
				}
				
				if (ev.key.code == Keyboard::S)
				{
					win.capture().saveToFile("hex.png");
				}
				
				if (ev.key.code == Keyboard::Plus)
				{
					hex.extendToTwice();
				}

				if (ev.key.code == Keyboard::Minus)
				{
					hex.reduceToHalf();
				}

				if ((ev.key.code >= Keyboard::Num0    && ev.key.code <= Keyboard::Num9) ||
					(ev.key.code >= Keyboard::Numpad0 && ev.key.code <= Keyboard::Numpad9))
				{
					int n;
					if (ev.key.code >= Keyboard::Num0 && ev.key.code <= Keyboard::Num9) n = ev.key.code - Keyboard::Num0;
					else n = ev.key.code - Keyboard::Numpad0;

					hex.setColorStyle(n);
				}
			}
			
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					leftDown  = true;
					lastMouse = Mouse::getPosition(win);
				}
			}
			
			if (ev.type == Event::ButtonReleased)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					leftDown = false;
				}
			}
			
			if (ev.type == Event::MouseMoved)
			{
				if (leftDown)
				{
					vec2 p = vec2i(ev.motion);
					
					viewPos += (p-lastMouse)*vec2(1,-1);
					
					lastMouse = p;
				}
			}
			
			if (ev.type == Event::MouseWheelMoved)
			{
				float s = 1;
				if (ev.wheel.delta > 0) s = 1.1;
				if (ev.wheel.delta < 0) s = 1.0/1.1;
				
				vec2 p = ((Mouse::getPosition(win)-win.getSize()/2.0)*vec2(1,-1));
				
				// x -> x*viewZoom + viewPos
				

				// viewPos += (1 - s)*p;
				
				viewZoom *= s;
			}
		}
		
		win.clear();
		shader.getModelStack().push().mul(MATRIX::translation(viewPos)*MATRIX::scaling(viewZoom,viewZoom));
		hex.onDraw(shader);
		shader.getModelStack().pop();
		win.swapBuffers();
		
		Sleep(1.0 / 60.0 - fpsClock.restart());
	}
}
