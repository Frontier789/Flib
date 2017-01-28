#include <Frontier.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
	Window win(vec2(640,480),"The falling sand",Window::Default &~ Window::Resize &~ Window::Maximize);
		
	srand(time(0));
	
	bool running = true;
	Clock fpsClock;
	
	FixedShaderManager shader;
	DrawData drawdata;
	
	vector<vec2> points;
	vector<vec4> colors;
	
	vector<vector<bool> > field(win.getSize().w,vector<bool>(win.getSize().h,false));
	
	bool leftPressed  = false;
	bool rightPressed = false;
	
	vec2 prevMousep;
	Clock gravityClock;
	
	pol2 gravity(70,deg(90));
	
	shader.getModelStack().push(MATRIX::ortho(0,win.getSize().h,win.getSize().w,0,1,-1));
	
	for (fm::Uint64 loop=0;running;++loop)
	{
		Event ev;
		while (win.popEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				running = false;
			}
			
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::Right)
					gravity.angle -= deg(45);
					
				if (ev.key.code == Keyboard::Left)
					gravity.angle += deg(45);
			}
			
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					leftPressed = true;
					
					if (!rightPressed)
						prevMousep  = Mouse::getPosition(win);
				}
				
				if (ev.mouse.button == Mouse::Right)
				{
					rightPressed = true;
					
					if (!leftPressed)
						prevMousep  = Mouse::getPosition(win);
				}
			}
			
			if (ev.type == Event::ButtonReleased)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					leftPressed = false;
					
					if (!rightPressed)
						cout << "# of points = " << points.size() << endl;
				}
				if (ev.mouse.button == Mouse::Right)
				{
					rightPressed = false;
					
					if (!leftPressed)
						cout << "# of points = " << points.size() << endl;
				}
			}
		}
		if (points.size() || leftPressed || rightPressed)
		{
			vector<vector<bool> > nextField(win.getSize().w,vector<bool>(win.getSize().h,false));
		
			if (points.size())
			{
				double dt = gravityClock.restart();
				
				for (int i=0;i<points.size();++i)
				{
					vec2 delta = (vec2(gravity) + vec2(gravity).sgn().perp() * (rand()%301-150))*dt;
					
					if (rect2i(vec2(),win.getSize()-vec2(1,1)).contains(points[i]+delta))
					{
						vec2i p = points[i]+delta;
						
						if (!field[p.x][p.y] || p == vec2i(points[i]))
						{
							points[i] += delta;
						}
						
						nextField[int(points[i].x)][int(points[i].y)] = true;
					}
					else if (rect2i(vec2(),win.getSize()-vec2(1,1)).contains(points[i]))
					{
						nextField[int(points[i].x)][int(points[i].y)] = true;
					}
				}
				
				drawdata.positions.set(&points[0],points.size());
			}
			
			if (leftPressed || rightPressed)
			/*C(15)*/{
				vec2 pInWin = Mouse::getPosition(win) + vec2(rand()%31-15,rand()%31-15);
				
				bool add = true;
				
				if (rect2i(vec2(),win.getSize()-vec2(1,1)).contains(pInWin))
				{
					if (field[int(pInWin.x)][int(pInWin.y)])
						add = false;
					else 
						nextField[int(pInWin.x)][int(pInWin.y)] = true;
				}
				
				if (add)
				{
					points.push_back(pInWin);
				
					if (leftPressed && !rightPressed)
						colors.push_back((vec4(cos(colors.size()/150.0),sin(colors.size()/200.0 + 5.2),sin(colors.size()/600.0 + 15.2),1)+vec4(1,1,1,1))/2);
					
					if (!leftPressed && rightPressed)
						colors.push_back(vec4::White);
						
					if (leftPressed && rightPressed)
						colors.push_back(vec4(vec3(rand()%100,rand()%100,rand()%100).sgn(),1));
					
					drawdata.reset();
					drawdata.positions.set(&points[0],points.size());
					drawdata.colors.set(&colors[0],colors.size());
					drawdata.addDraw(0,points.size(),fg::Points);
					
					prevMousep = pInWin;
				}
				
			}
			
			Cxy(100,100)
				nextField[270+x][190+y] = true;
			
			swap(nextField,field);
		}
		
		
		win.clear();
		shader.draw(drawdata);
		win.swapBuffers();
		
		Sleep(1.0 / 60.0 - fpsClock.restart());
	}
}
