#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
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
	deque<DrawData*> drawdatas;
	
	vector<vec2> points;
	vector<vec4> colors;
	
	vector<vector<bool> > field(win.getSize().w,vector<bool>(win.getSize().h,false));
	
	bool leftPressed  = false;
	bool rightPressed = false;
	
	vec2 prevMousep;
	Clock gravityClock;
	Clock loopClk;
	
	pol2 gravity(70,deg(90));
	
	shader.getModelStack().push(MATRIX::ortho(0,win.getSize().h,win.getSize().w,0,1,-1));
	
	bool gPress = false;

	float sandRange = 15;
	
	for (fm::Uint64 loop=0;running;++loop)
	{
		if (loopClk.getSeconds() > 1)
		{
			loopClk.restart();
			
			win.setTitle("The falling sand  " + fm::toString(loop/1) + "fps");
			
			loop = 0;
		}
		
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
					
				if (ev.key.code == Keyboard::C)
				{
					points.clear();
					colors.clear();
					C(drawdatas.size()) delete drawdatas[i];
					drawdatas.clear();
				}
					
				if (ev.key.code == Keyboard::G)
				{
					gPress = true;
				}
			}
			
			if (ev.type == Event::KeyReleased)
			{
				if (ev.key.code == Keyboard::G)
				{
					gPress = false;
				}

				if (ev.key.code == Keyboard::Plus)
				{
					sandRange++;
				}

				if (ev.key.code == Keyboard::Minus)
				{
					sandRange--;
				}
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
				
				for (fm::Size i=0;i<points.size();++i)
				{
					vec2 appliedGravity = gravity;
					if (gPress)
					{
						vec2 mouseToP = Mouse::getPosition(win) - points[i];
						appliedGravity = mouseToP.sgn() * gravity.length * 2.1;
					}
					
					vec2 delta = (vec2(appliedGravity) + vec2(appliedGravity).sgn().perp() * (rand()%301-150))*dt;
					
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
				
				for (fm::Size i=0;i*5000<points.size();i++)
				{
					int n = 5000;
					if (i*5000 + n >= points.size()) n = points.size() - i*5000;
					
					drawdatas[i]->positions.set(&points[i*5000],n);
				}
			}
			
			if (leftPressed || rightPressed)
			{
				int addedVertices = 0;
				
				int num = 1;
				if (Keyboard::isKeyHeld(Keyboard::LShift) || Keyboard::isKeyHeld(Keyboard::RShift)) num *= 5;
				if (Keyboard::isKeyHeld(Keyboard::LCtrl)  || Keyboard::isKeyHeld(Keyboard::RCtrl))  num *= 3;
				
				C(num)
				{
					vec2 pInWin = Mouse::getPosition(win) + vec2(rand()%101-50,rand()%101-50).sgn()*(rand()%10000)/10000.f*sandRange;
					
					if (rect2i(vec2(),win.getSize()-vec2(1,1)).contains(pInWin))
					{
						if (!field[int(pInWin.x)][int(pInWin.y)])
						{
							nextField[int(pInWin.x)][int(pInWin.y)] = true;
							
							points.push_back(pInWin);
						
							if (leftPressed && !rightPressed)
							{
								float colCount = colors.size() / float(num);
								colors.push_back((vec4(cos(colCount/150.0),sin(colCount/200.0 + 5.2),sin(colCount/600.0 + 15.2),1)+vec4(1,1,1,1))/2);
							}
							
							if (!leftPressed && rightPressed)
								colors.push_back(vec4::White);
								
							if (leftPressed && rightPressed)
								colors.push_back(vec4(vec3(rand()%100,rand()%100,rand()%100).sgn(),1));
							
							++addedVertices;
						}
					}
				}
				
				if (addedVertices)
				{
					if (drawdatas.size() && drawdatas.back()->getDraw(0).drawLen < 5000)
					{
						fm::Size begInd    = drawdatas.size()*5000 - 5000;
						fm::Size present   = drawdatas.back()->getDraw(0).drawLen;
						fm::Size copyCount = min<fm::Size>(5000 - present,addedVertices);
						
						drawdatas.back()->reset();
						drawdatas.back()->positions.set(&points[begInd],copyCount + present);
						drawdatas.back()->colors.set(&colors[begInd],copyCount + present);
						drawdatas.back()->addDraw(0,copyCount + present,fg::Points);
						
						addedVertices -= copyCount;
					}
					
					while (addedVertices)
					{
						drawdatas.push_back(new DrawData);
						
						fm::Size begInd    = drawdatas.size()*5000 - 5000;
						fm::Size copyCount = min<fm::Size>(5000,addedVertices);
						
						drawdatas.back()->positions.set(&points[begInd],copyCount);
						drawdatas.back()->colors.set(&colors[begInd],copyCount);
						drawdatas.back()->addDraw(0,copyCount,fg::Points);
						
						addedVertices -= copyCount;
					}	
				}
				
				prevMousep = Mouse::getPosition(win);
			}
			
			
			Cxy(100,100)
				nextField[270+x][190+y] = true;
			
			swap(nextField,field);
		}
		
		win.clear();
		C(drawdatas.size())
			shader.draw(*drawdatas[i]);
		win.swapBuffers();
		
		Sleep(1.0 / 60.0 - fpsClock.getSeconds());
		fpsClock.restart();
	}
	
	C(drawdatas.size()) delete drawdatas[i];
}
