#include <Frontier.hpp>
#include <iostream>

using namespace std;

class ParSurfTest : public GuiElement, public ClickListener, public ScrollListener
{
	DrawData m_dd;
	ShaderManager m_shader;
	Camera &m_cam;
	Texture m_atex;
	Texture m_stex;
	Texture m_ntex;
public:
	ParSurfTest(GuiContext &cont) : 
		GuiElement(cont,cont.getSize()), 
		m_cam(m_shader.getCamera())
	{
		Mesh m = Mesh::getCube();
		vec2 uvs[] = {vec2(0,0),vec2(1,0),vec2(0,1),vec2(1,1)};
		for (fm::Size i=0;i<m.uvs.size();++i) m.uvs[i] = uvs[i%4];
		m.calcNormals();
		m.calcTangents();
		m_dd = m;
		
		m_cam.set3D(cont.getSize(),vec3(0,0,10),vec3());
		
		loadShader();
	}
	
	/////////////////////////////////////////////////////////////
	void loadShader()
	{
		Camera cam = m_cam;
		
		fm::Result res = m_shader.loadFromFiles("vert.glsl","frag.glsl");
		res += m_atex.loadFromFile("ambient.png");
		res += m_stex.loadFromFile("specular.png");
		res += m_ntex.loadFromFile("normal.png");
		m_atex.setSmooth(true);
		m_stex.setSmooth(true);
		m_ntex.setSmooth(true);
		
		if (!res) {
			cout << res << endl;
			exit(0);
		}
		
		m_shader.setUniform("u_atex",m_atex);
		m_shader.setUniform("u_stex",m_stex);
		m_shader.setUniform("u_ntex",m_ntex);
		
		m_cam = cam;
	}
	
	/////////////////////////////////////////////////////////////
	bool onEvent(Event &ev) override 
	{
		if (ev.type == Event::FocusGained) {
			loadShader();
		}
		
		return GuiElement::onEvent(ev);
	}
	
	/////////////////////////////////////////////////////////////
	void onDraw(fg::ShaderManager &) override 
	{
		m_shader.draw(m_dd);
	}
	
	/////////////////////////////////////////////////////////////
	void onMouseMove(fm::vec2 p,fm::vec2 prevP) override
	{
		if (isPressed(Mouse::Left)) {
			vec2 delta = p - prevP;
			
			vec3 r = m_cam.r();
			vec3 u = m_cam.v().cross(m_cam.r()).sgn();
			
			m_shader.getModelStack().preMul(Quat(r,delta.y*-.01)).preMul(Quat(u,delta.x*.01));
		}
	}
	
	/////////////////////////////////////////////////////////////
	void onScroll(float d,bool hori) override
	{
		if (!hori)
			m_cam.setPosition(m_cam.getPosition() * pow(.5,d));
	}
};

int main()
{
	GuiWindow win(vec2(640,480));
	win.setClearColor(vec4(.2));
	win.setDepthTest(fg::LEqual);
	
	win.getMainLayout().addChildElement(new ParSurfTest(win));
	
	win.runGuiLoop();
}
