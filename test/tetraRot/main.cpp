#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>

using namespace std;

#include "Triangle.hpp"

void buildDrawData(int depth,DrawData &dd,DrawData &ddWf,DrawData &ddGw);

void printUsage()
{
	cout << "usage: \n" 
		 << "\tHold mouse button and drag to rotate\n"
		 << "\tScroll to zoom\n"
		 << "\tG: Toggle glow" << endl;
}

class Roti : public GuiElement, public ClickListener, public ScrollListener
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
	
	Transition<float> m_glowAm;
	bool m_doBlur;
	
	Clock m_fpsClk;
	size_t m_frames;
	Delegate<void,string> m_settitle;
	
	void init();
public:
	Roti(GuiContext &cont,Delegate<void,string> settitle);
	
	void onDraw(ShaderManager &) override;
	void onMouseMove(vec2 p,vec2 prev) override;
	void onScroll(float am) override;
	void onUpdate(const fm::Time &dt) override;
	
	bool onEvent(Event &ev) override;
};

void Roti::onUpdate(const fm::Time &)
{
	++m_frames;
	if (m_fpsClk.getSeconds() > 1) {
		m_fpsClk.setTime(m_fpsClk.getTime() - seconds(1));
		
		m_settitle("Rotx  " + fm::toString(m_frames) + "fps");
		
		m_frames = 0;
	}
}

bool Roti::onEvent(Event &ev)
{
	if (ev.type == Event::KeyPressed) {
		if (ev.key.code == Keyboard::Enter)
			m_glowTex.copyToImage().scale(getSize()).flipVertically().saveToFile("glow.png");
			
		if (ev.key.code == Keyboard::G)
		{
			if (!m_doBlur)
			{
				m_doBlur = true;
				m_glowAm.set(max<float>(m_glowAm.get(),1),50,seconds(.2));
			}
			else
				m_glowAm.set(m_glowAm.get(),0,seconds(.2));
		}
	}
	
	return false;
}

void Roti::onScroll(float am)
{
	m_cam.setPosition(m_cam.getPosition() * pow(2,-am));
}

Roti::Roti(GuiContext &cont,fm::Delegate<void,string> settitle) : 
	GuiElement(cont,cont.getSize()),
	m_shader(cont.getShader()),
	m_cam(m_shader.getCamera()),
	m_glowAm(50,50),
	m_doBlur(TextureConvolution::isAvailable()),
	m_frames(0),
	m_settitle(settitle)
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
	int blurAm = m_glowAm.get();
	if (m_doBlur && blurAm == 0)
		m_doBlur = false;
	
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shader.getModelStack().top(m_rotMat);
	
	if (m_doBlur) {
		m_glowFBO.bind();
		m_glowFBO.clear(true,true);
		m_shader.draw(m_ddGlow);
		
		m_blurConv.applyTo(m_glowTex,blurAm);
		
		
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
	} else 
	{
		FrameBuffer::applyDepthTest(fg::LEqual);
		
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1,1);
		m_shader.draw(m_dd);
		glDisable(GL_POLYGON_OFFSET_FILL);
		
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		m_shader.draw(m_ddWire);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
}

void Roti::onMouseMove(vec2 p,vec2 prev)
{
	if (isPressed(Mouse::Left)) {
		vec2 delta = p-prev;
		m_rotMat = Quat(m_cam.u(),-delta.x/80) * Quat(m_cam.r(),-delta.y/80) * m_rotMat;
	}
}

int main()
{
	GuiWindow wwin(vec2(640,480),"Rotx");
	printUsage();
	
	auto rot = new Roti(wwin,[&](string s){
		wwin.setTitle(s);
	});
	
	wwin.setClearColor(vec4::Black);
	wwin.getMainLayout().addChildElement(rot);
	
	return wwin.runGuiLoop();
}
