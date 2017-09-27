#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>

using namespace std;

class Triangle
{
public:
	vec3 a,b,c;
	int state;
	int depth;
	
	Triangle() : state(0) {}
	Triangle(vec3 a,vec3 b,vec3 c,int state = 0,int depth = 0) : a(a), b(b), c(c), state(state), depth(depth) {}
	
	void populate(Triangle *&tris)
	{
		if (state == 0)
		{
			vec3 midA = (b+c)/2;
			vec3 midB = (a+c)/2;
			vec3 midC = (b+a)/2;
			
			vec3 n = (a-b).cross(a-c).sgn();
			vec3 mid = (a+b+c)/3 + sqrt(5.f/6.f) / 2 * (a-b).length() * n;
			
			*(tris++) = Triangle(a,midC,midB,1,depth+1);
			*(tris++) = Triangle(b,midA,midC,1,depth+1);
			*(tris++) = Triangle(c,midB,midA,1,depth+1);
			
			*(tris++) = Triangle(midA,midB,mid,0,depth+1);
			*(tris++) = Triangle(midB,midC,mid,0,depth+1);
			*(tris++) = Triangle(midC,midA,mid,0,depth+1);
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

void buildDrawData(int depth,DrawData &dd,DrawData &ddWf,DrawData &ddGw)
{
	Mesh m;
	vector<Triangle> triangles;
	
	float invSqrt2 = 1 / sqrt(2);
	vec3 basePts[] = {vec3(1,0,-invSqrt2),vec3(-1,0,-invSqrt2),vec3(0,1,invSqrt2),vec3(0,-1,invSqrt2)};
	
	triangles.push_back(Triangle(basePts[0],basePts[1],basePts[2]));
	triangles.push_back(Triangle(basePts[1],basePts[3],basePts[2]));
	triangles.push_back(Triangle(basePts[2],basePts[3],basePts[0]));
	triangles.push_back(Triangle(basePts[3],basePts[1],basePts[0]));
	
	C(depth)
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
	
	vector<vec4> glowColors(triangles.size()*3);
	
	auto clrFromPos = [](vec3 p) -> vec4 {
		return vec4(p.sgn()/2 + vec3(.5,.5,.5),1);
	};
	
	auto glowFromTri = [&](const Triangle &t,int c) -> vec4 {
		
		if (t.depth < 2) return vec4::Black;
		
		if (c == 0) return clrFromPos(t.a);
		if (c == 1) return clrFromPos(t.b);
		if (c == 2) return clrFromPos(t.c);
		
		return vec4::White;
	};
	
	C(triangles.size())
	{
		m.pts[i*3 + 0] = triangles[i].a; m.clr[i*3 + 0] = clrFromPos(triangles[i].a), glowColors[i*3 + 0] = glowFromTri(triangles[i],0);
		m.pts[i*3 + 1] = triangles[i].b; m.clr[i*3 + 1] = clrFromPos(triangles[i].b), glowColors[i*3 + 1] = glowFromTri(triangles[i],1);
		m.pts[i*3 + 2] = triangles[i].c; m.clr[i*3 + 2] = clrFromPos(triangles[i].c), glowColors[i*3 + 2] = glowFromTri(triangles[i],2);
	}
	
	dd = m;
	
	m.clr.swap(glowColors);
	
	ddGw = m;
	
	C(m.clr.size()) m.clr[i] = vec4::Black;
	
	ddWf = m;
	
}

/*********
 *
 *
 *  Logika - átnéz    . 1hét 
 *  Nummód - átnéz    . 1hét 
 *  Anal   - átnéz    . 1hét
 *  OAF    - tesztek  . 1hét
 *  Sztech - feladat  . 2hét
 *
 *
 *********/

int main()
{
	GuiWindow win(vec2(640,480),"Rotx");
	win.setDepthTest(LEqual);
	win.setClearColor(vec4::Black);
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	
	bool running = true;
	Clock loopClk;
	Clock rotClk;
	
	ShaderManager *shader = ShaderManager::getDefaultShader();
	
	Camera &cam = shader->getCamera();
	cam.set3D(win.getSize(),vec3(3,3,3),vec3());
	
	vec2 glowSize = win.getSize() * .5;
	
	Texture glowTexture(glowSize);
	FrameBuffer glowFBO(glowTexture,FrameBuffer::DepthBuffer(glowSize));
	DrawData glowApplyDD;
	glowApplyDD.positions = {vec2(-1,-1),vec2(1,-1),vec2(1,1),vec2(-1,-1),vec2(1,1),vec2(-1,1)};
	glowApplyDD.texPositions = {vec2(0,0),vec2(1,0),vec2(1,1),vec2(0,0),vec2(1,1),vec2(0,1)};
	
	TextureConvolution blurConv({0.01598,0.060626,0.241843,0.383103,0.241843,0.060626,0.01598});
	
	DrawData dd,ddWf,ddGw;
	buildDrawData(5,dd,ddWf,ddGw);
	
	mat4 rotM;
	bool leftDown = false;
	vec2 mouseP;
	
	bool renderBlur = true;
	
	for (int loop=0;running;++loop)
	{
		if (loopClk.getSeconds() > 1)
		{
			loopClk.restart();
			
			win.setTitle("Rotx  " + fm::toString(loop/1) + "fps");
			
			loop = 0;
		}
		
		Event ev;
		while (win.popEvent(ev))
		{
			if (ev.type == Event::Closed) running = false;
			
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::Enter)
					glowTexture.copyToImage().scale(win.getSize()).flipVertically().saveToFile("glow.png");
					
				if (ev.key.code == Keyboard::G)
					renderBlur = !renderBlur;
			}
			
			if (ev.type == Event::Resized)
			{
				cam.setCanvasSize(win.getSize());
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
					
					rotM = Quat(cam.u(),-delta.x/80) * Quat(cam.r(),-delta.y/80) * rotM;
				}
			}
			
			if (ev.type == Event::MouseWheelMoved)
			{
				cam.setPosition(cam.getPosition() * pow(2,-ev.wheel.delta));
			}
		}
		
		win.clear();
		
		if (renderBlur)
		{
			shader->getModelStack().top(rotM);
			
			glowFBO.bind();
			glowFBO.clear(true,true);
			shader->draw(ddGw);
			
			blurConv.applyTo(glowTexture,51);
			
			
			FrameBuffer::bind(nullptr);
			FrameBuffer::setViewport(rect2s(vec2(),win.getSize()));
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1,1);
			shader->draw(dd);
			glDisable(GL_POLYGON_OFFSET_FILL);
			
			glDepthMask(GL_FALSE);
			cam.getViewStack().push(mat4());
			cam.getProjStack().push(mat4());
			shader->getModelStack().push(mat4());
			shader->setBlendMode(fg::Additive);
			glowTexture.setSmooth(true);
			shader->useTexture(glowTexture);
			
			shader->draw(glowApplyDD);
			
			shader->useTexture(nullptr);
			shader->setBlendMode(fg::Alpha);
			shader->getModelStack().pop();
			cam.getProjStack().pop();
			cam.getViewStack().pop();
			glDepthMask(GL_TRUE);
			
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			shader->draw(ddWf);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}
		else
		{
			shader->getModelStack().top(rotM);
			
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1,1);
			shader->draw(dd);
			glDisable(GL_POLYGON_OFFSET_FILL);
			
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			shader->draw(ddWf);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}
		
		win.swapBuffers();
		
		win.applyFpsLimit();
	}
	
	delete shader;
}
