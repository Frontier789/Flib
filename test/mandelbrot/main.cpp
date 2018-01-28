#include <Frontier.hpp>
#include <iostream>
#include <fstream>

using namespace std;

class Mandel : public GuiElement, public TransformListener
{
	ShaderManager m_shader;
	DrawData m_dd;
	fm::Size m_its;
	fm::Size m_fragID;
	fg::Texture m_palette;
public:
	/////////////////////////////////////////////////////////////
	Mandel(GuiContext &owner) : GuiElement(owner,owner.getSize() - vec2(50,0)),
								m_its(0),
								m_fragID(0)
	{
		setPosition(owner.getSize() - getSize());
		setupDraw();

		m_palette.loadFromFile("pal.png");
		m_palette.setRepeated(true);
		m_palette.setSmooth(false);

		nextShader(false);

		setOffset(owner.getSize()/2);
		setZoom(getSize().min()/4);
	}

	/////////////////////////////////////////////////////////////
	void setupDraw()
	{
		Mesh m;
		vec2 pt[] = {vec2(0,0),vec2(1,0),vec2(1,1),vec2(0,1)};

		for (vec2 p : pt)
		{
			vec2 pp = p*getSize() + getPosition();
			m.pts.push_back(pp / getOwnerContext().getSize());
		}

		m_dd = m;
		m_dd.clearDraws();
		m_dd.addDraw(fg::TriangleFan);
	}

	/////////////////////////////////////////////////////////////
	bool loadShader()
	{
		string id = fm::toString(m_fragID).str();
		fm::Result res = m_shader.loadFromFiles("shaders/vert.glsl","shaders/frag_" + id + ".glsl");

		if (!res)
		{
			cout << res << endl;
			return false;
		}

		m_shader.setUniform("u_winSize",vec2(getSize()));

		setIts(m_its);

		return true;
	}

	/////////////////////////////////////////////////////////////
	void nextShader(bool right)
	{
		if (right)
		{
			m_fragID++;
		}
		else if (m_fragID > 0)
		{
			m_fragID--;
		}

		m_its = 1;

		if (!loadShader())
		{
			cout << "exiting" << endl;
			exit(0);
		}
	}

	/////////////////////////////////////////////////////////////
	void capture(string file)
	{
		FrameBuffer fbo;
		Texture tex;

		tex.create(getSize());
		fbo.create(tex);

		fbo.bind();
		fbo.setClearColor(vec4::White);
		fbo.clear();

		onDraw(m_shader);

		tex.copyToImage().saveToFile(file);
	}

	/////////////////////////////////////////////////////////////
	void onDraw(fg::ShaderManager &/* shader */) override
	{
		m_shader.setUniform("u_mvt",getInvTransformMatrix());
		m_shader.setUniform("u_palette",m_palette);
		m_shader.draw(m_dd);
	}

	/////////////////////////////////////////////////////////////
	bool onEvent(fw::Event &ev) override
	{
		if (ev.type == fw::Event::Resized)
		{
			setSize(ev.size);
			m_shader.setUniform("u_winSize",vec2(getSize()));
		}

		if (ev.type == fw::Event::FocusGained)
		{
			loadShader();
		}

		if (ev.type == fw::Event::KeyPressed)
		{
			if (ev.key.code == Keyboard::Enter)
			{
				capture("mandel.png");
			}

			if (ev.key.code == Keyboard::Plus)
			{
				setIts(getIts()+1);
			}

			if (ev.key.code == Keyboard::Minus)
			{
				setIts(max<fm::Size>(getIts()-1,1));
			}
		}

		return GuiElement::onEvent(ev);
	}

	/////////////////////////////////////////////////////////////
	void setIts(fm::Size its)
	{
		m_its = its;
		m_shader.setUniform("u_it",(int)m_its);
	}

	/////////////////////////////////////////////////////////////
	fm::Size getIts() const
	{
		return m_its;
	}
};

int main()
{
	GuiWindow win(vec2(640+50,480),"shady");
	win.setClearColor(vec4(.3,1));
	win.enableKeyRepeat();

	Mandel *m = new Mandel(win);
	auto &layout = win.getMainLayout();

	auto btnLayout = new GridLayout(win);

	btnLayout->setPadding(vec2(0,2));

	btnLayout->addChildElement(new PushButton(win,"it++",[&](){
		m->setIts(m->getIts()+1);
	}));
	btnLayout->addChildElement(new PushButton(win,"it--",[&](){
		m->setIts(max<float>(m->getIts()-1,1));
	}));
	btnLayout->addChildElement(new PushButton(win,"next>",[&](){
		m->nextShader(true);
	}));
	btnLayout->addChildElement(new PushButton(win,"<prev",[&](){
		m->nextShader(false);
	}));

	btnLayout->setColumnWidth(1,50);

	layout.addChildElement(m);
	layout.addChildElement(btnLayout);

	win.runGuiLoop();
}

