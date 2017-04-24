#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>

using namespace std;

class Triangle
{
public:
	vec3 a,b,c;
	int state;
	
	Triangle() : state(0) {}
	Triangle(vec3 a,vec3 b,vec3 c,int state = 0) : a(a), b(b), c(c), state(state) {}
	
	void populate(Triangle *&tris)
	{
		if (state == 0)
		{
			vec3 midA = (b+c)/2;
			vec3 midB = (a+c)/2;
			vec3 midC = (b+a)/2;
			
			vec3 n = (a-b).cross(a-c).sgn();
			vec3 mid = (a+b+c)/3 + sqrt(5.f/6.f) / 2 * (a-b).length() * n;
			
			*(tris++) = Triangle(a,midC,midB,1);
			*(tris++) = Triangle(b,midA,midC,1);
			*(tris++) = Triangle(c,midB,midA,1);
			
			*(tris++) = Triangle(midA,midB,mid);
			*(tris++) = Triangle(midB,midC,mid);
			*(tris++) = Triangle(midC,midA,mid);
		}
		if (state == 1)
		{
			*(tris++) = *this;
		}
	}
	
	int getNewPopCount()
	{
		if (state == 0)
			return 6;
		
		return 1;
	}
};

int main()
{
	Window win(vec2(640,480),"Roty",Window::Default /*&~ Window::Resize &~ Window::Maximize*/);
	win.setDepthTest(LEqual);
	
	bool running = true;
	Clock loopClk;
	Clock fpsClk;
	Clock rotClk;
	
	FixedShaderManager shader;
	
	shader.getCamera().set3D(win.getSize(),vec3(3,3,3),vec3());
	
	DrawData dd,ddWf;
	{
		Mesh m;
		vector<Triangle> triangles;
		
		float invSqrt2 = 1 / sqrt(2);
		vec3 basePts[] = {vec3(1,0,-invSqrt2),vec3(-1,0,-invSqrt2),vec3(0,1,invSqrt2),vec3(0,-1,invSqrt2)};
		
		triangles.push_back(Triangle(basePts[0],basePts[1],basePts[2]));
		triangles.push_back(Triangle(basePts[1],basePts[3],basePts[2]));
		triangles.push_back(Triangle(basePts[2],basePts[3],basePts[0]));
		triangles.push_back(Triangle(basePts[3],basePts[1],basePts[0]));
		
		C(5)
		{
			int popCount = 0;
			for (auto &tri : triangles) popCount += tri.getNewPopCount();
			
			vector<Triangle> buf(popCount);
			
			Triangle *ptr = &buf[0];
			for (auto &tri : triangles) tri.populate(ptr);
			
			buf.swap(triangles);
		}
		
		m.pts.resize(triangles.size()*3);
		m.clr.resize(triangles.size()*3);
		
		auto clrFromPos = [](vec3 p) -> vec4 {
			
			return vec4(p.sgn()/2 + vec3(.5,.5,.5),1);
			
		};
		
		C(triangles.size())
		{
			m.pts[i*3 + 0] = triangles[i].a; m.clr[i*3 + 0] = clrFromPos(triangles[i].a);
			m.pts[i*3 + 1] = triangles[i].b; m.clr[i*3 + 1] = clrFromPos(triangles[i].b);
			m.pts[i*3 + 2] = triangles[i].c; m.clr[i*3 + 2] = clrFromPos(triangles[i].c);
		}
		
		dd = m;
		
		C(m.clr.size()) m.clr[i] = vec4::Black;
		
		ddWf = m;
	}
	
	mat4 rotM;
	bool leftDown = false;
	vec2 mouseP;
	
	for (int loop=0;running;++loop)
	{
		if (loopClk.getSeconds() > 1)
		{
			loopClk.restart();
			
			win.setTitle("Roty  " + fm::toString(loop/1) + "fps");
			
			loop = 0;
		}
		
		Event ev;
		while (win.popEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				running = false;
			}
			
			if (ev.type == Event::Resized)
			{
				shader.getCamera().setScreenSize(win.getSize());
				FrameBuffer::setViewport(rect2s(vec2(),win.getSize()));
			}
			
			if (ev.type == Event::ButtonPressed)
			{
				leftDown = true;
				mouseP = vec2i(ev.mouse);
			}
			
			if (ev.type == Event::ButtonReleased)
			{
				leftDown = false;
			}
			
			if (ev.type == Event::MouseMoved)
			{
				if (leftDown)
				{
					vec2 p = vec2i(ev.motion);
					vec2 delta = p-mouseP;
					mouseP = p;
					
					rotM = Quat(shader.getCamera().u(),-delta.x/80) * Quat(shader.getCamera().r(),-delta.y/80) * rotM;
				}
			}
			
			if (ev.type == Event::MouseWheelMoved)
			{
				shader.getCamera().setPos(shader.getCamera().getPos() * pow(2,-ev.wheel.delta));
			}
		}
		
		win.clear();
		shader.getModelStack().top(rotM);
		
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1,1);
		shader.draw(dd);
		glDisable(GL_POLYGON_OFFSET_FILL);
		
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		shader.draw(ddWf);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		
		win.swapBuffers();
		
		Sleep(1.0 / 120.0 - fpsClk.getSeconds());
		fpsClk.restart();
	}
}
