#include <Frontier.hpp>
#include <iostream>
#include <fstream>

using namespace std;

void visualizeRange(const KDTree2 &tree,Mesh &m,fm::Size first,fm::Size last,fm::Size depth,fm::vec2 leftP,fm::vec2 rightP)
{
	if (first+1 == last) return;
	
	fm::Size mid = (first+last)/2;
	
	bool splitOnX = (depth%2 == 0);
	
	float axis = tree.getAxesPtr()[mid];
	
	m.pts.push_back(fm::vec3(splitOnX ? axis : leftP.x, splitOnX ? leftP.y  : axis,depth*10+10));
	m.pts.push_back(fm::vec3(splitOnX ? axis : rightP.x,splitOnX ? rightP.y : axis,depth*10+10));
	m.clr.push_back(splitOnX ? vec4::Red : vec4::Blue);
	m.clr.push_back(splitOnX ? vec4::Red : vec4::Blue);

	visualizeRange(tree,m,first,mid,depth+1,leftP,splitOnX ? vec2(axis,rightP.y) : vec2(rightP.x,axis));
	visualizeRange(tree,m,mid, last,depth+1,splitOnX ? vec2(axis,leftP.y) : vec2(leftP.x,axis),rightP);
}

DrawData visualize(const KDTree2 &tree)
{
	Mesh m;
	visualizeRange(tree,m,0,tree.getSize(),0,vec2(),vec2(640,480));
	DrawData dd(m);
	dd.clearDraws();
	dd.addDraw(0,m.pts.size(),fg::Lines);
	return dd;
}

DrawData buildPtsDraw(const vector<vec2> &pts)
{
	Mesh m;
	for (vec2 v : pts)
	{
		for (float a=0;a<360;a+=10)
		{
			m.pts.push_back(v);
			m.pts.push_back(v + pol2(3,deg(a)));
			m.pts.push_back(v + pol2(3,deg(a+10)));
			m.clr.push_back(vec4::Black);
			m.clr.push_back(vec4::Black);
			m.clr.push_back(vec4::Black);
		}
	}
	DrawData dd(m);
	dd.clearDraws();
	dd.addDraw(0,m.pts.size(),fg::Triangles);
	return dd;
}

int main()
{
	vector<vec2> pts(42);
	C(pts.size())
		pts[i] = vec2i(fmod((sin(i*12345.0+12.0)+1.0)*4234.12,1.0)*640.0,
					   fmod((sin(i*18765.0+102.12)+3.1)*4139.1,1.0)*480.0);
	
	KDTree2 nearestPoint(&pts[0],pts.size());
	
	GuiWindow win(vec2(640,480),"kdTree");
	win.setClearColor(vec4::White);
	
	win.getShader().getCamera().setFar(1);
	float zfar = 1;
	
	DrawData dd  = visualize(nearestPoint);
	DrawData ddP = buildPtsDraw(pts);
	DrawData ndd;
	ndd.positions = {vec2(0,0),vec2(0,0)};
	ndd.colors = {vec4::Lime,vec4::Lime};
	ndd.addDraw(0,2,fg::Lines);
	
	bool running = true;
	for (fm::Size loop=0;running;++loop)
	{
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::FocusLost) running = false;
			if (ev.type == Event::Resized)
			{
				Camera &cam = win.getShader().getCamera();
				cam.setFar(zfar);
			}
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::Enter) win.capture().saveToFile("shot.png");
				if (ev.key.code == Keyboard::Plus)
				{
					Camera &cam = win.getShader().getCamera();
					zfar+=10;
					cam.setFar(zfar);
				}
				if (ev.key.code == Keyboard::Minus)
				{
					Camera &cam = win.getShader().getCamera();
					zfar-=10;
					cam.setFar(zfar);
				}
			}
			if (ev.type == Event::MouseMoved)
			{
				vec2 A = nearestPoint(ev.motion);
				ndd.positions = {vec2(ev.motion),A};
			}
		}
		
		win.clear();
		win.draw(dd);
		win.draw(ddP);
		win.draw(ndd);
		
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}

