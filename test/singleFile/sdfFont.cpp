#include <Frontier.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


class Particle : fg::Drawable
{
	vec2 m_vel;
	vec2 m_acc;
	Anglef m_angVel;
	Anglef m_angAcc;
	Time m_lifeTime;
	Time m_originalLife;
	FontSprite *m_sprite;
	vec3 m_baseColor;
	
public:
	static Glyph::Style particleStyle;
	
	Particle(vec2 pos,vec2 vel,vec2 acc,Anglef angVel,Anglef angAcc,Time lifeTime,FontSprite &&spr,vec3 baseColor) : 
			 m_vel(vel),
			 m_acc(acc),
			 m_angVel(angVel),
			 m_angAcc(angAcc),
			 m_lifeTime(lifeTime),
			 m_originalLife(lifeTime),
			 m_sprite(new FontSprite(std::move(spr))),
			 m_baseColor(baseColor)
	{
		m_sprite->setPosition(pos - m_sprite->getSize() / 2);
		m_sprite->setDirection(vel.sgn());
	}
	
	~Particle() {delete m_sprite;}
	
	/////////////////////////////////////////////////////////////
	void onDraw(ShaderManager &/* shader */) override {}
	
	/////////////////////////////////////////////////////////////
	void applyGravity(vec2 o,float w,const fm::Time &dt)
	{
		if (!m_sprite) return;
		
		vec2 p = m_sprite->getPosition() + m_sprite->getSize()/2;
		
		m_vel += (o-p).sgn() * w * dt.asSecs();
	}
	
	/////////////////////////////////////////////////////////////
	void onUpdate(const fm::Time &dt) override 
	{
		if (!m_sprite) return;
		
		m_vel += m_acc * dt.asSecs();
		m_angVel += m_angAcc * dt.asSecs();
		m_lifeTime -= dt.asSecs();
		
		pol2 polVel = m_vel;
		polVel.angle += m_angVel * dt.asSecs();
		m_vel = polVel;
		
		vec2 p = m_sprite->getPosition() + m_sprite->getSize()/2;
		
		p += m_vel * dt.asSecs();
		
		m_sprite->setPosition(p - m_sprite->getSize()/2);
		m_sprite->setDirection(-m_vel.sgn());
		
		float r = m_lifeTime / m_originalLife;
		m_sprite->setColor(vec4(m_baseColor - vec3(sin<float>(r*3.14159265358979)*.3,0,0),sin<float>(3.14159265358979*sqrt(1-r))));
		
		if (m_lifeTime.asSecs() < 0)
		{
			delete m_sprite;
			m_sprite = nullptr;
		}
	}
};

Glyph::Style Particle::particleStyle = Glyph::Regular;

#include <random>

class Random
{
	std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dis;
public:
	Random() : m_gen(42), m_dis(0, 1) {}

	double operator()() {return m_dis(m_gen);}

	fm::Size operator()(fm::Size below) {return fm::Size((*this)() * below); }
};

int main()
{
	GuiWindow win(vec2(640,480),"sdf");
	win.setClearColor(vec4(.5,1));

	win.getShader().setBlendMode(fg::Additive);
	
	Font f = win.getDefaultFont();
	
	vector<Particle*> particles;
	
	f.setCharacterSize(13);
	auto &sprMgr = f.getSpriteManager(Particle::particleStyle);
	
	Random rand;
	
	if (!sprMgr.getShaderLoadResult())
	{
		cout << "failed load, code:" << endl;
		
		f.loadSigDistFieldShader([](fg::ShaderSource &s){
			cout << s << endl;
		});
		
		cout << sprMgr.getShaderLoadResult() << endl;
		return 1;
	}
	
	GuiText *capText = new GuiText(win,"cap: \ncount: \nparticles:");
	GuiText *updText = new GuiText(win,"update: 0ms");
	GuiText *drwText = new GuiText(win,"draw: 0ms");

	updText->setPosition(capText->getSize() * vec2(0,1) + capText->getPosition() * vec2(0,1));
	drwText->setPosition(updText->getSize() * vec2(0,1) + updText->getPosition() * vec2(0,1));
/*
	capText->setColor(vec4::White);
	updText->setColor(vec4::White);
	drwText->setColor(vec4::White);
*/
	win.getMainLayout().addChildElement(capText);
	win.getMainLayout().addChildElement(updText);
	win.getMainLayout().addChildElement(drwText);
	
	vec2 lastMouseP;
	bool gravity = false;
	bool running = true;
	for (;running;)
	{
		fm::Time updTime;

		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::FocusLost) running = false;
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::D)
				{
					gravity = true;
					lastMouseP = Mouse::getPosition();
				}
				if (ev.key.code == Keyboard::C)
				{
					for (auto *ptr : particles)
						delete ptr;

					particles.clear();
				}
			}
			if (ev.type == Event::MouseMoved)
			{
				lastMouseP = vec2(ev.motion);
			}
			if (ev.type == Event::KeyReleased)
			{
				if (ev.key.code == Keyboard::D)
				{
					gravity = false;
				}
			}
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left) 
				{
					Clock clk;

					int db = 10 + rand(10);
					if (Keyboard::isKeyHeld(Keyboard::LShift))
						db *= 100;

					f.setCharacterSize(13);
					
					String s = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
					
					String letters(db,' ');
					
					for (auto &c : letters)
						c = s[rand(s.size())];
					
					std::vector<FontSprite> sprs = f.getSprites(letters,Particle::particleStyle);
					
					C(db)
					{
						particles.push_back(new Particle(vec2(ev.mouse),
														 pol2(rand(300)+200,deg(i/float(db)*360)),
														 vec2(rand(11)-5.f,rand(21)-10.f),
														 deg(5 + rand(180)),
														 deg(rand(50)),
														 seconds((rand(500))/100.0+2),
														 std::move(sprs[i]),
														 vec3(1)));
					}

					updTime += clk.getTime();
				}
			}
		}
		
		Clock clk;
		if (gravity)
		{
			for (Particle *particle : particles)
				particle->applyGravity(lastMouseP,500,win.getUpdateInterval() * (min<int>(sprMgr.getSpriteCount(),2100) / 1000.0 + 0.1));
			
		}

		for (auto *ptr : particles)
			ptr->onUpdate(win.getUpdateInterval() * (min<int>(sprMgr.getSpriteCount(),2100) / 1000.0 + 0.1));
		
		updTime += clk.getTime();
		
		capText->setText("cap: " + fm::toString(sprMgr.getCapacity()) + "\ncount: " + fm::toString(sprMgr.getSpriteCount()) + "\nparticles: " + fm::toString(particles.size()));
		updText->setText("upd: " + fm::toString(updTime.asMilsecs(),2) + "ms");

		win.clear();
		
		fg::TimeQuery query;
		query.start();
		f.onDrawSprites(win.getShader());
		query.stop();
		drwText->setText("drw: " + fm::toString(query.getTime().asMilsecs(),3) + "ms");
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
	
	for (auto *ptr : particles)
		delete ptr;
}

