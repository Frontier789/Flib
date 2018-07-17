#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>

using namespace std;

#include "Triangle.hpp"

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

void printUsage()
{
	cout << "usage: \n" 
		 << "\tHold mouse button and drag to rotate\n"
		 << "\tScroll to zoom\n"
		 << "\tG: Toggle glow" << endl;
}

class Roti : public GuiElement, public MouseMoveListener
{
	ShaderManager &m_shader;
	Camera &m_cam;
	vec2 m_glowSize;
	Texture m_glowTex;
	FrameBuffer m_glowFBO;
	DrawData m_glowApplyDD;
	TextureConvolution m_blurConv;
	DrawData m_dd,m_ddWire,m_ddGlow;
	mat4 m_rotMat;
	
	void init();
public:
	Roti(GuiContext &cont);
	
	void onDraw(ShaderManager &) override;
	
	void onMouseMoved(vec2 p,vec2 prev) override;
};

Roti::Roti(GuiContext &cont) : 
	GuiElement(cont,cont.getSize()),
	m_shader(cont.getShader()),
	m_cam(m_shader.getCamera())
{
	init();
}

void Roti::init()
{
	m_cam.set3D(getSize(),vec3(3,3,3),vec3());
	m_glowSize = getSize() * .5;
	
	m_glowTex.create(m_glowSize);
	m_glowFBO.create(m_glowTex,FrameBuffer::DepthBuffer(m_glowSize));
	m_glowFBO.setDepthTest(LEqual);
	m_glowApplyDD.positions = {vec2(-1,-1),vec2(1,-1),vec2(1,1),vec2(-1,-1),vec2(1,1),vec2(-1,1)};
	m_glowApplyDD.texPositions = {vec2(0,0),vec2(1,0),vec2(1,1),vec2(0,0),vec2(1,1),vec2(0,1)};
	
	m_blurConv = TextureConvolution({0.01598,0.060626,0.241843,0.383103,0.241843,0.060626,0.01598});
	
	buildDrawData(5,m_dd,m_ddWire,m_ddGlow);
}
	
void Roti::onDraw(ShaderManager &)
{
	m_shader.getModelStack().top(m_rotMat);
	
	m_glowFBO.bind();
	m_glowFBO.clear(true,true);
	m_shader.draw(m_ddGlow);
	
	m_blurConv.applyTo(m_glowTex,50);
	
	
	FrameBuffer::bind(nullptr);
	FrameBuffer::setViewport(rect2s(vec2(),getSize()));
	FrameBuffer::applyDepthTest(fg::LEqual);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1,1);
	m_shader.draw(m_dd);
	glDisable(GL_POLYGON_OFFSET_FILL);
	
	glDepthMask(GL_FALSE);
	m_cam.getViewStack().push(mat4());
	m_cam.getProjStack().push(mat4());
	m_shader.getModelStack().push(mat4());
	m_shader.setBlendMode(fg::Additive);
	m_glowTex.setSmooth(true);
	m_shader.useTexture(m_glowTex);
	
	m_shader.draw(m_glowApplyDD);
	
	m_shader.useTexture(nullptr);
	m_shader.setBlendMode(fg::Alpha);
	m_shader.getModelStack().pop();
	m_cam.getProjStack().pop();
	m_cam.getViewStack().pop();
	glDepthMask(GL_TRUE);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	m_shader.draw(m_ddWire);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void Roti::onMouseMoved(vec2 p,vec2 prev)
{
	vec2 delta = p-prev;
	m_rotMat = Quat(m_cam.u(),-delta.x/80) * Quat(m_cam.r(),-delta.y/80) * m_rotMat;
}

int main()
{
	GuiWindow wwin(vec2(640,480),"Rotx");
	wwin.setClearColor(vec4::Black);
	wwin.getMainLayout().addChildElement(new Roti(wwin));
	return wwin.runGuiLoop();
	
	printUsage();
	
	GuiWindow win(vec2(640,480),"Rotx");
	win.setDepthTest(LEqual);
	win.setClearColor(vec4::Black);
	win.enableKeyRepeat();
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	
	bool running = true;
	Clock loopClk;
	Clock rotClk;
	
	ShaderManager &shader = win.getShader();
	
	Camera &cam = shader.getCamera();
	cam.set3D(win.getSize(),vec3(3,3,3),vec3());
	
	vec2 glowSize = win.getSize() * .5;
	
	Texture glowTexture(glowSize);
	FrameBuffer glowFBO(glowTexture,FrameBuffer::DepthBuffer(glowSize));
	glowFBO.setDepthTest(LEqual);
	DrawData glowApplyDD;
	glowApplyDD.positions = {vec2(-1,-1),vec2(1,-1),vec2(1,1),vec2(-1,-1),vec2(1,1),vec2(-1,1)};
	glowApplyDD.texPositions = {vec2(0,0),vec2(1,0),vec2(1,1),vec2(0,0),vec2(1,1),vec2(0,1)};
	
	TextureConvolution blurConv({0.01598,0.060626,0.241843,0.383103,0.241843,0.060626,0.01598});
	
	DrawData dd,ddWf,ddGw;
	buildDrawData(5,dd,ddWf,ddGw);
	
	mat4 rotM;
	bool leftDown = false;
	vec2 mouseP;
	
	Transition<float> glowAm(50,50);
	
	bool renderBlur = TextureConvolution::isAvailable();
	
	string title;

	for (int loop=0;running;++loop)
	{
		if (loopClk.getSeconds() > 1)
		{
			loopClk.restart();
			
			title = "Rotx  " + fm::toString(loop/1) + "fps";
			win.setTitle(title);
			
			loop = 0;
		}
		
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::Enter)
					glowTexture.copyToImage().scale(win.getSize()).flipVertically().saveToFile("glow.png");
					
				if (ev.key.code == Keyboard::G)
				{
					if (!renderBlur)
					{
						renderBlur = true;
						glowAm.set(max<float>(glowAm.get(),1),50,seconds(.2));
					}
					else
						glowAm.set(glowAm.get(),0,seconds(.2));
				}
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
		
		int blurAm = glowAm.get();
		if (renderBlur && blurAm == 0)
		{
			renderBlur = false;
		}
		
		if (renderBlur)
		{
			shader.getModelStack().top(rotM);
			
			glowFBO.bind();
			glowFBO.clear(true,true);
			shader.draw(ddGw);
			
			blurConv.applyTo(glowTexture,blurAm);
			
			
			win.bindDefaultFrameBuffer();
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1,1);
			shader.draw(dd);
			glDisable(GL_POLYGON_OFFSET_FILL);
			
			glDepthMask(GL_FALSE);
			cam.getViewStack().push(mat4());
			cam.getProjStack().push(mat4());
			shader.getModelStack().push(mat4());
			shader.setBlendMode(fg::Additive);
			glowTexture.setSmooth(true);
			shader.useTexture(glowTexture);
			
			shader.draw(glowApplyDD);
			
			shader.useTexture(nullptr);
			shader.setBlendMode(fg::Alpha);
			shader.getModelStack().pop();
			cam.getProjStack().pop();
			cam.getViewStack().pop();
			glDepthMask(GL_TRUE);
			
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			shader.draw(ddWf);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}
		else
		{
			shader.getModelStack().top(rotM);
			
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1,1);
			shader.draw(dd);
			glDisable(GL_POLYGON_OFFSET_FILL);
			
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			shader.draw(ddWf);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}

		win.swapBuffers();
		
		win.applyFpsLimit();
	}
}
