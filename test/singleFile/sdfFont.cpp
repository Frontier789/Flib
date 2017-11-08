#include <Frontier.hpp>
#include <iostream>
#include <fstream>

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
	
	Particle(Uint32 type,vec2 pos,vec2 vel,vec2 acc,Anglef angVel,Anglef angAcc,Time lifeTime,Font f,vec3 baseColor) : 
			 m_vel(vel),
			 m_acc(acc),
			 m_angVel(angVel),
			 m_angAcc(angAcc),
			 m_lifeTime(lifeTime),
			 m_originalLife(lifeTime),
			 m_sprite(new FontSprite(std::move(f.getSprite(type,particleStyle)))),
			 m_baseColor(baseColor)
	{
		m_sprite->setPosition(pos - m_sprite->getSize() / 2);
		m_sprite->setDirection(vel.sgn());
	}
	
	~Particle() {delete m_sprite;}
	
	/////////////////////////////////////////////////////////////
	void onDraw(ShaderManager &/* shader */) override {}
	
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
		m_sprite->setColor(vec4(m_baseColor - vec3(sin(r*3.14159265358979),0,0),r));
		
		if (m_lifeTime.asSecs() < 0)
		{
			delete m_sprite;
			m_sprite = nullptr;
		}
	}
};

Glyph::Style Particle::particleStyle = Glyph::Regular;

int main()
{
	GuiWindow win(vec2(640,480),"sdf");
	win.setClearColor(vec4(.5,1));
	
	Font f = win.getDefaultFont();
	
	vector<Particle*> particles;
	
	f.setCharacterSize(13);
	auto &sprMgr = f.getSpriteManager(Particle::particleStyle);
	
	
	if (!sprMgr.getShaderLoadResult())
	{
		cout << "failed load, code:" << endl;
		
		f.loadSigDistFieldShader([](fg::ShaderSource &s){
			cout << s << endl;
		});
		
		cout << sprMgr.getShaderLoadResult() << endl;
		return 1;
	}
	
	GuiText *capText = new GuiText(win,"cap: \ncount:");
	win.getMainLayout().addChildElement(capText);
	
	bool running = true;
	for (;running;)
	{
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::FocusLost) running = false;
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left) 
				{
					int db = 10 + rand()%10;
					if (Keyboard::isKeyHeld(Keyboard::LShift))
						db *= 100;
					
					C(db)
					{
						f.setCharacterSize(13);
						particles.push_back(new Particle('A' + rand()%20,
														 ev.mouse,
														 pol2(rand()%300+200,deg(i/float(db)*360)),
														 vec2(rand()%11-5,rand()%21-10),
														 deg(5 + rand()%180),
														 deg(rand()%50),
														 seconds((rand()%500)/100.0+2),
														 f,
														 vec3(1)));
					}
				}
			}
		}
		
		capText->setText("cap: " + fm::toString(sprMgr.getCapacity()) + "\ncount: " + fm::toString(sprMgr.getSpriteCount()));
		
		for (auto *ptr : particles)
			ptr->onUpdate(win.getUpdateInterval());
		
		win.clear();
		
		f.forEachSpriteManager([&](FontSpriteManager &mgr){
			win.draw(mgr);
		});
		
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
	
	for (auto *ptr : particles)
		delete ptr;
}

